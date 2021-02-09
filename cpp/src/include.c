#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpp.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

Includelist	includelist[NINCLUDE];

extern char	*objname;

#if defined(_WIN32) || defined(_WIN64)
#define CSEPFILE '\\'
#define SSEPFILE  "\\"
#else
#define CSEPFILE '/'
#define SSEPFILE  "/"
#endif

void
doinclude(Tokenrow *trp)
{
    //  fprintf(stderr,"## fn   : doinclude\n"); // DEBUG

	char fname[256], iname[256];
	Includelist *ip;
	int angled, len, i;
	FILE *fd;

	trp->tp += 1;
	if (trp->tp>=trp->lp)
		goto syntax;
        
	if (trp->tp->type!=STRING && trp->tp->type!=LT) 
    {
		len = trp->tp - trp->bp;
		expandrow(trp, "<include>");
		trp->tp = trp->bp+len;
	}
	if (trp->tp->type==STRING) 
    {
		len = trp->tp->len-2;
		if (len > sizeof(fname) - 1)
			len = sizeof(fname) - 1;
		strncpy(fname, (char*)trp->tp->t+1, len);
		angled = 0;
	} 
    else
    { 
        if (trp->tp->type==LT) 
        {
            len = 0;
            trp->tp++;
            while (trp->tp->type!=GT) 
            {
                if (trp->tp>trp->lp || len+trp->tp->len+2 >= sizeof(fname))
                    goto syntax;
                strncpy(fname+len, (char*)trp->tp->t, trp->tp->len);
                len += trp->tp->len;
                trp->tp++;
            }
            angled = 1;
        } 
        else
        {
            goto syntax;
		}
    }
        
	trp->tp += 2;
	if (trp->tp < trp->lp || len==0)
		goto syntax;
	fname[len] = '\0';
    
    // fprintf(stderr,"## file : [%s]\n",fname);   ; // DEBUG

	if (fname[0]==CSEPFILE) 
    {
		fd = fopen(fname, "r");
		strcpy(iname, fname);
	} 
    else 
    {
        for (fd = NULL,i=NINCLUDE-1; i>=0; i--) 
        {
            ip = &includelist[i];
            if (ip->file==NULL || ip->deleted || (angled && ip->always==0))
                continue;
            if (strlen(fname)+strlen(ip->file)+2 > sizeof(iname))
                continue;
              
            strcpy(iname, ip->file);
            strcat(iname, SSEPFILE);
            strcat(iname, fname);

            #if defined(_WIN32) || defined(_WIN32)
            for(int i=0;iname[i]!=0;i++) if (iname[i]=='/') iname[i]='\\';
            #endif

            //fprintf(stderr,"## ifile : [%s]\n",iname); // DEBUG

            if ((fd = fopen(iname, "r")) != NULL)
                break;
        }
    }
    
	if ( Mflag>1 || !angled&&Mflag==1 ) 
    {
		fwrite(objname,1,strlen(objname),stdout);
		fwrite(iname,1,strlen(iname),stdout);
		fwrite("\n",1,1,stdout);
	}
	if (fd != NULL) 
    {
		if (++incdepth > 10)
			error(FATAL, "#include too deeply nested");
		setsource((char*)newstring((uchar*)iname, strlen(iname), 0), fd, NULL);
		genline();
	} 
    else 
    {
		trp->tp = trp->bp+2;
		error(ERROR, "Could not find include file %r", trp);
	}
    
	return;
    
syntax:
	error(ERROR, "Syntax error in #include");
	return;
}

/*
 * Generate a line directive for cursource
 * outp in windows is function switch with Outp
 */
void
genline(void)
{
	static Token ta = { UNCLASS };
	static Tokenrow tr = { &ta, &ta, &ta+1, 1 };
	uchar *p;

	ta.t = p = (uchar*)Outp;
	strcpy((char*)p, "#line ");
	p += sizeof("#line ")-1;
	p = (uchar*)outnum((char*)p, cursource->line);
	*p++ = ' '; *p++ = '"';
	strcpy((char*)p, cursource->filename);
	p += strlen((char*)p);
	*p++ = '"'; *p++ = '\n';
	ta.len = (char*)p-Outp;
	Outp = (char*)p;
    
    //fprintf(stderr,"## genline outp : [%s]\n",ta.t); // DEBUG
    #if defined(_WIN32) || defined(_WIN32)
    for(int i=0;ta.t[i]!=0;i++) if (ta.t[i]=='/') ta.t[i]='\\';
    #endif

	tr.tp = tr.bp;
	puttokens(&tr);
}

void
setobjname(char *f)
{
	int n = strlen(f);
	objname = (char*)domalloc(n+5);
	strcpy(objname,f);
    
	if(objname[n-2]=='.'){
		strcpy(objname+n-1,"$O: ");
	}else{
		strcpy(objname+n,"$O: ");
	}
}

#ifdef __GNUC__
// -Wparentheses
#pragma GCC diagnostic pop
#endif


/**/


