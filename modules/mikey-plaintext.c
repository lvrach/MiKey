#include <stdio.h>
#include <stdarg.h>
#include <time.h>

char *outputfile = "out.log";

void log(const char * fmt, ...) {

    va_list va_alist;
    char buf[2048], logbuf[2048];
    FILE* file;
    struct tm* current_tm;
    time_t current_time;
    time(&current_time);

    current_tm = localtime (&current_time);
    sprintf(logbuf, "[%02d:%02d:%02d] ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);

    va_start(va_alist, fmt);
    vsprintf(buf, fmt, va_alist);
    va_end(va_alist);

    strcat(logbuf, buf);
    strcat(logbuf, "\n");

    if ((file = fopen(outputfile, "a+")) != NULL) {
        fputs(logbuf, file);
        fclose(file);
    }
}


void getFeed(char *b) {
    log(" %s\n", b);
}

void handleArgs(int argc, char* argv[]) {

    int i;

    for (i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-b") == 0) || (strcmp(argv[i], "--background") == 0)) {
            printf("aaaaaa");
        }

        else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output") == 0)) {
            //output = 1;
            if (i + 1 <= argc - 1)  {
                i++;
                outputfile = argv[i];
            }
            else {
                printf("Not good.");
            }
        }
    }
}
