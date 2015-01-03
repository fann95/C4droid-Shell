#include <sys/file.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>
#include <linux/binfmts.h>
#include <stdbool.h>
#define MAXCONFNAMELEN 25
#define MAXCONFVALUELEN 1024
#define MAXCONFSIZE 40

struct myconfig {
	char *name[MAXCONFSIZE];
	char *value[MAXCONFSIZE];
};

struct SHEBANG {
	char path2script[MAXPATHLEN];
	char shebang[BINPRM_BUF_SIZE];
	char opt[BINPRM_BUF_SIZE];
};

/* Forward declarations. */
char *stripwhite();
char *dupstr(char *s);
static char *rstrip(char *s);
static char *lskip(const char *s);
static char* find_char(const char* s, char c);
int update_config(struct myconfig *config, int *configsize);
char *promt_line();
int getShebang(char *execname,struct SHEBANG *pTRshebang);
char *replace(char *instring,char *old,char *new);
static char cur4dir[MAXPATHLEN];

/* When non-zero, this global means the user is done using this program. */
int done;


char *promt_line()
{
	static char buf[MAXPATHLEN];
	char *tom=(char*) xmalloc(15);
	char *p, *prompt;
	prompt = (getuid()? " $ " : " # ");
	char *path=replace(getcwd(cur4dir, MAXPATHLEN),getenv("HOME"),"~");
	int n=0;
	tom[0]='\0';
	while(1)
	{
		if(!(p=strchr(path+1,'/')))
		{
			break;
		}
		if(n==0)
		{
			if((p-path)>14){break;}
			strncpy(tom,path,(p-path));
			tom[(p-path)]='\0';
		}
		path=p;
		n++;
	}	
	if(n<=1){
		sprintf(buf, "\001\e[1;33m\002%s%s%s\001\e[00m\002", tom,path, prompt);
	}else{
		size_t dot_len=(n-1)*4-1;
		char dot[dot_len+1];
		memset(dot,'.',dot_len);
		dot[dot_len] = '\0';
		int i;
		for(i=0;i<dot_len;i++)
		{
			dot[i]=((i&3) ? '.' : '/');
		}		
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
		config.value[0] = "/data/data/com.n0n3m4.droidc/usr/sbin:/data/data/com.n0n3m4.droidc/usr/bin:/busybox-virtual:/data/data/com.n0n3m4.droidc/files/:/data/data/com.n0n3m4.droidc/files/gcc/bin:/data/data/com.n0n3m4.droidc/files/gcc/qt/bin:/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/bin:/data/data/com.n0n3m4.droidc/usr/bin/scripts:/sbin:/system/bin:/system/xbin:/data/local/bin";
		config.name[1] = "SHELL";
		config.value[1] = "/data/data/com.n0n3m4.droidc/files/busybox sh";
		config.name[2] = "CONFIG_SHELL";
		config.value[2] = "sh";
		config.name[3] = "CFLAGS";
		config.value[3] = "-Os -s  -DANDROID -I/sdcard/C4droid_EXT/include -I/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/include/ncurses -I/data/data/com.n0n3m4.droidc/usr/include -L/data/data/com.n0n3m4.droidc/usr/lib -L/sdcard/C4droid_EXT/lib";
		config.name[4] = "CXXFLAGS";
		config.value[4] = "-Os -s -DANDROID -I/sdcard/C4droid_EXT/include -I/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/include/ncurses -I/data/data/com.n0n3m4.droidc/usr/include -L/data/data/com.n0n3m4.droidc/usr/lib -L/sdcard/C4droid_EXT/lib";
		config.name[5] = "LDFLAGS";
		config.value[5] = "-lm -ldl -llog -lz -lncurses -L/sdcard/C4droid_EXT/lib -L/data/data/com.n0n3m4.droidc/usr/lib -Wl,-allow-shlib-undefined";
		config.name[6]= "PKG_CONFIG_PATH";
		config.value[6] = "/data/data/com.n0n3m4.droidc/files/gcc/arm-linux-androideabi/lib/pkgconfig:/sdcard/C4droid_EXT/lib/pkgconfig;/data/data/com.n0n3m4.droidc/usr/lib/pkgconfig";
		config.name[7] = "CC";
		config.value[7] = "/data/data/com.n0n3m4.droidc/files/gcc/bin/arm-linux-androideabi-gcc";
		config.name[8] = "CXX";
		config.value[8] = "/data/data/com.n0n3m4.droidc/files/gcc/bin/arm-linux-androideabi-g++";
		config.name[9] = "LD_LIBRARY_PATH";
		config.value[9] = "/data/data/com.n0n3m4.droidc/usr/lib:/vendor/lib:/system/lib";
		config.name[10] = "PREFIX";
		config.value[10] = "/data/data/com.n0n3m4.droidc/usr";		
		config.name[11] = "TEMP";
		config.value[11] = "/data/data/com.n0n3m4.droidc/tmp";
		config.name[12] = "TMP";
		config.value[12] = "/data/data/com.n0n3m4.droidc/tmp";
		config.name[13] = "TMPDIR";
		config.value[13] = "/data/data/com.n0n3m4.droidc/tmp";
		config.name[14] = "CMAKE_ROOT";
		config.value[14] = "/mnt/sdcard/C4droid_EXT";
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
		/*
		in doio.c PerlProc_execl(exsh,nash,cosh,cmd, (char *)NULL);
		for exec script with shebang #!/bin/sh 
		*/
		config.name[22] = "PERL_EXSH";
	    config.value[22] = "/data/data/com.n0n3m4.droidc/files/busybox";
	    config.name[23] = "PERL_NASH";
	    config.value[23] = "sh";
	    config.name[24] = "PERL_COSH";
	    config.value[24] = "-c";
		configsize = 25;		
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
	register HIST_ENTRY **the_list;
	register int i;
	register int j;
	the_list = history_list ();
    if (the_list){
        for (i= 1; the_list[i]; i++){
	        for (j= i+1; the_list[j]; j++){
		        if(strcmp(the_list[i]->line,the_list[j]->line)==0){
		        	
		        	HIST_ENTRY * entry=remove_history(i);
		        	free(entry->line);
		        	free(entry);
				    i--;
				    break;
			    }
		    }
        
	    }
    }
    write_history(NULL);	
	
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
	char *command;
	struct SHEBANG *execFile;
	execFile=(struct SHEBANG*)xmalloc(sizeof(struct SHEBANG));
	line=replace(line,"~",getenv("HOME"));
	if(!line){
		printf("ERROR:allocate memmory for replace()\n");
		return 0;
	}
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
	if (strstr(wordplus,"&& ")){
		printf("WARNING!!:found '&&'\nThe correct execution of multiple commands can not be guaranteed.\n");
	}
	if (*word) {
		command=(char *)basename(word);		
		/*configure*/
		if(strcmp(command,"configure")==0){
			if((strcmp(wordplus,"--help")!=0 \
				&& strcmp(wordplus,"-h")!=0 ) \
				&& (strcmp(wordplus,"-help")!=0 \
				&& strcmp(wordplus,"?")!=0)){
			    sprintf(syscom,\
					"/data/data/com.n0n3m4.droidc/usr/bin/CONFIGFIX %s --host=arm-linux-androideabi --build=i686-linux --disable-nls --prefix=%s CFLAGS='%s' CXXFLAGS='%s' LDFLAGS='%s' %s CONFIG_SHELL=sh",\
					word,getenv("PREFIX"),getenv("CFLAGS"),\
					getenv("CXXFLAGS"),getenv("LDFLAGS"),wordplus);
				printf("\001\e[1;33m\002 %s\001\e[00m\002\n",syscom);
			}else{
				sprintf(syscom,"%s %s --help",getenv("SHELL"),word);
			}
			system(syscom);
		/*make*/
		}else if(strcmp(command,"make")==0){
			sprintf(syscom,"%s %s CC='%s' CXX='%s' SHELL='%s'",\
				word,wordplus,getenv("CC"),getenv("CXX"),getenv("SHELL"));
			printf("\001\e[1;33m\002 %s\001\e[00m\002\n",syscom);
			system(syscom);
		/*cd*/
		}else if(strcmp(command,"cd")==0){			
		    if (wordplus==""){
		        if(chdir(getenv("HOME"))==-1){
					perror(getenv("HOME"));
		        }						
			}else if (chdir(wordplus) == -1) {
				perror(wordplus);
			}      
		/*ls*/
		}else if(strcmp(command,"ls")==0){
			sprintf(syscom, "busybox ls %s", wordplus);
			system(syscom);
		/*shebang*/
		}else if(strcmp(command,"shebang")==0){
			if(wordplus!=""){
			    if(getShebang(wordplus,execFile)){
				    printf("\001\e[1;34m\002File:\001\e[00m\002 %s\n\001\e[1;34m\002Shebang:\001\e[00m\002 %s\n",execFile->path2script,execFile->shebang);
					if(*execFile->opt){
					    printf("\001\e[1;34m\002Options:\001\e[00m\002 %s\n",execFile->opt);
					}
			    }else if(execFile->path2script){
				    printf("\001\e[1;34m\002File:\001\e[00m\002 %s\n",execFile->path2script);
			    }
			}else{
				printf("usage: shebang <file> \n");
			}
		/*su*/
		}else if(strcmp(command,"su")==0){
			free(line);
			free(origline);
			free(execFile);
			system("su -c c4dsh");
			return 0;
		/*quit*/
		}else if(strcmp(command,"quit")==0 || strcmp(command,"exit")==0){
			done=1;
		/*not substituted commands*/
		/*if script /bin/sh*/
		}else if(getShebang(word,execFile) && strstr(execFile->shebang,"#!/bin/sh")){
			sprintf(syscom,"%s %s %s",getenv("SHELL"),execFile->path2script,wordplus);
			system(syscom);
		/*if file exist*/
		}else if(execFile->path2script[0]){
			sprintf(syscom,"%s %s",execFile->path2script,wordplus);
			system(syscom);
		/*try exec*/
		}else{
			sprintf(syscom,"%s",origline);
			system(syscom);
		}
		
	}
	free(execFile);
	free(line);
	free(origline);
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



char *replace(char *instring,char *old,char *new)
{
	if(!instring || !old || !new){
		return (char*)NULL;
	}
	size_t instring_size=strlen(instring);
	size_t new_size=strlen(new);
	size_t old_size=strlen(old);
	size_t outstring_size=instring_size + 1;
	char *outstring;
	char *test;
	test=(char*)malloc(old_size+1);
	outstring =(char*) malloc(outstring_size);
	if(!outstring || !test){
		return (char*)NULL;
	}
	if(instring_size<old_size || old_size==0)
	{		
	   strcpy(outstring, instring);
	   free(test);
	   return outstring;
	}	
	outstring[0]='\0';
	int i;
	for(i=0; i <= instring_size; i++)
	{		
		strncpy(test,(instring+i),old_size);
		test[old_size]='\0';
		if(strcmp(test,old)==0){
			if(new_size!=old_size)
			{
				outstring_size=outstring_size+new_size-old_size;
				outstring=realloc(outstring,outstring_size);
				if(!outstring){
					free(test);
					return (char*)NULL;
				}
			}
			strcat(outstring,new);
			i=i+old_size-1;
		}else{
			test[1]='\0';
			strcat(outstring,test);
		}
	}
	free(test);
	return outstring;
}


//#include <linux/binfmts.h>
//#include <sys/param.h>
int getShebang(char *execname,struct SHEBANG *pTRshebang){

	if(!execname || !pTRshebang){return 0;}
	/*clean*/
	memset(pTRshebang,0,sizeof(*pTRshebang));
    FILE *file;
	int shebang_len =0;
	int opt_len =0;
	bool optexist =false;
	char ch= '\0';	
	struct stat sb;
	sprintf(pTRshebang->path2script,"%s",execname);
	/*if not full path?*/
	if(stat(pTRshebang->path2script,&sb)==-1 || S_ISDIR(sb.st_mode)){
		/*not in curdir?*/

		//static char cur4dir[MAXPATHLEN];
		//if(getcwd(cur4dir, MAXPATHLEN)!=NULL){
		if(cur4dir){
			sprintf(pTRshebang->path2script,"%s/%s",cur4dir,basename(execname));
		    if(stat(pTRshebang->path2script,&sb)==-1 || S_ISDIR(sb.st_mode)){
			    /* search in PATH */
			    char *start,*end,*sline;
				sline=(char*)malloc(strlen(getenv("PATH")) + 1);
				if(!sline){return 0;}
			    strcpy(sline,getenv("PATH"));
			    start=sline;
			    while(*start){
					end=find_char(start,':');
				    if (*end == ':'){*end = '\0';}
				    sprintf(pTRshebang->path2script,"%s/%s",start,basename(execname));
				    if (stat(pTRshebang->path2script, &sb)!= -1 && !S_ISDIR(sb.st_mode)){break;}
					pTRshebang->path2script[0]='\0';
				    start = lskip(end + 1);
			    }
			    free(sline);				
		    }
	    }
	}else{
		if(execname[0]!='/'){
		sprintf(pTRshebang->path2script,"%s/%s",cur4dir,basename(execname));
		}
	}
	if(!pTRshebang->path2script[0] || !(file = fopen(pTRshebang->path2script, "r"))){
	    return 0;
	}
	/*without fgets*/
	while(shebang_len < 2 && (ch=fgetc(file)) != EOF) {
        pTRshebang->shebang[shebang_len]=ch;
		shebang_len++;
    }
	/*if script*/
	if(pTRshebang->shebang && strstr(pTRshebang->shebang,"#!")){
	    while((ch=fgetc(file)) != EOF && (ch!='\n' && (shebang_len+opt_len)< BINPRM_BUF_SIZE)) {
		    if(isspace(ch)){
		        if(shebang_len==2){
				    continue;
				}else{
				    optexist=true;
				    if(opt_len==0){continue;}
				}
			}
			if(optexist){
			    pTRshebang->opt[opt_len]=ch;
				opt_len++;
			}else{
                pTRshebang->shebang[shebang_len]=ch;
                shebang_len++;
			}		    
		}
				
    }else{
		fclose(file);
	    return 0;
	}
	fclose(file);	
	if(ch =='\n'){return 1;}
    return 0;
}