myFileSystem: main.o Tree.o Dict.o
	gcc main.o Tree.o Dict.o -o myFileSystem
Tree.o:Tree.c Tree.h
	gcc -c Tree.c
Dict.o: Dict.c Dict.h
	gcc -c Dict.c
main.o: main.c Dict.h Tree.h Dict.h
	gcc -c main.c
