CFLAGS = -std=c++11 -DUNIX -DBIT64 -D_FILE_OFFSET_BITS=64 -m64 -O3 -Wall -pthread

PROG_NAME = dist/rz4
PROG_SRC = rz4.cpp\
compressor.cpp\
injector.cpp\
helper.cpp\
scanner.cpp\
extractor.cpp

.PHONY: all clean

all: $(PROG_NAME)

$(PROG_NAME): $(PROG_SRC)
	mkdir -p dist
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(PROG_NAME)