#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

string_manip.o: string_manip.c string_manip.h
	$(CC) $(DEBG) $(MESG) -c -o string_manip.o string_manip.c
system_info.o: system_info.c system_info.h
	$(CC) $(DEBG) $(MESG) -c -o system_info.o system_info.c
print_messages.o: print_messages.c print_messages.h
	$(CC) $(DEBG) $(MESG) -c -o print_messages.o print_messages.c
path_interpreter.o: path_interpreter.c path_interpreter.h string_manip.o system_info.o print_messages.o
	$(CC) $(DEBG) $(MESG) -c -o path_interpreter.o path_interpreter.c


unit_path: path_interpreter.o
	$(CC) $(DEBG) $(MESG) -o unit_path path_tests.c path_interpreter.o system_info.o string_manip.o print_messages.o
	./unit_path
unit_string: string_manip.o string_manip_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_string string_manip.o string_manip_tests.c
	./unit_string

#TODO: fix up the rest of these makes
pathintegrationtest: pathdisplay.c path_interpreter.c string_manip.c system_info.c print_messages.c
	$(CC) $(DEBG) $(MESG) -o pathdisplay pathdisplay.c path_interpreter.c string_manip.c system_info.c print_messages.c
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
