tList: tList.o List.o status.o
	gcc -o tList List.o status.o tList.o

tList.o: tList.c
	gcc -c tList.c

List.o: List.c
	gcc -c List.c

status.o: status.c
	gcc -c status.o

