.SUFFIXES: .cpp

all: test

test: test.cpp Variant.cpp Variant.h Locks.cpp Locks.h Thread.cpp Thread.h IMainloop.h SimpleMainloop.h SimpleMainloop.cpp PowerService.cpp PowerService.h ServiceBase.cpp ServiceBase.h
	$(CXX) $(CFLAGS) -o test Variant.cpp Locks.cpp Thread.cpp SimpleMainloop.cpp ServiceBase.cpp PowerService.cpp test.cpp -D_REENTRANT -pthread -lboost_thread

clean:
	$(RM) test

really-clean:
	git clean -xfd
