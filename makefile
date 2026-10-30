main.exe: build
	cmake --build build
build:
	cmake -S . -B build
r:
	build/Automata
c:
	rm -rf build
