CC = g++

CFLAGS= -D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++\
-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations\
-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported\
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral\
-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op\
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith\
-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo\
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn\
-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default\
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast\
-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers\
-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector\
-fcheck-new\
-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging\
-fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,${strip \
}bounds,enum,float-cast-overflow,float-divide-by-zero,${strip \
}integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,${strip \
}returns-nonnull-attribute,shift,signed-integer-overflow,undefined,${strip \
}unreachable,vla-bound,vptr\
-pie -Wlarger-than=8192 -Wstack-usage=8192

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
	$(CC) $(MAIN_OBJECTS) $(CFLAGS) -o $(BLD_FOLDER)/$(BLD_FULL_NAME)

run:
	cd $(BLD_FOLDER) && exec ./$(BLD_FULL_NAME) $(ARGS)

main.o:
	$(CC) -c $(CFLAGS) main.cpp

argparser.o:
	$(CC) -c $(CFLAGS) lib/util/argparser.cpp

logger.o:
	$(CC) -c $(CFLAGS) lib/util/dbg/logger.cpp

debug.o:
	$(CC) -c $(CFLAGS) lib/util/dbg/debug.cpp

ll_stack.o:
	$(CC) -c $(CFLAGS) lib/ll_stack.cpp

clean:
	rm -rf *.o

rmbld:
	rm -rf $(BLD_FOLDER)
	rm -rf $(TEST_FOLDER)

rm: clean rmbld