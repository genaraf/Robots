rm robotcomp
#flex -8 scan.l
#bison -o comp.cpp --defines=comp.h -v comp.y
g++  comp.cpp lex.yy.c nt.cpp tree.cpp lt.cpp icode.cpp value.cpp interpret.cpp -o robotcomp
