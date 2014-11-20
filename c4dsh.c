/*bassed on example of the libreadline*/
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>
#define MAXCONFNAMELEN 20
#define MAXCONFVALUELEN 1024
#define MAXCONFSIZE 40

struct myconfig {
	char *name[MAXCONFSIZE];
	char *value[MAXCONFSIZE];
};


/* Forward declarations. */
char *stripwhite();
char *dupstr(char *s);
static char *rstrip(char *s);
static char *lskip(const char *s);
static char* find_char(const char* s, char c);
int update_config(struct myconfig *config, int *configsize);
char *promt_line();
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
char *replace(char *instring,char *old,char *new);
static char cur4dir[MAXPATHLEN];

/* When non-zero, this global means the user is done using this program. */
int done;


char *promt_line()
{
	static char buf[MAXPATHLEN];
	static char tom[15];
	char *p, *prompt;
	prompt = (getuid()? " $ " : " # ");
	char *path = getcwd(cur4dir, MAXPATHLEN);	
	p = strstr(path, getenv("HOME"));
	if (p) {
		sprintf(buf, "\001\e[1;33m\002~%s%s\001\e[00m\002", p + strlen(getenv("HOME")), prompt);
	buf[sizeof(buf) - 1] = '\0';
	return buf;
	}	
	
	int n=0;
	while(1)
	{
		if(!(p=strchr(path+1,'/')))
		{
			break;
		}
		if(n==0)
		{
			strncpy(tom,path,(p-path));
		}
		path=p;
		n++;
	}
	if(n<=1){
		sprintf(buf, "\001\e[1;33m\002%s%s%s\001\e[00m\002", tom,path, prompt);
	}else{
		char dot[(n-1)*4];
		memset(dot,'.',sizeof(dot));	
		int i;
		for(i=0;i<strlen(dot);i++)
		{
			dot[i]=((i&3) ? '.' : '/');
		}
		dot[sizeof(dot) - 1] = '\0';
		sprintf(buf, "\001\e[1;33m\002%s%s%s%s\001\e[00m\002", tom,dot,path, prompt);
	}
	buf[sizeof(buf) - 1] = '\0';
	return buf;
	
}

void sigint_init()
{
	struct sigaction act;
	memset(&act,0,sizeof(struct sigaction));
	act.sa_handler=SIG_IGN;
	sigaction (SIGINT,&act,0);
	return;
}

