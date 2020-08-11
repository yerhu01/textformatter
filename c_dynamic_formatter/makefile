CC=gcc
CFLAGS=-c -g -Wall -DDEBUG
LDFLAGS=

all: format265alt driver

format265alt: format265alt.o formatter.o
	$(CC) $(LDFLAGS) format265alt.o formatter.o -o format265alt

driver: driver.o formatter.o
	$(CC) $(LDFLAGS) driver.o formatter.o -o driver

driver.o: driver.c formatter.h
	$(CC) $(CFLAGS) driver.c

format265alt.o: format265alt.c formatter.h
	$(CC) $(CFLAGS) format265alt.c

formatter.o: formatter.c formatter.h
	$(CC) $(CFLAGS) formatter.c

clean:
	/bin/rm -f format265alt driver format265alt.o driver.o formatter.o
