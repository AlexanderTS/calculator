CC=g++
CFLAGS=-c -O2 -Weffc++ -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -std=c++11

all: unit_tests
	

unit_tests: calculator.o unit_tests.o
	$(CC) unit_tests.o calculator.o -o unit_tests

unit_tests.o:
	$(CC) $(CFLAGS) unit_tests.cpp

calculator.o:
	$(CC) $(CFLAGS) calculator.cpp
	
clean:
	rm -rf *.o unit_tests
