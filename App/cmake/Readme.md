Important
-------------- 
*While compiling, your current directory must be in the internal memory and not on sdcard. 
**The Wrong Way:**
```sh
$ cd /sdcard/project
$ cmake src
```
**Correct:**('pwd'=/data/data/com.n0n3m4.droidc/home)
```sh
$ mkdir mybuild
$ cd mybuild
$ cmake /sdcard/project/src
```