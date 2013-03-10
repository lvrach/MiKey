typedef struct buf {
    int size;
    char *buffer;
    int occupied;
} BUFFER;

BUFFER *initBuffer();
int addData(BUFFER *b, char *data);
int emptyData(BUFFER *b);
