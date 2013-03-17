Mikey
=====

Description
-----------
MiKey is a minimal and modular Linux keylogger. It provides a toolkit that monitors and records all activities performed on a computer. It makes use of the Xlib.

Installation
-----------
    make

    ./mikey

Usage
-----
* -b, --background    Runs the keylogger at the backround.
* -o, --output        Declares the path of the output file. Used by plain-text module.

Building a Module
-------
Modules receive the recorded data and handle it accordingy. 

To build a module, you need to define two funtions:

1. **void handleArgs(int argc, char \*argv[])** - This function handles the given arguments by the user. It is executed by the core only once at the very beginning so any initialization (for example, connection to a server) also needs to take place here.

1. **void getFeed(char \*b)** - This function receives a copy of a buffer that holds the recent pressed keystrokes. This buffer tries to hold consistent data and the core will feed the plugins with a new one by triggering this function when nothing is pressed within 5 seconds or user changes the window.

For a skeleton, take a look at plaintext module. All it does is logging the received data to the specified file.

Modules
-------

### Plain Text
Plain Text is logging the received data to the specified file.

**Usage** 
* -o, --output    Declares the path of the output file.

### Raw Tcp
Raw Tcp is sending the received data into the specified server over TCP. You will need a TCP listener to receive data. Listener server must run before the module attempt any connection. You can use:

    netcat -l -p 13050 

**Usage**
* --host ADDRESS    Specify listener address. Required.
* --port PORT       Specify listener port. Default: 13050.
* --no-dns          Do not do DNS lookup.

Licence
-------
Mikey is released under the [Modified BSD Licence](http://opensource.org/licenses/bsd-license.html "Modified BSD License").

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
