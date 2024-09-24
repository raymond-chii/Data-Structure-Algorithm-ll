CXX = g++
CXXFLAGS = -std=c++11

spell.exe: spellcheck.o hash.o
	$(CXX) $(CXXFLAGS) -o spell.exe spellcheck.o hash.o

spellcheck.o: spellcheck.cpp hash.h
	$(CXX) $(CXXFLAGS) -c spellcheck.cpp

hash.o: hash.cpp hash.h
	$(CXX) $(CXXFLAGS) -c hash.cpp

debug: CXXFLAGS += -g
debug: spell.exe

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups