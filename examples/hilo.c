char *shell="/bin/bash";
char *opt="-i";
char *name="hilo";

char lines[]={
	0,13,9,11,5,72,67,68,5,72,75,89,66
	,32,42,0,34,9,10,89,75,71,90,70,79,10,66
	,67,7,70,69,10,77,75,71,79,10,89,66
	,79,70,70,10,89,73,88,67,90,94,32,42,0,64
	,79,73,66,69,10,115,69,95,10,94,66,67
	,68,65,10,69,76,10,75,10,68,95,71,72
	,79,88,10,76,88,69,71,10,27,10,94,69
	,10,27,26,24,30,10,75,68,78,10,99,10
	,93,67,70,70,10,77,95,79,89,89,10,67
	,94,4,32,42,0,9,126,120,99,111,121,23,27,32
	,42,0,9,98,99,23,27,26,24,30,32,42,0,6,102,101
	,23,27,32,42,0,12,93,66,67,70,79,10,94,88
	,95,79,32,42,0,4,78,69,32,42,0,36,10,10,109,127
	,111,121,121,23,74,79,82,90,88,10,118,2
	,10,14,98,99,10,1,10,14,102,101,10,118
	,3,10,5,10,24,74,32,42,0,23,10,10,79,73
	,66,69,10,99,89,10,67,94,10,14,109,127
	,111,121,121,118,21,32,42,0,72,10,10,79,73,66
	,69,10,7,68,10,13,122,70,79,75,89,79
	,10,94,83,90,79,10,8,66,8,6,10,76
	,69,88,10,66,67,77,66,10,8,70,8,10
	,76,69,88,10,70,69,93,10,69,88,10,8
	,73,8,10,76,69,88,10,73,69,88,88,79
	,73,94,16,10,13,32,42,0,17,10,10,88,79,75
	,78,10,120,111,121,122,101,100,121,111,32,42,0,28
	,10,10,67,76,10,113,10,8,14,120,111,121
	,122,101,100,121,111,8,10,23,10,8,66,8
	,10,119,32,42,0,8,10,10,94,66,79,68,32,42,0,29
	,10,10,10,10,126,120,99,111,121,23,74,79
	,82,90,88,10,14,126,120,99,111,121,10,1
	,10,27,74,32,42,0,15,10,10,10,10,98,99,23
	,14,109,127,111,121,121,32,42,0,14,10,10,10,10
	,73,69,68,94,67,68,95,79,32,42,0,6,10,10
	,76,67,32,42,0,28,10,10,67,76,10,113,10,8
	,14,120,111,121,122,101,100,121,111,8,10,23
	,10,8,70,8,10,119,32,42,0,8,10,10,94,66
	,79,68,32,42,0,29,10,10,10,10,126,120,99,111
	,121,23,74,79,82,90,88,10,14,126,120,99
	,111,121,10,1,10,27,74,32,42,0,15,10,10,10
	,10,102,101,23,14,109,127,111,121,121,32,42,0,14
	,10,10,10,10,73,69,68,94,67,68,95,79
	,32,42,0,6,10,10,76,67,32,42,0,28,10,10,67,76
	,10,113,10,8,14,120,111,121,122,101,100,121
	,111,8,10,23,10,8,73,8,10,119,32,42,0,8
	,10,10,94,66,79,68,32,42,0,41,10,10,10,10
	,79,73,66,69,10,99,10,109,101,126,10,99
	,126,10,120,99,109,98,126,10,99,100,10,14
	,126,120,99,111,121,10,126,120,99,111,121,32
	,42,0,12,10,10,10,10,79,82,67,94,10,26,32
	,42,0,6,10,10,76,67,32,42,0,6,78,69,68,79,32
	,42
	};

unsigned linesize=sizeof(lines);
// XSH.C The root part of the AWC Shell Script Compiler
// Written in 1990 by Al Williams
// Updated 1998 for Linux/ANSI

#define DEBUG 0
#define SUFFIX ".tmp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

int shpid;
FILE *out;
char pipename[32];
extern char *name;
extern char lines[];
extern char *shell;
extern char *opt;
extern unsigned linesize;
char linebuf[8192];
char *decode(char *s, size_t len);

// Called at death of child
void atend(int notused)
  {
  int rc;
  if (wait(&rc)==shpid) 
    rc=(rc&0xFF00)>>8; 
  else
    return; 
/* delete NSHPIPE */
  unlink(pipename);
  exit(rc);
  }  


