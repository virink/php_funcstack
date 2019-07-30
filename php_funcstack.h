/* funcstack extension for PHP */

#ifndef PHP_FUNCSTACK_H
# define PHP_FUNCSTACK_H

extern zend_module_entry funcstack_module_entry;
# define phpext_funcstack_ptr &funcstack_module_entry

# define PHP_FUNCSTACK_VERSION "0.1.0"
# define PHP_FUNCSTACK_AUTHOR "Virink <virink@outlook.com>"

static int level = 0;
static int stack = 0;

typedef struct _php_funcstack_globals {
    zend_bool   enable;
    zend_bool   deep;
    zend_bool   debug;
    char        *logpath;
} php_funcstack_globals;

typedef php_funcstack_globals zend_funcstack_globals;


#ifdef ZTS
#define FS_G(v) ZEND_TSRMG(funcstack_globals_id, php_funcstack_globals *, v)
#ifdef COMPILE_DL_FUNCSTACK
ZEND_TSRMLS_CACHE_EXTERN()
#endif
#else
#define FS_G(v) (funcstack_globals.v)
#endif

#endif	/* PHP_FUNCSTACK_H */
