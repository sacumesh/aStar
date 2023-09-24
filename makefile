ALL = aStar
CC = gcc
CFLAGS = -Wall -Wextra

all: $(ALL)

aStar: aStarProject.o Map.o List.o status.o
	$(CC) -o $@ aStarProject.o Map.o List.o status.o

clean:
	rm -rf *.*o aStar
	rm -rf doc

distribute: clean
	tar zcvf dist.tgz *

aStarProject.o: aStarProject.c Map.h List.h status.h
	$(CC) $(CFLAGS) -c aStarProject.c

Map.o: Map.c Map.h List.h status.h
	$(CC) $(CFLAGS) -c Map.c

List.o: List.c List.h status.h
	$(CC) $(CFLAGS) -c List.c

status.o: status.c status.h
	$(CC) $(CFLAGS) -c status.c

doc:
	doxygen Doxyfile
