build:
	gcc-12 src/compiler.c -o vm

clean:
	rm -rf vm