/* funcstack extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_funcstack.h"

ZEND_DECLARE_MODULE_GLOBALS(funcstack)

PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("funcstack.enable", "0", PHP_INI_ALL, OnUpdateBool, enable, zend_funcstack_globals, funcstack_globals)
    STD_PHP_INI_BOOLEAN("funcstack.deep", "0", PHP_INI_ALL, OnUpdateBool, deep, zend_funcstack_globals, funcstack_globals)
    STD_PHP_INI_BOOLEAN("funcstack.debug", "0", PHP_INI_ALL, OnUpdateBool, debug, zend_funcstack_globals, funcstack_globals)
    STD_PHP_INI_ENTRY("funcstack.logpath", "/tmp/func.stack", PHP_INI_ALL, OnUpdateString, logpath, zend_funcstack_globals, funcstack_globals)
PHP_INI_END()

static void php_funcstack_log(char *filename, char *fname, uint32_t lineno, char *inc){
    if(FS_G(debug)){
        printf("[+] %d %d [%s:%d] %s => %s\n", stack, level, filename, lineno, fname, inc);
    }else{
        FILE *fp;
        fp = fopen(FS_G(logpath), "ab+");
        if (fp) {
            fprintf(fp, "[+]\x99%d\x99%d\x99%s\x99%d\x99%s\x99%s\n", stack, level, filename, lineno, fname, inc);
            fclose(fp);
        }
    }
        
}

static int php_funcstack_opcode_handler(zend_execute_data *execute_data)
{
    const zend_op *opline = execute_data->opline;
    zend_execute_data *ex = EG(current_execute_data);
    zend_execute_data *call = execute_data->call;

    char *filename = "";
    uint32_t lineno = 0;
    char *fname = "";
    char *inc = "";

    while (ex && (!ex->func || !ZEND_USER_CODE(ex->func->type))) {
        ex = ex->prev_execute_data;
    }
    if (ex) {
        if (EG(exception) && ex->opline->opcode == ZEND_HANDLE_EXCEPTION &&
            ex->opline->lineno == 0 && EG(opline_before_exception)) {
            lineno = EG(opline_before_exception)->lineno;
        }else{
            lineno = ex->opline->lineno;
        }
        filename = ZSTR_VAL(ex->func->op_array.filename);
    } else {
        lineno = 0;
        filename = "";
    }

    switch(opline->opcode){
        case ZEND_DO_ICALL:
            if(!FS_G(deep))
                break;
        case ZEND_DO_UCALL:
        case ZEND_DO_FCALL:
        case ZEND_DO_FCALL_BY_NAME:
            level++;
            stack++;
            fname = ((zend_string *)((zend_function *)call->func)->common.function_name)->val;
            break;
        case ZEND_INCLUDE_OR_EVAL:
            level++;
            if (ZEND_INCLUDE_OR_EVAL == ZEND_EVAL){
                inc = "eval...";
            }else{
                inc = Z_STRVAL_P(RT_CONSTANT(opline, opline->op1));
            }
            switch (opline->extended_value) {
                case ZEND_INCLUDE_ONCE:
                    fname = "include_once";
                    break;
                case ZEND_REQUIRE_ONCE:
                    fname = "require_once";
                    break;
                case ZEND_INCLUDE:
                    fname = "include";
                    break;
                case ZEND_REQUIRE:
                    fname = "require";
                    break;
                case ZEND_EVAL:
                    fname = "eval";
                    break;
                }
            break;
        case ZEND_RETURN:
            level--;
            break;
    }
    if(opline->opcode == ZEND_DO_ICALL && !FS_G(deep))
        return ZEND_USER_OPCODE_DISPATCH;
    if (opline->opcode != ZEND_RETURN ){
        php_funcstack_log(filename, fname, lineno, inc);
    }
    return ZEND_USER_OPCODE_DISPATCH;
}


PHP_MINIT_FUNCTION(funcstack)
{
    REGISTER_INI_ENTRIES();
    if(FS_G(enable)){
        zend_set_user_opcode_handler(ZEND_DO_ICALL, php_funcstack_opcode_handler);
        zend_set_user_opcode_handler(ZEND_DO_UCALL, php_funcstack_opcode_handler);
        zend_set_user_opcode_handler(ZEND_DO_FCALL, php_funcstack_opcode_handler);
        zend_set_user_opcode_handler(ZEND_DO_FCALL_BY_NAME, php_funcstack_opcode_handler);
        // 
        zend_set_user_opcode_handler(ZEND_INCLUDE_OR_EVAL, php_funcstack_opcode_handler);
        // 
        zend_set_user_opcode_handler(ZEND_RETURN, php_funcstack_opcode_handler);
    }
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(funcstack)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_RINIT_FUNCTION(funcstack)
{
#if defined(ZTS) && defined(COMPILE_DL_FUNCSTACK)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
    if(FS_G(enable)){
        level = 0;
        stack = 0;
    }
	return SUCCESS;
}

static const zend_function_entry funcstack_functions[] = {
	PHP_FE_END
};

PHP_MINFO_FUNCTION(funcstack)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "funcstack support", "enabled");
    php_info_print_table_row(2, "Version", PHP_FUNCSTACK_VERSION);
    php_info_print_table_row(2, "Author", PHP_FUNCSTACK_AUTHOR);
	php_info_print_table_end();
}

zend_module_entry funcstack_module_entry = {
	STANDARD_MODULE_HEADER,
	"funcstack",					/* Extension name */
	funcstack_functions,			/* zend_function_entry */
	PHP_MINIT(funcstack),			/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(funcstack),		/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(funcstack),			/* PHP_RINIT - Request initialization */
	NULL,		                    /* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(funcstack),			/* PHP_MINFO - Module info */
	PHP_FUNCSTACK_VERSION,		    /* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_FUNCSTACK
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(funcstack)
#endif
