
CC=gcc
CFLAGS=-Wall -ggdb
LDFLAGS=-shared -fPIC -rdynamic 

all: mikey-plaintext.so mikey-rawtcp.so

mikey-plaintext.so: mikey-plaintext.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


mikey-rawtcp.so: mikey-rawtcp.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@  


.PHONY: plaintext
plaintext: mikey-plaintext.so

.PHONY: rawtcp
rawtcp: mikey-rawtcp.so

.PHONY: clean
clean:
	rm *.so
