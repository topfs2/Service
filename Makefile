.SUFFIXES: .cpp

all: test

test: test.cpp Variant.cpp Variant.h Time.cpp Time.h Locks.cpp Locks.h Thread.cpp Thread.h IMainloop.h BaseMainloop.h BaseMainloop.cpp ProcessMainloop.h ProcessMainloop.cpp ThreadedMainloop.h ThreadedMainloop.cpp PowerService.cpp PowerService.h ServiceBase.cpp ServiceBase.h
	$(CXX) $(CFLAGS) -o test Variant.cpp Time.cpp Locks.cpp Thread.cpp BaseMainloop.cpp ProcessMainloop.cpp ThreadedMainloop.cpp ServiceBase.cpp PowerService.cpp test.cpp -D_REENTRANT -pthread -lboost_thread -lrt

clean:
	$(RM) test

really-clean:
	git clean -xfd
