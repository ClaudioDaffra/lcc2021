
rm src/dagcheck.c
rm src/alpha.c
rm src/mips.c
rm src/sparc.c
rm src/x86linux.c
rm src/x86.c

bin/lburg src/dagcheck.md 		src/dagcheck.c
bin/lburg src/alpha.md 			src/alpha.c
bin/lburg src/mips.md 			src/mips.c
bin/lburg src/sparc.md 			src/sparc.c
bin/lburg src/x86linux.md 		src/x86linux.c
bin/lburg src/x86.md 			src/x86.c
	
vdir 		src/dagcheck.c
vdir		src/alpha.c
vdir  		src/mips.c
vdir 		src/sparc.c
vdir  		src/x86linux.c
vdir 		src/x86.c

rm bin/rcc

gcc -Wpointer-to-int-cast\
	src/alloc.c \
	src/bind.c \
	src/dag.c \
	src/decl.c \
	src/enode.c \
	src/error.c \
	src/expr.c \
	src/event.c \
	src/init.c \
	src/inits.c \
	src/input.c \
	src/lex.c \
	src/list.c \
	src/main.c \
	src/output.c \
	src/prof.c \
	src/profio.c \
	src/simp.c \
	src/stmt.c \
	src/string.c \
	src/sym.c \
	src/trace.c \
	src/tree.c \
	src/types.c \
	src/null.c \
	src/symbolic.c \
	src/bytecode.c \
	src/gen.c \
	src/stab.c \
	src/dagcheck.c \
	src/alpha.c \
	src/mips.c \
	src/sparc.c \
	src/x86linux.c \
	src/x86.c\
	-o bin/rcc
	

	
