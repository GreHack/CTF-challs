all: dist

dist: xordump
	mkdir -p xordump
	cp src/xordump xordump/
	strip xordump/xordump
	tar -zcf xordump.tar.gz xordump/

xordump:
	make -C src/ xordump

distclean:
	make -C src/ clean
	rm -rf xordump/
	rm xordump.tar.gz
