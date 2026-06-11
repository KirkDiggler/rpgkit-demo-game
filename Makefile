.PHONY: build run clean

build:
	cmake -B build
	cmake --build build -j

run: build
	./build/demo_game

clean:
	rm -rf build
