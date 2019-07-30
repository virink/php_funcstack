--TEST--
funcstack_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('funcstack')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = funcstack_test1();

var_dump($ret);
?>
--EXPECT--
The extension funcstack is loaded and working!
NULL