main(int argc, char **argv)
{
	
	if (access("/data/data/com.n0n3m4.droidc/files/gcc", 0)) {
		printf("Please install gcc in internal memory\n");
		exit(1);
	}

	int error=0;
	struct myconfig config;
	int configsize;
	configsize = 0;
	if (access("/data/data/com.n0n3m4.droidc/home", 0)) {
		if(getuid()==0){
		    printf("Please uncheck 'Run programs as root' in C4droid preferences for first start\n");
		    exit(0);
	    }
		mkdir("/data/data/com.n0n3m4.droidc/home", 0777);
	}
	
	if (access("/data/data/com.n0n3m4.droidc/tmp",0)) {
		if(getuid()==0){
		    printf("Please uncheck 'Run programs as root' in C4droid preferences for first start\n");
		    exit(0);
	    }
		mkdir("/data/data/com.n0n3m4.droidc/tmp",0777);
		chmod("/data/data/com.n0n3m4.droidc/tmp", S_IRWXU | S_IRWXG | S_IRWXO | S_ISVTX);
	}
	struct stat sb;
	if (stat("/data/data/com.n0n3m4.droidc/files/.C4dENV", &sb)== -1  || sb.st_size==0) {
		if(getuid()==0){
		    printf("Please uncheck 'Run programs as root' in C4droid preferences for first start\n");
		    exit(0);
	    }
	    
		config.name[0]= "PATH";
		config.value[0] = "/busybox-virtual:/data/data/com.n0n3m4.droidc/files/:/data/data/com.n0n3m4.droidc/files/gcc/bin:/data/data/com.n0n3m4.droidc/files/gcc/qt/bin:/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/bin:/data/data/com.n0n3m4.droidc/usr/bin:/sbin:/system/bin:/system/xbin:/data/local/bin";
		config.name[1] = "SHELL";
		config.value[1] = "/data/data/com.n0n3m4.droidc/files/busybox sh";
		config.name[2] = "CONFIG_SHELL";
		config.value[2] = "sh";
		//-Wl,rpath-link=/data/data/com.n0n3m4.droidc/usr/lib
		config.name[3] = "CFLAGS";
		config.value[3] = "-Os -s  -DANDROID -I/sdcard/C4droid_EXT/include -I/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/include/ncurses -I/data/data/com.n0n3m4.droidc/usr/include -L/data/data/com.n0n3m4.droidc/usr/lib -L/sdcard/C4droid_EXT/lib";
		config.name[4] = "CXXFLAGS";
		config.value[4] = "-Os -s  -DANDROID -I/sdcard/C4droid_EXT/include -I/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/include/ncurses -I/data/data/com.n0n3m4.droidc/usr/include -L/data/data/com.n0n3m4.droidc/usr/lib -L/sdcard/C4droid_EXT/lib";
		config.name[5] = "LDFLAGS";
		config.value[5] = "-lm -ldl -llog -lz -lncurses -L/sdcard/C4droid_EXT/lib -L/data/data/com.n0n3m4.droidc/usr/lib -Wl,-allow-shlib-undefined";
		config.name[6]= "PKG_CONFIG_PATH";
		config.value[6] = "/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/lib/pkgconfig:/sdcard/C4droid_EXT/lib/pkgconfig;/data/data/com.n0n3m4.droidc/usr/lib/pkgconfig";
		config.name[7] = "CC";
		config.value[7] = "/data/data/com.n0n3m4.droidc/files/gcc/bin/arm-linux-androideabi-gcc";
		config.name[8] = "CXX";
		config.value[8] = "/data/data/com.n0n3m4.droidc/files/gcc/bin/arm-linux-androideabi-g++";
		config.name[9] = "LD_LIBRARY_PATH";
		config.value[9] = "/data/data/com.n0n3m4.droidc/usr/lib:/system/lib:vendor/lib";
		config.name[10] = "PREFIX";
		config.value[10] = "/data/data/com.n0n3m4.droidc/usr";		
		config.name[11] = "TEMP";
		config.value[11] = "/data/data/com.n0n3m4.droidc/tmp";
		config.name[12] = "TMP";
		config.value[12] = "/data/data/com.n0n3m4.droidc/tmp";
		config.name[13] = "TMPDIR";
		config.value[13] = "/data/data/com.n0n3m4.droidc/tmp";
		config.name[14] = "CONFIGFIX";
		config.value[14] = "ac_cv_host=arm-unknown-linux-androideabi ac_cv_host_alias=arm-linux-androideabi ac_cv_func_malloc_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes ac_cv_func_ttyname=no";
		config.name[15] = "LC_ALL";
		config.value[15] = "en_EN.UTF-8";
		
		config.name[16]="PERL";
		config.value[16]="/data/data/com.n0n3m4.droidc/usr/bin/perl";
		
		config.name[17] = "GIT_SSL_CAINFO";
		config.value[17] = "/data/data/com.n0n3m4.droidc/usr/lib/ssl/certs/ca-bundle-cert.pem";
		config.name[18] = "GIT_CURL_VERBOSE";
		config.value[18] = "0";
		config.name[19] = "CURL_CA_BUNDLE";
		config.value[19] = "/data/data/com.n0n3m4.droidc/usr/lib/ssl/certs/ca-bundle-cert.pem";
		config.name[20] = "SSL_CERT_FILE";
		config.value[20] = "/data/data/com.n0n3m4.droidc/usr/lib/ssl/certs/ca-bundle-cert.pem";
		config.name[21] = "SSL_CERT_DIR";
		config.value[21] = "/data/data/com.n0n3m4.droidc/usr/lib/ssl/certs";
		config.name[22] = "CMAKE_ROOT";
		config.value[22] = "/mnt/sdcard/C4droid_EXT";
		config.name[23] = "SHELL-G";
		config.value[23] = "/data/data/com.n0n3m4.droidc/files/busybox sh";
		configsize = 24;
		
	}
	
	error=update_config(&config, &configsize);
	if(error!=0) {
		done =1;
	}
	
	setenv("HOME", "/data/data/com.n0n3m4.droidc/home", 1);
	

	if (chdir("/data/data/com.n0n3m4.droidc/home") == -1) {
		perror("/data/data/com.n0n3m4.droidc/home");
	}
	char *line, *s;
	
	
	sigint_init();

	if(read_history(NULL)!=0){
		write_history(NULL);
	}
	
	error=1;

	for (; done == 0;) {

		line = readline(promt_line());
		if (!line){
			printf("\n");
			continue;
		}		
		s = stripwhite(line);
		if (*s) {
			if(error==0){
				add_history(s);
				append_history(1,NULL);
			}else{error=0;}
			execute_line(s);
		}
		
		free(line);
	}
	exit(0);
}

