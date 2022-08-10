# just binding my \rm as usual
run: demo.html
	flatpak run org.mozilla.firefox demo.html

demo.html: examples/00-demo.vbml
	./vbml < examples/00-demo.vbml > demo.html

