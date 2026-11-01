main.exe: build
	cmake --build build
build:
	cmake -S . -B build
g git:
	git add .
	git commit -m "sync"
	git push
d doc:
	doxygen docs/Doxyfile
	$(MAKE) -C docs/latex
	cp docs/latex/refman.pdf docs/documentation.pdf
r:
	build/Automata
c:
	rm -rf build
