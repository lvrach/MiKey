P=mikey
CC=gcc
LD=ld

SRC_DIR=src
INC_DIR=include
MOD_DIR=modules
BUILD_DIR=build

CFLAGS=-Wall -ggdb
LDFLAGS= -lX11 -ldl -Wl,-rpath,./$(MOD_DIR) 

MODS=mikey-plaintext.so


$(P): buffer.o main.o mikey-plaintext.so
	$(CC) $(LDFLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/$(INC_DIR)/buffer.o -o $@

main.o : $(SRC_DIR)/main.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(LDFLGAS) -o $(BUILD_DIR)/$@  $^ 

buffer.o: $(INC_DIR)/buffer.c
	mkdir -p $(BUILD_DIR)/$(INC_DIR)
	$(CC) -c $(CFLAGS) $(LDFLGAS) -o $(BUILD_DIR)/$(INC_DIR)/$@  $^ 

mikey-plaintext.so: $(MOD_DIR)/mikey-plaintext.c
	$(CC) -shared -fPIC -rdynamic $^ -o $(MOD_DIR)/$@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
	rm $(MOD_DIR)/*.so

