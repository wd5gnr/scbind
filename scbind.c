/*
    Copyright 1990, 1998, 2005, 2006 by Al Williams (alw@al-williams.com)

    This file is part of Binds

    Binds is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Binds is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Binds; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


*/


// SCBIND.C The "compiler" part of the AWC Shell Script Compiler
// Written in 1990 by Al Williams
// Updated 1998 for Linux/ANSI
// Updated 2006 for GPL release
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

// Assume no lines bigger than 8K
char linebuf[8192];
// User provided encoder and decoder
char *decode(char *,size_t len);  // not really used here
char *encode(char *, size_t len, size_t *outlen);
FILE *efopen();

// Users shouldn't call this directly, but if you must
// $1 = input name
// $2 = C program name (output)
// $3 = output name
// $4 = Shell line
// $5 = Shell option
main(int argc,char *argv[])
  {
  FILE *in,*out;
  int first=1;
  int linect=0;
  int n=0;
  char *lp;
  
/* For now first 4 arguments are required */
  if (argc!=5&&argc!=6) help();
  in=efopen(argv[1],"r");
  out=efopen(argv[2],"w");
  // figure out shell name
  strcpy(linebuf,argv[4]);
  while (isspace(*(linebuf+n))) n++;  /* skip spaces */
  if (linebuf[n]!='#' || linebuf[n+1]!='!')
    strcpy(linebuf,"#!/bin/sh");  // default
  else
    while (isspace(*(linebuf+n+2))) n++;
  fprintf(out,"char *shell=\"%s\";\n",linebuf+n+2);
  // figure out arguments
  if (argc==6)
    fprintf(out,"char *opt=\"%s\";\n",argv[5]);
  else
    fprintf(out,"char *opt=(char *)0;\n");
  // name program
  fprintf(out,"char *name=\"%s\";\n\n",argv[3]);
  // spit out encoded text
  fprintf(out,"char lines[]={\n");
  linebuf[0]='\0';
  lp=linebuf;
  
  while (!feof(in))
    {
    size_t result;
    // get line of script
       lp=fgets(lp,sizeof(linebuf)-strlen(linebuf),in);
      if (!lp) 
        if (feof(in)) break; else error("input file");
        lp=strchr(linebuf,'\n');
	if (!lp) error("Line too long");
	if (lp[-1]=='\\') { *--lp='\0'; continue; }
      // encode it 
      lp=encode(linebuf,strlen(linebuf)+1,&result);
 // this is ugly. We assume no line is >8192 so two characters
 // can hold it. So even with buffer change 64K line limit!
      fprintf(out,"%c%d,%d",first?'\t':',',(int)result/256,(int)(result&0xFF));
      first=0;
      while(result--)
          { 
	    // every 10 lines, output a new line to keep things neat
          if (linect++>10) { linect=0; fprintf(out,"\n\t"); }

	  // output each byte
          fprintf(out,",%d",*lp++);

          }
      // end of line
      //      fprintf(out,",0");
      lp=linebuf;
      *linebuf='\0';
      
    }
  // write code to compute total
  fprintf(out,"\n\t};\n\nunsigned linesize=sizeof(lines);\n");
  if (fclose(out)) error("output file");
  exit(0);
  }


help()
  {
  fprintf(stderr,"SCBIND " VERSION " -- shell script binder by Al Williams\n");  
  fprintf(stderr,"Usage: scbind INFILE OUTFILE NAME SHELL [ARG]\n");
  exit(1);
  }

// some kind of error
error(char *s)
  {
  perror(s);
  exit(2);
  }

// Open with error checking
FILE *efopen(char *fn,char *mode)
  {
  FILE *fp;
  if (!(fp=fopen(fn,mode))) error(fn);
  return fp;
  }



















