main.exe: build
	cmake --build build
build:
	cmake -S . -B build
g git:
	git add .
	git commit -m "sync"
	git push
r:
	build/Automata
c:
	rm -rf build
