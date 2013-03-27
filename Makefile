.SUFFIXES: .cpp

all: test

test: test.cpp Variant.cpp Variant.h PowerService.cpp PowerService.h ServiceBase.cpp ServiceBase.h
	$(CXX) $(CFLAGS) Variant.cpp ServiceBase.cpp PowerService.cpp test.cpp -o test

clean:
	$(RM) test

really-clean:
	git clean -xfd
