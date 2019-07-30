/* funcstack extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_funcstack.h"

// #define DEBUG 1

static void php_funcstack_log(char *filename, char *fname, uint32_t lineno, zval *inc){
    // if(DEBUG){
        
    // }
    printf("[+] ==========================\n");
    printf("[+] %d %d [%s:(%d)] %s => %s\n", stack, level, filename, lineno, fname, inc);
    // printf("[+] %d %s:%d %s %s\n", stack, filename, lineno, Z_STRVAL_P(function_name),
    //     "");
}

static int php_funcstack_opcode_handler(zend_execute_data *execute_data)
{
    zend_op *opline = execute_data->opline;

    char *filename = "";
    uint32_t lineno = 0;

    char *fname = "";
    char *inc = "";


    // Get FileName and LineNo
    // zend_execute_data *ex = execute_data;
    zend_execute_data *ex = EG(current_execute_data);
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
        case ZEND_INIT_FCALL:
            // ???
            // if (!zend_is_executing()) break;
            // ??????
            // if(!deep)
                // break;
            level++;
        case ZEND_INIT_FCALL_BY_NAME:
            stack++;
            zend_function *fbc;
            fbc = CACHED_PTR(opline->result.num);
            if (UNEXPECTED(fbc == NULL)) {
                fname = Z_STRVAL_P((zval*)RT_CONSTANT(opline, opline->op2));
            }
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
    if (opline->opcode != ZEND_RETURN){
        php_funcstack_log(filename, fname, lineno, inc);
        // printf("[+] ==========================\n");
        // printf("[+] level : %d\n", level);
        // printf("[+] lineno : %d\n", lineno);
        // printf("[+] lineno : %d\n", lineno);
        // printf("[+] stack : %d\n", stack);
    }
    return ZEND_USER_OPCODE_DISPATCH;
}


PHP_MINIT_FUNCTION(funcstack)
{
    // zend_set_user_opcode_handler();
    // 初始化自定义函数
    zend_set_user_opcode_handler(ZEND_INIT_FCALL_BY_NAME, php_funcstack_opcode_handler);
    // 初始化内置函数
    zend_set_user_opcode_handler(ZEND_INIT_FCALL, php_funcstack_opcode_handler);

    // zend_set_user_opcode_handler(ZEND_DO_FCALL, php_funcstack_opcode_handler);
    // zend_set_user_opcode_handler(ZEND_DO_FCALL_BY_NAME, php_funcstack_opcode_handler);
    // 
    zend_set_user_opcode_handler(ZEND_INCLUDE_OR_EVAL, php_funcstack_opcode_handler);
    //
    // zend_set_user_opcode_handler(ZEND_ECHO, php_funcstack_opcode_handler);
    // zend_set_user_opcode_handler(ZEND_PRINT, php_funcstack_opcode_handler);
    //
    zend_set_user_opcode_handler(ZEND_RETURN, php_funcstack_opcode_handler);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(funcstack)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(funcstack)
{
#if defined(ZTS) && defined(COMPILE_DL_FUNCSTACK)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
    level = 0;
    stack = 0;
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(funcstack)
{
    return SUCCESS;
}

/* {{{ funcstack_functions[]
 */
static const zend_function_entry funcstack_functions[] = {
	PHP_FE_END
};
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(funcstack)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "funcstack support", "enabled");
    php_info_print_table_row(2, "Version", PHP_FUNCSTACK_VERSION);
    php_info_print_table_row(2, "Author", PHP_FUNCSTACK_AUTHOR);
	php_info_print_table_end();
}
/* }}} */

/* {{{ funcstack_module_entry
 */
zend_module_entry funcstack_module_entry = {
	STANDARD_MODULE_HEADER,
	"funcstack",					/* Extension name */
	funcstack_functions,			/* zend_function_entry */
	PHP_MINIT(funcstack),			/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(funcstack),		/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(funcstack),			/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(funcstack),		/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(funcstack),			/* PHP_MINFO - Module info */
	PHP_FUNCSTACK_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FUNCSTACK
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(funcstack)
#endif
