#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

path: pathdisplay.c
	$(CC) $(DEBG) $(MESG) -o pathdisplay pathdisplay.c
snap: snapshot.c
	$(CC) $(DEBG) $(MESG) -c snapshot.c -o frecov.o
frecov: snap
	$(CC) $(DEBG) $(MESG) -o frecov frecov.o -L. -lautoCurseMenu -lpanel -lncurses

clean:
	rm snapshot.o snapshot frecov frecov.o
