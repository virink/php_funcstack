--TEST--
Check if funcstack is loaded
--SKIPIF--
<?php
if (!extension_loaded('funcstack')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "funcstack" is available';
?>
--EXPECT--
The extension "funcstack" is available
