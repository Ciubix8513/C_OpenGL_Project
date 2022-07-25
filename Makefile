CC=clang

all: main.o System.o Graphics.o FileLoader.o Input.o  Math.o global.h
	$(CC) -lglfw -lGL -lGLEW -lm main.o System.o Graphics.o FileLoader.o Input.o  Math.o -o main 
	@echo 'Compilation successful!'

main: main.c global.h
	$(CC)  -c -o $@
System: System.c System.h global.h
	$(CC)  -c -o $@
Graphics: Graphics.c Graphics.h global.h
	$(CC)  -c -o $@
FileLoader: FileLoader.c FileLoader.h global.h
	$(CC)  -c -o $@
Input: Input.c Input.h global.h
	$(CC)	-c -o $@	
Math: Math.c Math.h global.h
	$(CC)	-c -o $@
clean:
	$(RM) -rf *.o *.dYSM	