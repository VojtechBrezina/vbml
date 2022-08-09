# just binding my \rm as usual
run:
	./vbml-convert < examples/00-demo.vbml > output.html
	flatpak run org.mozilla.firefox output.html
