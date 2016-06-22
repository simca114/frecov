#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

path: pathdisplay.c path_interpreter.c
	$(CC) $(DEBG) $(MESG) -o pathdisplay pathdisplay.c path_interpreter.c
pathtest: path_tests.c path_interpreter.c
	$(CC) $(DEBG) $(MESG) -o unit_path path_tests.c path_interpreter.c
snap: snapshot.c
	$(CC) $(DEBG) $(MESG) -c snapshot.c -o frecov.o
print_messages: print_messages.c
	$(CC) $(DEBG) $(MESG) -c print_messages.c -o print_messages.o
frecov: snap print_messages
	$(CC) $(DEBG) $(MESG) -o frecov frecov.o print_messages.o -L. -lautoCurseMenu -lpanel -lncurses

clean:
	rm *.o
