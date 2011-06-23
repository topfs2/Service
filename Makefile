.SUFFIXES: .cpp

all: test

test: test.cpp Variant.cpp Variant.h IServiceBase.h
	$(CXX) $(CFLAGS) Variant.cpp test.cpp -o test

clean:
	$(RM) test

really-clean:
	git clean -xfd
