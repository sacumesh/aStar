# test: test.o List.o
# 	gcc -o test test.o List.o

# test.o: test.c
# 	gcc -c test.c

# tList: tList.o List.o status.o
# 	gcc -o tList List.o status.o tList.o

# tList.o: tList.c
# 	gcc -c tList.c

# List.o: List.c
# 	gcc -c List.c

# status.o: status.c
# 	gcc -c status.o

main: main.o Graph.o List.o status.o
	gcc -o main Graph.o main.o List.o status.o

Graph.o: Graph.c
	gcc -c 	graph.c

List.o:  List.c
	gcc -c List.c	

status.o: status.c
	gcc -c status.o


