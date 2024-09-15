all: src/database.c tables/*.c  main.c
	gcc -I./include src/database.c  tables/*.c main.c 
