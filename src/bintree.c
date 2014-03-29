/*---------rabota s derevom------------*/
/*-------------------------------------*/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "../include/findw.h"
#include "../include/bintree.h"
/*-------------------------------------*/
BinTree *tree, *p;
FILE* outfile;
extern char tempname[MAXPATH];
extern char cfgdir[MAXPATH];
/*-------------------------------------*/
/*sozdanie kornya binarnogo dereva (B.D.)*/
int MakeTree(char *info)
{
  if (!(tree=malloc(sizeof(BinTree)))) my_exit(NOT_ENOUGH_MEMORY, "mtree-1");
  if (!(tree->info = strdup(info))) my_exit(NOT_ENOUGH_MEMORY, "mtree-2");
  tree->pFather=tree->pRight=tree->pLeft=NULL;
/*  strcpy(tree->info,info);*/
  tree->counter=1;
  return 0;
}
/*-------------------------------------*/
/*sozganie levogo cyna dlya uzla *p*/
int SetLeft(char *info)
{
  BinTree* q;
  if(!(q=malloc(sizeof(BinTree)))) my_exit(NOT_ENOUGH_MEMORY, "leftt-1");
  if (!(q->info = strdup(info))) my_exit(NOT_ENOUGH_MEMORY, "leftt-2");
  q->pFather=p;
  p->pLeft=q;
  q->pLeft=q->pRight=NULL;
/*  strcpy(q->info,info);*/
  q->counter=1;
  return 0;
}
/*-------------------------------------*/
/*sozdanie pravogo syna dlya uzla *p*/
int SetRight(char *info)
{
  BinTree* q;
  if(!(q=malloc(sizeof(BinTree)))) my_exit(NOT_ENOUGH_MEMORY, "rightt-1");
  if (!(q->info = strdup(info))) my_exit(NOT_ENOUGH_MEMORY, "rightt-2");
  q->pFather=p;
  p->pRight=q;
  q->pLeft=q->pRight=NULL;
/*  strcpy(q->info,info);*/
  q->counter=1;
  return 0;
}
/*-------------------------------------*/
/*raznye izvrachenye operacii s derevom*/
char* create_and_goto(char Case,char* info)
{
/*  MakeTree("\0");*/
/*  p=tree;*/
  switch(Case){
	case CREATE_L:
	  SetLeft(info);
	  break;
	case CREATE_R:
	  SetRight(info);
	  break;
	case GOTO_L:
	  if(!p->pLeft)
	    return NULL;
	  p=p->pLeft;
	  break;
	case GOTO_R:
	  if(!p->pRight)
	    return NULL;
	  p=p->pRight;
	  break;
	case GOTO_F:
	  if(p==tree)
	    return NULL;
	  p=p->pFather;
  }
  return p->info;
}
/*-------------------------------------*/
/*
'tree' - eto koren' dereva
'p'    - eto tekushiy element
*/
/*-------------------------------------*/
int place_lex_to_tree(char* str1)
{
  int r=1;
  if(!tree)
  {
    MakeTree(str1);
    return 0;
  }
  p=tree;
  for(r=strcmp(p->info,str1);r;r=strcmp(p->info,str1))
  {
    if(r<0)
      if(p->pLeft)p=p->pLeft;
      else break;
    else if(p->pRight)p=p->pRight;
	 else break;
  }
  if(!r)
  {
    if(p->counter<MAX_NUM_ONE_LEXEM)p->counter++;
    return 0;/*leksema uzhe vstrechalas'*/
  }
  if(r<0)create_and_goto(CREATE_L,str1);
  else create_and_goto(CREATE_R,str1);
  return 1;/*leksema zanesena v derevo*/
}
/*sohranity' derevo v fayl (simmetrichnym obhodom)*/
int save_tree_2_file(char* sourcefile,unsigned numSourcefile)
{
  FILE* tmpfil;
/*
  int i;
  for (i = 0; !(tmpfil=fopen(tempname,"w")) && i < NUM_TRIAL; i++, sleep(1))
    fprintf(stderr, "try open file %s\n", tempname);
  if (i == NUM_TRIAL) error_open_file(tempname,5);
*/
  if (!(tmpfil = fopen(tempname, "w"))) error_open_file(tempname, 5);
  p=tree;
  if(p)round_tree(p,tmpfil);
  fclose(tmpfil);

  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_TEMPFILES);
  if(!(outfile=fopen(filename, "a+t")))
    error_open_file(filename, 6);

  fseek(outfile,0L,SEEK_END);
  fprintf(outfile,"%s*%s*%u\n",tempname,sourcefile,numSourcefile);
/*DEBUG: fprintf(stderr, "TEMPORARY FILENAME: %s\n",tempname);*/
  fclose(outfile);
  return 0;
}
/*--------------------------------------*/
int round_tree(BinTree* p, FILE* tmpfil)
{
  if(p->pLeft) round_tree(p->pLeft,tmpfil);
  if(p->counter)
  {
    fprintf(tmpfil,"%s=%u\n",p->info,p->counter);
    p->counter=0;
  }
  if(p->pRight) round_tree(p->pRight,tmpfil);
  return 0;
}
/*--------------------------------------*/
