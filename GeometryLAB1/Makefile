
program: main.o stack.o input.o graphics.o scanline.o
	gcc main.o stack.o input.o graphics.o scanline.o -o program

main.o: main.c
	gcc -Wall -Wextra -c main.c

stack.o: stack.c
	gcc -Wall -Wextra -c stack.c

input.o: input.c
	gcc -Wall -Wextra -c input.c

graphics.o: graphics.c
	gcc -Wall -Wextra -c graphics.c

scanline.o: scanline.c
	gcc -Wall -Wextra -c scanline.c

clean:
	rm -f *.o program
