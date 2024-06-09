SRC = ./src
BUILD = ./build
EXEC := collisions
CC := g++
CFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
build:
	$(CC) $(SRC)/$(EXEC).cpp -o $(BUILD)/$(EXEC).out  $(CFLAGS)
run:
	$(BUILD)/$(EXEC).out
clean:
	rm $(BUILD)/*
exec: build run



