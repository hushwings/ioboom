#!/usr/bin/perl

for ($i = 0; $i < 20; $i++){
	system "./randrw /mnt/ramdisk/test1 0";
#	system "echo 3 > /proc/sys/vm/drop_caches";
}
