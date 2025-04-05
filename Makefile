all: corrgram

corrgram: lexical.c syntax.c symtab.c quad.c
	gcc -o corrgram lexical.c syntax.c symtab.c quad.c -lfl

syntax.c syntax.tab.h: syntax.y
	
	bison --defines=syntax.tab.h -o syntax.c syntax.y 

lexical.c: lexical.l syntax.tab.h
	flex -o lexical.c lexical.l

clean:
	rm -f corrgram lexical.c syntax.c syntax.tab.h

