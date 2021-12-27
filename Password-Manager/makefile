password_manager.out: main.o password_manager.o
	gcc -g -Wall -Werror -o password_manager.out main.o password_manager.o
main.o: main.c
	gcc -g -Wall -Werror -c main.c
password_manager.o: password_manager.c password_manager.h
	gcc -g -Wall -Werror -c password_manager.c
clean:
	rm -fr *.o *.out
