# Linux Makefile
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c 
LDFLAGS := 
DEPS := Socket.h
TARGETS := ForkMirrorServer.out MirrorClientTest.out ThreadMirrorServer.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

%.out: %.o VSocket.o Socket.o
	$(info $$^ = $^)
	$(CXX) $^ Socket.o $(LDFLAGS) -o $^

ForkMirrorServer.out:	ForkMirrorServer.o VSocket.o Socket.o 
	$(CXX) ForkMirrorServer.o VSocket.o Socket.o $(LDFLAGS) -o $@
	
ThreadMirrorServer.out:	ThreadMirrorServer.o VSocket.o Socket.o 
	$(CXX) ThreadMirrorServer.o VSocket.o Socket.o $(LDFLAGS) -o $@
	
MirrorClientTest.out:	MirrorClientTest.o VSocket.o Socket.o 
	$(CXX) MirrorClientTest.o VSocket.o Socket.o $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out
