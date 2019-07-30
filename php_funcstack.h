/* funcstack extension for PHP */

#ifndef PHP_FUNCSTACK_H
# define PHP_FUNCSTACK_H

extern zend_module_entry funcstack_module_entry;
# define phpext_funcstack_ptr &funcstack_module_entry

# define PHP_FUNCSTACK_VERSION "0.1.0"
# define PHP_FUNCSTACK_AUTHOR "Virink <virink@outlook.com>"

// # define EX(element) execute_data->element
# define OX(element) opline->element

static int level = 0;
static int stack = 0;

// PHP_MINIT_FUNCTION(funcstack);
// PHP_MSHUTDOWN_FUNCTION(funcstack);
// PHP_RINIT_FUNCTION(funcstack);
// PHP_RSHUTDOWN_FUNCTION(funcstack);
// PHP_MINFO_FUNCTION(funcstack);

# if defined(ZTS) && defined(COMPILE_DL_FUNCSTACK)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_FUNCSTACK_H */
