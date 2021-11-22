CC = gcc
CFLAGS = -std=c89 -Wall
DEPS = file_handler.h
OBJ = main.o file_handler.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	del /f *.o