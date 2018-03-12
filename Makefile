CFLAGS = -std=c++17 -DUNIX -DBIT64 -D_FILE_OFFSET_BITS=64 -m64 -O3 -Wall

BOOST_L = -lboost_system -lboost_program_options -lboost_filesystem

PROG_NAME = build/rz4
PROG_SRC = src/rz4.cpp\
src/compressor.cpp\
src/injector.cpp\
src/helper.cpp\
src/scanner.cpp\
src/extractor.cpp\
src/types.cpp\
src/args.cpp

.PHONY: all clean

all: $(PROG_NAME)

$(PROG_NAME): $(PROG_SRC)
	mkdir -p build
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -L/usr/local $(BOOST_L)

clean:
	rm -f $(PROG_NAME)