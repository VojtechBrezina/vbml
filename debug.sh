make -j$(($(nproc) * 2)) -C .. compile-debug && valgrind ../build/debug/vbml $@
