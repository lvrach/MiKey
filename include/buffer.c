#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
 
#define SIZE 1024
 
typedef struct buf {
    int size;
    char *buffer;
    int occupied;
} BUFFER;

BUFFER *initBuffer() { 

    BUFFER *b = (BUFFER *) malloc(sizeof(BUFFER));

    if (b != NULL) {
        b->size = SIZE;
        b->occupied = 1;
        b->buffer = (char *) malloc(sizeof(char) * SIZE);
        b->buffer[0] = '\0';
        return b;
    }
    fprintf(stderr, "Error allocating memory.\n");
    return 0;
    exit(1);
}
 
int addData(BUFFER *b, char *data) {

    b->occupied += strlen(data);
     
    if (b->occupied > b->size) {
        char *tmppointer = realloc(b->buffer, 2 * b->size);
        if (tmppointer == NULL) {
            fprintf(stderr, "Error allocating memory.\n");
            exit(1);
        }
        else {
            b->buffer = tmppointer;
            b->size *= 2;
        }
    }

    strcat(b->buffer, data);
    return 1;
}
 
 
int emptyData(BUFFER *b) {
 
    b->occupied = 1;
    b->buffer[0] = '\0';
 
    return 1;
}
 
/* 
void main() {
    BUFFER *b;
 
    b = initBuffer();    
    assert(b->occupied == 1);

    addData(b, "1");
    addData(b, "2");
    assert(b->occupied == 3);

    emptyData(b);
    assert(b->occupied == 1);

    addData(b, "123");
    assert(b->occupied == 4);

    char *str;
    str = "456";
    addData(b, str); 
    assert(b->occupied == 7);
}
*/
