CC = g++

CFLAGS = -c -Wall

BLD_FOLDER = build
TEST_FOLDER = test

BLD_NAME = build
BLD_VERSION = 0.1
BLD_PLATFORM = linux
BLD_TYPE = dev
BLD_FORMAT = .out

BLD_FULL_NAME = $(BLD_NAME)_v$(BLD_VERSION)_$(BLD_TYPE)_$(BLD_PLATFORM)$(BLD_FORMAT)

all: main

MAIN_OBJECTS = main.o argparser.o logger.o debug.o ll_stack.o
main: $(MAIN_OBJECTS)
	mkdir -p $(BLD_FOLDER)
	$(CC) $(MAIN_OBJECTS) -o $(BLD_FOLDER)/$(BLD_FULL_NAME)

run:
	cd $(BLD_FOLDER) && exec ./$(BLD_FULL_NAME) $(ARGS)

main.o:
	$(CC) $(CFLAGS) main.cpp

argparser.o:
	$(CC) $(CFLAGS) lib/util/argparser.cpp

logger.o:
	$(CC) $(CFLAGS) lib/util/dbg/logger.cpp

debug.o:
	$(CC) $(CFLAGS) lib/util/dbg/debug.cpp

ll_stack.o:
	$(CC) $(CFLAGS) lib/ll_stack.cpp

clean:
	rm -rf *.o

rmbld:
	rm -rf $(BLD_FOLDER)
	rm -rf $(TEST_FOLDER)

rm: clean rmbld