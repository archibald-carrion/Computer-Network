# Linux Makefile
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c 
LDFLAGS := -lssl -lcrypto
DEPS := Socket.h
TARGETS := SSLServer.out SSLClient.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

%.out: %.o VSocket.o Socket.o
	$(info $$^ = $^)
	$(CXX) $^ Socket.o $(LDFLAGS) -o $^

SSLServer.out:	SSLServer.o VSocket.o Socket.o
	$(CXX) SSLServer.o VSocket.o Socket.o $(LDFLAGS) -o $@

SSLClient.out:	SSLClient.o VSocket.o Socket.o 
	$(CXX) SSLClient.o VSocket.o Socket.o $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out
