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

lines:
	wc -l `find src -type f`
