#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

snap: snapshot.c
	$(CC) $(DEBG) $(MESG) -c snapshot.c -o snapshot.o
snapshot: snap
	$(CC) $(DEBG) $(MESG) -o snapshot snapshot.o -L. -lautoCurseMenu -lpanel -lncurses

clean:
	rm snapshot.o snapshot
