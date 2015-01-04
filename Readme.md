Shell and tools for C4droid
----------------------------
[C4droid] *is a IDE + C/C++ compiler for Android.*

**Now available(only for devices with ARM cpu):**
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
```
**~40 Mb in internal memory and ~45 Mb on sdcard for all tools**

Installation
--------------
requires [c4droid](not a free app.,but well that not expensive) and
[GCC plugin] installed in the internal memory.
1.  Open Makefile in C4droid and compile.
2.  Add **(c4droid:DATADIR)c4dsh**  in *Menu->Preferences->Shell path*
3.  Remove all in *Menu->Preferences->Autorun command* and add **clear**
4.  Check *Menu->Enable Run Terminal button*

*Now you can run the terminal from C4droid menu and use it*
*to compile native Linux software for Android.*

*P.S. All the libraries and utilities in this project were compiled by*
*me from official sources.*

[C4droid]:https://play.google.com/store/apps/details?id=com.n0n3m4.droidc
[GCC plugin]:https://play.google.com/store/apps/details?id=com.n0n3m4.gcc4droid