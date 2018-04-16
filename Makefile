CXX = g++
CXXFLAGS = -Wall -O2 --std=c++17
PREFIX = /usr/local/bin
obj =  main.o ipv4subnetting.o 

all: subcal

subcal: $(obj)
	$(CXX) $(CXXFLAGS) ipv4subnetting.o main.o -o subcal

ipv4subnetting.o: ipv4subnetting.cpp ipv4subnetting.hpp

main.o: main.cpp ipv4subnetting.hpp

install: subcal
	mv subcal $(PREFIX)/subcal

clean:
	rm -f subcal $(obj)
