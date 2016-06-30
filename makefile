#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

path: pathdisplay.c path_interpreter.c
	$(CC) $(DEBG) $(MESG) -o pathdisplay pathdisplay.c path_interpreter.c
pathtest: path_tests.c path_interpreter.c string_manip.c
	$(CC) $(DEBG) $(MESG) -o unit_path path_tests.c path_interpreter.c string_manip.c
	./unit_path
stringtest: string_manip.c string_manip_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_string string_manip.c string_manip_tests.c
	./unit_string
systemtest: system_info.c system_info_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_system system_info.c system_info_tests.c
	./unit_system
snap: snapshot.c
	$(CC) $(DEBG) $(MESG) -c snapshot.c -o frecov.o
print_messages: print_messages.c
	$(CC) $(DEBG) $(MESG) -c print_messages.c -o print_messages.o
frecov: snap print_messages
	$(CC) $(DEBG) $(MESG) -o frecov frecov.o print_messages.o -L. -lautoCurseMenu -lpanel -lncurses

clean:
	rm *.o
