TARGET = spholes_test
SRCDIR = ../src/spholes
INCDIR = $(SRCDIR)
CC = g++
CFLAGS = -std=c++14 -Wall -O2 -I$(INCDIR)

.PHONY: default all clean run

default: $(TARGET)
all: default

OBJECTS = $(patsubst $(SRCDIR)/%.cpp, %.o, $(wildcard $(SRCDIR)/*.cpp)) main.o
HEADERS = $(wildcard $(INCDIR)/*.h)
TESTS = $(wildcard *.h)

%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cpp $(HEADERS) $(TESTS) catch.hpp
	$(CC) $(CFLAGS) -c $< -o $@

catch.hpp:
	wget https://github.com/catchorg/Catch2/releases/download/v2.0.1/catch.hpp

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $^ -Wall -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
