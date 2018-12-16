default:
	clang -g -o run src/run.c
	clang -g -o compile src/compile.c

clean:
	rm -rf run compile
