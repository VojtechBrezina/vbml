vbml: compile
	-rm vbml
	cp build/release/vbml .
	strip --strip-unneeded vbml

install: vbml
	cp vbml ~/.local/bin/

uninstall:
	-rm ~/.local/bin/vbml

compile:
	$(MAKE) -C build/release vbml

run:
	$(MAKE) -C build/release run

compile-debug:
	$(MAKE) -C build/debug vbml

run-debug:
	$(MAKE) -C build/debug run

test:
	$(MAKE) -C build/test run

.PHONY: compile compile-debug run run-debug test clean lines

clean:
	$(MAKE) -C build/release clean 
	$(MAKE) -C build/debug clean 
	$(MAKE) -C build/test clean 

lines:
	wc -l `find src -type f`
