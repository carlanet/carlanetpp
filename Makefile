all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile

makefiles:
	cd src && opp_makemake -f --make-so --deep -Xcarlanet/lightcontrol -Xcarlanet/lightcontrol_v2 -o carlanet -O out -KINET_PROJ=../../inet-4.6.0 -DINET_IMPORT -I$$\(INET_PROJ\)/src -L$$\(INET_PROJ\)/src -I/opt/homebrew/Cellar/cppzmq/4.11.0/include -I/opt/homebrew/Cellar/zeromq/4.3.5_2/include -DZMQ_BUILD_DRAFT_API=1 -I/opt/homebrew/Cellar/libsodium/1.0.22/include -I/opt/homebrew/Cellar/nlohmann-json/3.12.0/include -L/opt/homebrew/Cellar/zeromq/4.3.5_2/lib -lzmq -lINET$$\(D\)

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
