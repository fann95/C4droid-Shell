PREFIX   := /data/data/com.n0n3m4.droidc
INCLUDES := -I$(PWD)/include
LIBS     :=-L$(PWD)/lib -lreadline -lhistory -lncurses

all: c4dsh install install-perl install-autotools install-flex install-texinfo install-help2man install-bison install-ssl install-ssh2 install-curl install-git install-sqlite3

	
c4dsh: c4dsh.o
	$(CC) c4dsh.o -o c4dsh $(LIBS)

c4dsh.o: c4dsh.c
	$(CC) $(INCLUDES) -c c4dsh.c -o c4dsh.o    


install: c4dsh
	install -m 0777 c4dsh $(PREFIX)/files
	install -m 0777 .C4dENV  $(PREFIX)/files
	if ! [ -d "/data/data/com.n0n3m4.droidc/home" ]; then mkdir -m 0777 /data/data/com.n0n3m4.droidc/home;fi;

install-perl:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/perl" ]; then cd $(PREFIX) && tar -mzxf $(PWD)/App/Perl5/usr.tar.gz;fi;
	if ! [ -d "/mnt/sdcard/C4droid_EXT/lib/perl" ]; then cd /mnt/sdcard && tar -mzxf $(PWD)/App/Perl5/C4droid_EXT.tar.gz;fi;

install-autotools:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/automake" ]; then cd $(PREFIX) && tar -mzxf $(PWD)/App/Autotools/usr.tar.gz;fi;

install-flex:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/flex" ]; then cd $(PREFIX) && tar -mzxf $(PWD)/App/Flex/usr.tar.gz;fi;

install-texinfo:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/makeinfo" ]; then cd $(PREFIX) && tar -mzxf $(PWD)/App/Texinfo5.2/usr.tar.gz;fi;
	if ! [ -d "/mnt/sdcard/C4droid_EXT/share/texinfo" ]; then cd /mnt/sdcard && tar -mzxf $(PWD)/App/Texinfo5.2/C4droid_EXT.tar.gz;fi;


install-bison:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/bison" ]; then cd $(PREFIX) && tar -mzxf $(PWD)/App/bison3.0.2/usr.tar.gz;fi

install-help2man:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/help2man" ]; then cd $(PREFIX) && tar -mzxf $(PWD)/App/help2man1.43.3/usr.tar.gz;fi

install-ssl:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/openssl" ]; then cd $(PREFIX) && tar -zxf $(PWD)/App/openssl/usr.tar.gz;fi;
	if ! [ -d "/mnt/sdcard/C4droid_EXT/include/openssl" ]; then cd /mnt/sdcard && tar -zxf $(PWD)/App/openssl/C4droid_EXT.tar.gz;fi;


install-ssh2: install-ssl
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/lib/libssh2.so.1" ]; then cd $(PREFIX) && tar -zxf $(PWD)/App/ssh2/usr.tar.gz;fi;
	if ! [ -f "/mnt/sdcard/C4droid_EXT/lib/libssh2.so" ]; then cd /mnt/sdcard && tar -zxf $(PWD)/App/ssh2/C4droid_EXT.tar.gz;fi;

install-curl: install-ssl install-ssh2
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/curl" ]; then cd $(PREFIX) && tar -zxf $(PWD)/App/Curl/usr.tar.gz;fi;
	if ! [ -f "/mnt/sdcard/C4droid_EXT/lib/libcurl.so" ]; then cd /mnt/sdcard && tar -zxf $(PWD)/App/Curl/C4droid_EXT.tar.gz;fi;

install-git: install-ssl install-ssh2 install-curl
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/git" ]; then cd $(PREFIX) && tar -zxf $(PWD)/App/Git-2.0/usr.tar.gz;fi;
	if ! [ -d "/mnt/sdcard/C4droid_EXT/lib/perl/5.16.0/Git" ]; then cd /mnt/sdcard && tar -zxf $(PWD)/App/Git-2.0/C4droid_EXT.tar.gz;fi;

install-sqlite3:
	if ! [ -f "/data/data/com.n0n3m4.droidc/usr/bin/sqlite3" ]; then cd $(PREFIX) && tar -zxf $(PWD)/App/sqlite3/usr.tar.gz;fi;
	if ! [ -f "/mnt/sdcard/C4droid_EXT/lib/libsqlite3.so" ]; then cd /mnt/sdcard && tar -zxf $(PWD)/App/sqlite3/C4droid_EXT.tar.gz;fi;