// Look for a file along the path
int search_path(char *cmd, char *res_path, int access_mode)
{
    char *cp;   /* used to scan PATH for directories */
    int ret;      /* return value from access */
    char *pathenv;
    char tmppath[PATH_MAX];
    char curpath[PATH_MAX];
    char *path;
    int lastpath;
    int toolong=0;

#if DEBUG
printf("search_path: cmd = %s, access_mode = %d, fullpath = %d\n", cmd, access_mode, fullpath);
#endif

    /*
     * full or relative path was given
     */
    if ( (cmd[0] == '/') || ( (cp=strchr(cmd, '/')) != NULL )) {  
	if ( access(cmd, access_mode) == 0 ) {

	    if ( cmd[0] != '/' ) { /* relative path */
		if ( getcwd(curpath, PATH_MAX) == NULL ) {
		    strcpy(res_path, curpath);
		    return -1;
		}
		if ( (strlen(curpath) + strlen(cmd) + 1) > (size_t)PATH_MAX ) {
		    sprintf(res_path, "cmd (as relative path) and cwd is longer than %d",
			PATH_MAX);
		    return -1;
		}
		sprintf(res_path, "%s/%s", curpath, cmd);
	    }
	    else
	        strcpy(res_path, cmd);
	    return 0;
        }
	else {
	    sprintf(res_path, "file %s not found", cmd);
	    return -1;
	}
    }

    /* get the PATH variable */
    if ( (pathenv=getenv("PATH")) == NULL) {
        /* no path to scan, return */
	sprintf(res_path, "Unable to get PATH env. variable");
        return -1;
    }

    /*
     * walk through each path in PATH. 
     * Each path in PATH is placed in tmppath.  
     * pathenv cannot be modified since it will affect PATH.
     * If a signal came in while we have modified the PATH
     * memory, we could create a problem for the caller.
     */

    curpath[0]='\0';

    cp = pathenv;
    path = pathenv;
    lastpath = 0;
    for (;;) {

	if ( lastpath )
	    break;

	if ( cp != pathenv )
	    path = ++cp;	 /* already set on first iteration */

	/* find end of current path */

	for (; ((*cp != ':') && (*cp != '\0')); cp++);

	/*
	 * copy path to tmppath so it can be NULL terminated
	 * and so we do not modify path memory.
	 */
	strncpy(tmppath, path, (cp-path) );
	tmppath[cp-path]='\0';
#if DEBUG
printf("search_path: tmppath = %s\n", tmppath);
#endif

	if ( *cp == '\0' )
	    lastpath=1;		/* this is the last path entry */

	/* Check lengths so not to overflow res_path */
	if ( strlen(tmppath) + strlen(cmd) + 2 > (size_t)PATH_MAX ) {
	    toolong++;
	    continue;
	}

	sprintf(res_path, "%s/%s", tmppath, cmd);
#if DEBUG
printf("search_path: res_path = '%s'\n", res_path);
#endif



	if ( (ret=access(res_path, access_mode)) == 0 ) {
#if DEBUG
printf("search_path: found res_path = %s\n", res_path);
#endif
	    return 0;
	}
    }

    /* return failure */
    if ( toolong )
        sprintf(res_path,
	    "Unable to find file, %d path/file strings were too long", toolong);
    else
        strcpy(res_path, "Unable to find file");
    return 1;	/* not found */
}



int main(int argc,char *argv[],char *envp[])
  {
  char *p;
  char res_path[PATH_MAX];
  // validate that we can find the shell
  if (search_path(shell,res_path,X_OK))
    {
      fprintf(stderr,"%s: %s\n", name, res_path);
      exit(127);
    }
#if DEBUG
  else
    printf("Found executable %s\n",res_path);
#endif
  sprintf(pipename,".NSHPIPE%d%s",getpid(),SUFFIX);
  mknod(pipename,S_IFIFO|0666,0);  
  signal(SIGCHLD,atend);
  signal(SIGTERM,SIG_IGN);
  signal(SIGINT,SIG_IGN);
  signal(SIGHUP,SIG_IGN);
  signal(SIGQUIT,SIG_IGN);
  if (!(shpid=fork()))
   {
   char **newargv;
   int i,o=1;
   newargv=(char **)malloc((argc+3)*sizeof(char *));
   if (!newargv) { strcpy(linebuf,name); strcat(linebuf," (1)"); perror(linebuf); exit(127); }
   newargv[0]=shell;
   if (opt) newargv[o++]=opt;
   newargv[o]=pipename;
   for (i=1;i<argc;i++) newargv[i+o]=argv[i];
   newargv[i+o]=NULL;
#if DEBUG
   fprintf(stderr,"Exec: %s\n", shell);
   i=0;
   while (newargv[i]) { fprintf(stderr,"argv[%d]=%s\n", i, newargv[i]); i++;}
#endif
   execve(shell,newargv,envp);
/* should not get here */
   strcpy(linebuf,name);
   strcat(linebuf," (2)");
   perror(linebuf); 
   exit(127);
   }
  out=fopen(pipename,"w");
  if (!out) { strcpy(linebuf,name); strcat(linebuf," (3)"); perror(linebuf); exit(127); } else
    {
    unsigned char *lp=lines;
    while (linesize>0)
      {
	unsigned llen=(*lp++)*256;
	llen+=(*lp++);
	fprintf(out,"%s\n",decode(lp,llen));
	fflush(out);
	lp+=llen;
	linesize-=llen+2;
      }
#if EXIT    // This is the old way to close the script
            // which is fine for shell, but not for other non-shells
    fprintf(out,"exit\n");
    fflush(out);
#endif
    fclose(out);
    }
  pause();  /* wait for SIGCLD */
  // This just keeps the complier happy. Actually, at SIGCLD
  // the atend handler returns the exit code of the child
  return 0;  
}

char *decode(char *s, size_t len)
   {
    memfrob(s,len);
    return s;
   }



