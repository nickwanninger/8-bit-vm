default:
	clang -o run src/run.c
	clang -o compile src/compile.c

clean:
	rm -rf run compile
