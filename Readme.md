Shell and tools for C4droid
----------------------------
[C4droid] *is a IDE + C/C++ compiler for Android.*

**Now available (only for devices with ARM cpu and Android < 5):**
```sh
* c4dsh
* perl-5.20.1
* autoconf-2.69
* automake-1.14.1
* curl-7.37.1
* git-2.2.0
* cmake-3.0.1
* gdb-7.7.1
* flex-2.5.39
* texinfo-5.2
* bison-3.0.2
* help2man-1.43.3
* openssl-1.0.1c
* openssh-6.7p1
* sqlite3-3.8.5
* libssh2-1.4.3
* gdbm-1.11
* libiconv-1.14
* libreadline-6.3
* libexpat-2.1.0
* libssh-0.7.0
* libxml2-2.7.8
* libpcap-2.4
* libpcre-8.33
* libzlib-1.2.3
```
**~40 Mb in internal memory and ~45 Mb on sdcard for all tools**

**Requires:**
----------------
1. *[C4droid] not a free app.,but well that not expensive*
2. *[GCC plugin] installed in the internal memory*

Installation
--------------

1.  Open Makefile in C4droid and compile.
2.  Add **(c4droid:DATADIR)c4dsh**  in *Menu->Preferences->Shell path*
3.  Remove all in *Menu->Preferences->Autorun command* and add **clear**
4.  Check *Menu->Enable Run Terminal button*

*Now you can run the terminal from C4droid menu and use it*
*to compile native Linux software for Android.*

Examples
---------
*All environment variables are already set.*

**example with configure:**
```sh
curl ftp://ftp.gnu.org/gnu/sed/sed-4.2.tar.gz > sed-4.2.tar.gz
tar -xvzf sed-4.2.tar.gz
cd sed-4.2
./configure
make
make install DESTDIR=~/OUT
```
**example with curl:**
```sh
`curl-config --cc --cflags` ${CFLAGS} -o example example.c `curl-config --libs` ${LDFLAGS}
```
**simple example:**
```sh
$CC $CFLAGS -o example example.c ${LDFLAGS}
```
**example with additional libraries**
```sh
$CC $CFLAGS -I/path_to_additional_headers -o example example.c ${LDFLAGS} -L/path_to_additional_libraries -ladditional_lib_1 -ladditional_lib_2 ...etc.
```
*P.S. All the libraries and utilities in this project were compiled by*
*me from official sources.*

[C4droid]:https://play.google.com/store/apps/details?id=com.n0n3m4.droidc
[GCC plugin]:https://play.google.com/store/apps/details?id=com.n0n3m4.gcc4droid
