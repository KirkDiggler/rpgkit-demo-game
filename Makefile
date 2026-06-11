.PHONY: build run clean

build:
	cmake -B build
	cmake --build build -j

run: build
	./build/rpgkit_demo_game

clean:
	rm -rf build
