LDFLAGS = -L/opt/local/lib -lgcrypt
CXXFLAGS = -std=c++11 -Wall -Wextra -g

BINS = keygen main decrypt encrypt
OBJS = gcry.o

all: $(BINS)

keygen: gcry.o
	$(CXX) $(CXXFLAGS) keygen.cc $(OBJS) -o keygen $(LDFLAGS)

main: gcry.o
	$(CXX) $(CXXFLAGS) main.cc $(OBJS) -o main $(LDFLAGS)

decrypt: gcry.o
	$(CXX) $(CXXFLAGS) decrypt.cc $(OBJS) -o decrypt $(LDFLAGS)

encrypt: gcry.o
	$(CXX) $(CXXFLAGS) encrypt.cc $(OBJS) -o encrypt $(LDFLAGS) 

gcry.o: gcry.cc

clean:
	rm -f *.o $(BINS)
