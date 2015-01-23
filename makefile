#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

snapshot: snapshot.c
	$(CC) $(DEBG) $(MESG) -o snapshot snapshot.c -lmenu -lncurses

clean:
	rm snapshot
