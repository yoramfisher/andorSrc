To Build:

1)Include licin-test or link to libcin-test under AndorSRC/
2)Change configuration directory path in cin_power.c 
3)Under libcin-test/:
	$make clean
	$make
4)Under AndorSRC/:
	$make clean
	$make refresh_libcin   (Pulls lib/libcin.a, cin.h and cin_register_map.c from libcin-test and copies them into AndorSRC/
	$make