/* Execute a command line. */
int execute_line(line)
char *line;
{
	register int i;	
	char *word;
	char *wordplus;
	line=replace(line,"~",getenv("HOME"));
	if(!line){return 0;}
	char *origline=dupstr(line);
	static char syscom[1024];
	
	i = 0;
	while (line[i] && whitespace(line[i]))
		i++;
	word = line + i;

	while (line[i] && !whitespace(line[i]))
		i++;

	if (line[i]){
		line[i++] = '\0';
	}
	
	while (whitespace(line[i])){
		i++;
	}
	wordplus = line + i;
	
	
	if(!wordplus || !*wordplus){wordplus="";}
	
	if (*word) {
		char *command;
		command=(char *)basename(word);		
		/*configure*/
		if(strcmp(command,"configure")==0){
			if((strcmp(wordplus,"--help")!=0 && strcmp(wordplus,"-h")!=0 ) && (strcmp(wordplus,"-help")!=0 && strcmp(wordplus,"?")!=0)){
			    sprintf(syscom,"%s %s --host=arm-linux-androideabi --build=i686-linux --disable-nls --prefix=%s CFLAGS='%s' CXXFLAGS='%s' LDFLAGS='%s' %s %s CONFIG_SHELL=sh",getenv("SHELL"),word,getenv("PREFIX"),getenv("CFLAGS"),getenv("CXXFLAGS"),getenv("LDFLAGS"),getenv("CONFIGFIX"),wordplus);
				printf("\001\e[1;33m\002 %s\001\e[00m\002\n",syscom);
			}else{
				sprintf(syscom,"%s %s --help",getenv("SHELL"),word);
			}
                    system(syscom);
                    return 0;
		/*make*/
		}else if(strcmp(command,"make")==0){
			sprintf(syscom,"%s %s CC='%s' CXX='%s' SHELL='%s'",word,wordplus,getenv("CC"),getenv("CXX"),getenv("SHELL"));
			printf("\001\e[1;33m\002 %s\001\e[00m\002\n",syscom);
                        system(syscom);
                        return 0;	
		/*cd*/
		}else if(strcmp(command,"cd")==0){
			
		    if (wordplus==""){
		        if(chdir(getenv("HOME"))==-1){
			    perror(getenv("HOME"));
			    return 1;
		        }
	            }else if (chdir(wordplus) == -1) {
		        perror(wordplus);
		        return 1;
	            }
	            return 0;	        
		/*ls*/
		}else if(strcmp(command,"ls")==0){
			sprintf(syscom, "busybox ls %s", wordplus);
                        system(syscom);
                        return 0;
		/*su*/
		}else if(strcmp(command,"su")==0){
			printf("Su off/on in C4droid menu->settings->Preferences->Run programs as root checkbox\n");
			return 0;
		/*quit*/
		}else if(strcmp(command,"quit")==0 || strcmp(command,"exit")==0){
			done=1;
			return 0;
		/*not special command*/
		}else{
		    struct stat sb;
			static char buf[MAXPATHLEN];
			sprintf(buf,"%s/%s",cur4dir,command);
			/* search full path */
			if(stat(buf,&sb)==-1 || S_ISDIR(sb.st_mode)){
				if(stat(word,&sb)!=-1 && !S_ISDIR(sb.st_mode))
				{
					sprintf(buf,"%s",word);
				}else{
					char *start,*end,*line;
					line=dupstr(getenv("PATH"));
					start=line;			    
			        while(*start)
			        {
					    end=find_char(start,':');
					    if (*end == ':'){*end = '\0';}
					    sprintf(buf,"%s/%s",start,command);
					    if (stat(buf, &sb)!= -1 && !S_ISDIR(sb.st_mode)){break;}
					    sprintf(buf,"");
					    start = lskip(end + 1);
			        }
			        free(line);
			    }
		    }
			/*if path to file*/
			if(strlen(buf)>4){				
				FILE *file = fopen(buf, "r");
				if (!file) {
					printf("ERROR:open file %s\n",buf);
					return;
				}
				char *sc=(char*)malloc(4);
				if(!sc)
				{
					printf("ERROR:allocate memmory for sc\n");
					fclose(file);
					return;
				}
				/*if script*/
				if(fgets(sc, 3, file) && strstr(sc,"#!")){					
					char * scrbuf = NULL;
					size_t scrbuf_size = 0;
					getline( & scrbuf , & scrbuf_size , file);
					/*if /bin/sh */
					if(strstr((const char *)basename(scrbuf),"sh") && scrbuf_size<15){
						sprintf(syscom,"%s %s %s",getenv("SHELL"),buf,wordplus);
						fclose(file);
						free(sc);
						free(scrbuf);
						system(syscom);
						return 0;
					}
					free(scrbuf);
				}
				/*not sh script*/				
				sprintf(syscom,"%s %s",buf,wordplus);
				fclose(file);
				free(sc);
				system(syscom);
				return 0;
			}
			/*command,not file*/
			sprintf(syscom,"%s",origline);
			system(syscom);
			return 0;
		}
		return 0;
	}
	
	return 0;
}

