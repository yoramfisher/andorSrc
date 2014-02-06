To Build:

1)Include licin-test or link to libcin-test under AndorSRC/
2)Under libcin-test/:
	$make clean
	$make
3)Under AndorSRC/:
	$make clean
	$make refresh_libcin   (Pulls lib/ and cin.h for libcin-test and copies to AndorSRC/
	$make
