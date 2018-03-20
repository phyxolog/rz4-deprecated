CFLAGS = -std=c++17 -DUNIX -DBIT64 -D_FILE_OFFSET_BITS=64 -m64 -O3 -Wall

BOOST_L = -lboost_system -lboost_filesystem

PROG_NAME = build/rz4
PROG_SRC = src/main.cpp\
src/util.cpp\
src/scan.cpp\
src/eject.cpp\
src/types.cpp

.PHONY: all clean

all: $(PROG_NAME)

$(PROG_NAME): $(PROG_SRC)
	mkdir -p build
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -L/usr/local $(BOOST_L)

clean:
	rm -f $(PROG_NAME)