char *dupstr(s)
char *s;
{
	char *r;

	r =(char*) xmalloc(strlen(s) + 1);
	strcpy(r, s);
	return (r);
}

static char* find_char(const char* s, char c)
{
	
	while (*s && *s != c) {
		s++;
	}
	return (char*)s;
}
static char *rstrip(char *s)
{
	char *p = s + strlen(s);
	while (p > s && isspace((unsigned char)(*--p)))
		*p = '\0';
	return s;
}

static char *lskip(const char *s)
{
	while (*s && isspace((unsigned char)(*s)))
		s++;
	return (char *)s;
}



int update_config(struct myconfig *config, int *configsize)
{

	FILE *file;
	int mode=0;
	int i;
	int count=*configsize;
	int error=0;
	if(*configsize) {
		file = fopen("/data/data/com.n0n3m4.droidc/files/.C4dENV", "w+");
	} else {
		file = fopen("/data/data/com.n0n3m4.droidc/files/.C4dENV", "r");
	}
	if (!file) {
		printf("ERROR:open file '/data/data/com.n0n3m4.droidc/files/.C4dENV'\n");
		return -1;
	}
	if(*configsize==0) {
		char* start;
	    char* end;
	    char *line;
	    int maxlenline=MAXCONFVALUELEN+MAXCONFNAMELEN+5;
	    line = (char*)malloc(maxlenline);
	    if(!line)
		{
			printf("ERROR:allocate memmory for line\n");
			fclose(file);
			return -2;
		}
		while (fgets(line, maxlenline, file) != NULL) 
		{
			
		    start = line;
		    if (count == 0 && (unsigned char)start[0] == 0xEF &&
		        (unsigned char)start[1] == 0xBB &&
		        (unsigned char)start[2] == 0xBF) {
			    start += 3;
		    }
		    start = lskip(rstrip(start));
		    if (*start) {
			    end = find_char(start, '=');
			    if (*end == '=') {
				    *end = '\0';
				    config->name[count] = dupstr(rstrip(start));
				    start = lskip(end + 1);
				    if(*start='[') {
					    end = find_char(start, ']');
					    if (*end == ']') {
						    *end = '\0';
							start = lskip(start + 1);
						    config->value[count]=dupstr(rstrip(start));
						    //printf("%s\n",rstrip(start));
						    
					    } else {
							printf("ERROR:']' not found in line: %i (config broken)\n",count);
							error=1;
						    break;
					    }
				    } else {
						printf("ERROR:'[' not found in line: %i (config broken)\n",count);
						error=1;
					    break;
				    }

			    } else {
					printf("ERROR:'=' not found in line: %i (config broken)\n",count);
					error=1;
				    break;
			    }

		    } else {
				printf("ERROR:config file is empty or broken\n");
				error=1;
			    break;
		    }		    
		    count++;

	    }
		if(error){
			fclose(file);
			free(line);
			return -3;
		}
		*configsize=count;		
		if(*configsize==0) {
			fclose(file);
			free(line);
			printf("ERROR:parse config emty or broken\n");
			return -3;
		}
		free(line);
		mode=1;
	}
	
	for (i = 0; i < count; i++) {
		if(!mode) {
			if(!(fprintf(file, "%s = [%s]\n", config->name[i], config->value[i]))) {
				printf("ERROR:write %i line in config\n",i);
				error=-4;
			}
		}
		
		if(setenv(config->name[i],config->value[i],1)!=0){
			printf("ERROR:set environment from line %i \n",i);
			error=-5;
		}
    }
    fclose(file);
	return error;
}

