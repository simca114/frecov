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
path_interpreter.o: path_interpreter.c system_info.h string_manip.o system_info.o print_messages.o
	$(CC) $(DEBG) $(MESG) -c -o path_interpreter.o path_interpreter.c


unit_path: path_interpreter.o
	$(CC) $(DEBG) $(MESG) -o unit_path path_tests.c path_interpreter.o system_info.o string_manip.o print_messages.o
	./unit_path
unit_string: string_manip.o string_manip_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_string string_manip.o string_manip_tests.c
	./unit_string
unit_system: system_info.o string_manip.o system_info_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_system system_info.o string_manip.o system_info_tests.c
	./unit_system

frecov.o: snapshot.c snapshot.h string_manip.o system_info.o print_messages.o path_interpreter.o
	$(CC) $(DEBG) $(MESG) -o frecov.o -c snapshot.c string_manip.o system_info.o print_messages.o path_interpreter.o
frecov: frecov.c frecov.h string_manip.o system_info.o print_messages.o path_interpreter.o
	$(CC) $(DEBG) $(MESG) -o frecov frecov.c string_manip.o system_info.o print_messages.o path_interpreter.o -L. -lautoCurseMenu -lpanel -lncurses

#TODO: fix up the rest of these makes
pathintegrationtest: pathdisplay.c path_interpreter.c string_manip.c system_info.c print_messages.c
	$(CC) $(DEBG) $(MESG) -o pathdisplay pathdisplay.c path_interpreter.c string_manip.c system_info.c print_messages.c
stringtest: string_manip.c string_manip_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_string string_manip.c string_manip_tests.c
	./unit_string
systemtest: system_info.c system_info_tests.c
	$(CC) $(DEBG) $(MESG) -o unit_system system_info.c system_info_tests.c
	./unit_system



clean:
	rm *.o
