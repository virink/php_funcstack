--TEST--
funcstack_test2() Basic test
--SKIPIF--
<?php
if (!extension_loaded('funcstack')) {
	echo 'skip';
}
?>
--FILE--
<?php
var_dump(funcstack_test2());
var_dump(funcstack_test2('PHP'));
?>
--EXPECT--
string(11) "Hello World"
string(9) "Hello PHP"
