
CPP

C preprocessor, windows porting ( vs 2017 ).
from lcc https://github.com/ClaudioDaffra/lcc "
A Retargetable C Compiler"

win :  

	nmake /F wMakeFile		
	bin\cpp.exe -DDEBUG -D__STDC__ -C tst/ex0300.txt tst/x.out

lin : 

	make -f lMakefile		
	./bin/cpp	-DDEBUG -D__STDC__ -C tst/ex0300.txt tst/x.out