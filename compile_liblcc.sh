gcc -c lib/assert.c -o lib/assert.o
gcc -c lib/bbexit.c -o lib/bbexit.o
gcc -c lib/yynull.c -o lib/yynull.o
ar rcs lib/liblcc.a lib/assert.o lib/bbexit.o lib/yynull.o

