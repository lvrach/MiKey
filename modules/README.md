Mikey Modules
=============

Plain Text
-----------
Plain Text is logging the received data to the specified file.

**Usage** 
* -o, --output        Declares the path of the output file. Used by plain-text module.

Raw Tcp
-----------
Raw Tcp is sending the received data into specified server over tcp.

**Usage**
* --host ADDRESS 	Specify listener address, required.
* --port PORT		Specify listener port, default port 13050.
* --no-dns          Do not use DNS lookup.

**Defaults**
* Server port: 13050

**Notes**

You will need a tcp listener to receive data. We don't provide one yet. You can use:

* netcat: netcat -l -p 13050 

Listener server must be run before the module attempt connection.Otherwise the connection would be established, but you would loss data.


