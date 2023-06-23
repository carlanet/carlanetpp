all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile

makefiles:
	cd src && opp_makemake -f --make-so --deep -o carlanet -O out -KINET4_4_PROJ=../../inet4.4 -DINET_IMPORT -I$$\(INET4_4_PROJ\)/src -L$$\(INET4_4_PROJ\)/src -lzmq -lINET$$\(D\)

makexamples:
	cd examples && opp_makemake -f --deep -o carlanet -O out -KCARLANETPP_PROJ=.. -KINET4_4_PROJ=../../inet4.4 -DINET_IMPORT -I$$\(INET4_4_PROJ\)/src -I$$\(CARLANETPP_PROJ\)/src -L$$\(INET4_4_PROJ\)/src -L$$\(CARLANETPP_PROJ\)/src -lzmq -lcarlanet$$\(D\) -lINET$$\(D\) &&  $(MAKE)

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
