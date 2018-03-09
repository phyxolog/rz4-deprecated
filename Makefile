CFLAGS = -std=c++17 -DUNIX -DBIT64 -D_FILE_OFFSET_BITS=64 -m64 -O3 -Wall

BOOST_L = -lboost_system -lboost_program_options -lboost_filesystem

PROG_NAME = dist/rz4
PROG_SRC = rz4.cpp\
compressor.cpp\
injector.cpp\
helper.cpp\
scanner.cpp\
extractor.cpp\
types.cpp\
args.cpp

.PHONY: all clean

all: $(PROG_NAME)

$(PROG_NAME): $(PROG_SRC)
	mkdir -p dist
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -L/usr/local $(BOOST_L)

clean:
	rm -f $(PROG_NAME)