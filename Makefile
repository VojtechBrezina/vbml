compile:
	$(MAKE) -C build/release vbml

run: 
	$(MAKE) -C build/release run

compile-debug:
	$(MAKE) -C build/debug vbml

run-debug: 
	$(MAKE) -C build/debug run

.PHONY: compile compile-debug run run-debug

clean:
	$(MAKE) -C build/release clean 
	$(MAKE) -C build/debug clean 

