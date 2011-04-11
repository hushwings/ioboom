#!/usr/bin/perl

for ($i = 0; $i < 20; $i++){
    system "./iotest /dev/raw/raw1 4g 4k 0 3";
#	system "echo 3 > /proc/sys/vm/drop_caches";
}