char *stripwhite(string)
char *string;
{
	register char *s, *t;

	for (s = string; whitespace(*s); s++)
		;

	if (*s == 0)
		return (s);

	t = s + strlen(s) - 1;
	while (t > s && whitespace(*t))
		t--;
	*++t = '\0';

	return s;
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    char *ptr;
    ptr = fgetln(stream, n);
    if (ptr == NULL) {
        return -1;
    }
    /* Free the original ptr */
    if (*lineptr != NULL) free(*lineptr);
    /* Add one more space for '\0' */
    size_t len = n[0] + 1;
    /* Update the length */
    n[0] = len;
    /* Allocate a new buffer */
    *lineptr = malloc(len);
    /* Copy over the string */
    memcpy(*lineptr, ptr, len-1);
    /* Write the NULL character */
    (*lineptr)[len-1] = '\0';
    /* Return the length of the new buffer */
    return len;
}

char *replace(char *instring,char *old,char *new)
{
	size_t instring_size=strlen(instring);
	size_t new_size=strlen(new);
	size_t old_size=strlen(old);
	
	if(instring_size<old_size)
	{
		printf("ERROR:argument 2 is too much for replace()\n");
	    return (char*)NULL;
	}
	char *test=(char*)malloc(instring_size+1);
	char *outstring=(char*)malloc(instring_size+new_size-old_size+1);
		
	if(!outstring || !test){
		printf("ERROR:allocate memmory for replace()\n");
	    return (char*)NULL;
	}
	outstring[0]='\0';
	int i;
	for(i=0; i <= instring_size; i++)
	{		
		strcpy(test,(instring+i));
		test[old_size]='\0';
		if(*test==*old){
			strcat(outstring,new);
			i=i+old_size-1;
		}else{
			test[1]='\0';
			strcat(outstring,test);
		}
	}
	return outstring;
}