EXEC := collisions
CC := g++

build:
	$(CC) $(EXEC).cpp -o $(EXEC).out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
run:
	./$(EXEC).out
exec: build run

