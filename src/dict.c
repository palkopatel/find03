/*---------generaciya slovarya---------*/
/*-------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/dict.h"
#include "../include/findw.h"
#include "../include/hash.h"
#include "../include/platform.h"
/*-------------------------------------*/
int reclen;
int wordfile;
extern FILE *std_err;
extern char cfgdir[MAXPATH];
/*-------------------------------------*/
void search_word(char* pattern)
{
  FILE* in, *tmpfil/*,*out*/;
  char str[280], truename[280], word[280];
  unsigned num;

  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_TEMPFILES);
  if (!(in = fopen(filename, "rt")))
    error_open_file(filename, EOF_CODE_9);

/*  if(!(out=fopen(FILE_W_RESULTS,"wt")))error_open_file(FILE_W_RESULTS,10);*/
  for ( ; !feof(in); )
  {
    if (fscanf(in," %s", str) < 1) break;/*konec fayla*/
    *strchr(str, '*') = 0;
    strcpy(truename, str + strlen(str) + 1);
    *strchr(truename, '*') = 0;
    if (!(tmpfil = fopen(str, "rt"))) error_open_file(str, EOF_CODE_11);
    for ( ; !feof(tmpfil); )
    {
      if (fscanf(tmpfil, "%s", word) < 1) break;
      num = atoi(strchr(word, '=') + 1);
      *strchr(word, '=') = 0;
      if (!strcmp(word, pattern))
      {
        fprintf(stdout, "word '%s' (counter: %u) found in the file '%s'\n", pattern, num, truename);
        break;
      }
    }
    fclose(tmpfil);
  }
  fclose(in);
/*  fclose(out);*/
}
/*-------------------------------------*/
void create_dict(int repeater)
{
/*DEBUG: fprintf(stderr, "create_dict() is invoked with repeater=%d\n", repeater);*/
  FILE *tempfile, *in;
  char str[RFLEN], str2[6], nametmpfile[MAXPATH];
  unsigned fcod, num;

  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_TEMPFILES);
  if(!(in = fopen(filename, "rt")))
    error_open_file(filename, EOF_CODE_12);

  generate_word_file(repeater);
/*  generate_path_file();*/
  while (!feof(in))
  {
/*    if (fscanf(in, "%s", str) < 1) break;*/ /*awdeew: eta funkciya schitaet probeli koncom vvoda*/
    if (!fgets(str, RFLEN, in)) break; /*konec fayla*/
/*DEBUG:*/ fprintf(std_err, "tmp_file_name = %s\n", str);
    fcod = atoi(strcpy(str2, strrchr(str, '*') + 1));
    *strchr(str, '*') = 0;
    if (!(tempfile = fopen(str, "rt")))
    {
      error_open_file(str, EOF_CODE_13);
      continue; /*propustit' fayl*/
    }
    strcpy(nametmpfile, str);
    open_word_file(0); /*otkryt' fayl so slovami*/
    while (!feof(tempfile))
    {
      if (fscanf(tempfile, "%s", str) < 1) break;
/*DEBUG: fprintf(stderr, "create_dict() reads str='%s'\n", str);*/
      num = atoi(strchr(str, '=') + 1);
      *strchr(str, '=') = 0;
      save_word_2_file(repeater, str, fcod, num);
    }
    open_word_file(1);/*zakryt' fayl so slovami*/
    fclose(tempfile);
    remove(nametmpfile);
  }
  fclose(in);
}
/*-------------------------------------*/
/*void generate_path_file(void)
{
  unsigned i, j;
  FILE *file1;
  char str1[MAXPATH + 1 + LEN_META_DESC + 1];
  if (!(file1 = fopen(FILE_W_PATH, "wt"))) error_open_file(FILE_W_PATH, 141);
  for (i = 0; i < MAXPATH; i++)
    str1[i] = ' '; //zabit' imya fayla
  str1[i++] = '*'; //razdelitel'
  for ( ; i < MAXPATH + 1 + LEN_META_DESC; i++)
    str1[i] = ' '; //zabit' opisanie fayla
  str1[i] = 0;
  for(i = 0; i < LEN_FILE_W_PATH; i++)
    fprintf(file1, "%s", str1);
  fclose(file1);
}*/
/*-------------------------------------*/
void generate_word_file(int repeater)
{
  unsigned i, j;
  FILE *wordfile;
  char str1[LEN_LEXEM + 1];

  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_WORD);
  if(!(wordfile=fopen(filename, "wt")))
    error_open_file(filename, EOF_CODE_14);

  for(i=0; i<LEN_LEXEM; i++)
    str1[i]=' ';
  str1[i]=0;
  for(i=0; i<LEN_FILE_W_WORD; i++)
  {
    fprintf(wordfile, "%s;", str1);
    for(j=0; j<repeater; j++)
      fprintf(wordfile, PATTERN_LOCATION, 0, 0);
    fprintf(wordfile,"\n");
  }
  reclen=/*LEN_LEXEM*/ + 10 /*+ PLATFORM_STRING_END*/;
  fclose(wordfile);
}
/*-------------------------------------*/
/*
'word' - slovo
'numOfWordreps' - chislo povtorov slova v fayle s nomerom (iz hesha) 'numOfFile'
'repeater' - skol'ko real'no povtorov budet zapisano v fayl
*/
void save_word_2_file(int repeater, char* word, unsigned numOfFile, char numOfWordreps)
{
/*DEBUG: fprintf(stderr, "save_word_2_file() is invoked with repeater=%d, word='%s', numOfFile=%d, numOfWordreps=%d\n", repeater, word, numOfFile, numOfWordreps);*/
  unsigned int cod = hash(word, 0), i, min = 0, transf = 0, minindex = 0;
  char str[100], str2[100];
  while (1)
  {
    lseek(wordfile, (LEN_LEXEM + 1 +
		     repeater * reclen +
		     PLATFORM_STRING_END + 1) * (long)cod, SEEK_SET);
    read(wordfile,str,LEN_LEXEM+1+reclen);
    str[LEN_LEXEM+reclen]=0;
    strcpy(str2,str);
    *strchr(str,';')=0;/*otrezat' to chto ne slovo*/
    *strchr(str,' ')=0;/*otrezat' probely*/
    if(!strlen(str))/*naydeno novoe slovo*/
    {
      lseek(wordfile, (LEN_LEXEM + 1 +
		       repeater * reclen +
		       PLATFORM_STRING_END + 1) * (long)cod, SEEK_SET);
/*      lseek(wordfile,(reclen+1)*(long)cod,SEEK_SET);*/
      for(i=strlen(word); i<LEN_LEXEM; i++)
         word[i]=' ';
      word[i]=0;
      sprintf(str, PATTERN_LOCATION, numOfFile, numOfWordreps);
      sprintf(str2, "%s;%s", word, str);
      write(wordfile, str2, LEN_LEXEM+1+reclen);
/*DEBUG: fprintf(stderr, "save_word_2_file() returns with new word '%s'\n", str2);*/
      return;
    }
    if(strcmp(str, word))
      cod = hash(word, 1);/*slova ne sovpali: poluchim novoe znachenie hecha*/
    else/*slovo uzhe est' v fayle*/
    {
      char str_orig[100];
      strcpy(str_orig, str2);
      if (repeater == 1)
	min = atoi(strcpy(str_orig, strchr(str_orig, '=') + 1));/*sohranim chislo povtorov slova v predydushem fayle*/
      else
	for(i = 1; i < repeater; i++)
	{
	  transf = atoi(strcpy(str_orig, strchr(str_orig, '=') + 1));/*sohranim chislo povtorov slova v predydushem fayle*/
	  read(wordfile, str, reclen);
	  str[reclen - 1] = 0;/*'reclen-1' t. k. nuzhno otrezat' ';'*/
	  strcpy(str_orig, str);
	  if(transf > atoi(strcpy(str2, strchr(str, '=') + 1)))/*tekushee slovo vstrechaetsya rezhe chem proshliy minimum*/
	  {
	    minindex = i;
	    min = atoi(str2);
	  }
	  *strchr(str, '=') = 0;/*otbrosit' ravenstvo*/
	  if (strrchr(str, ' '))
	    strcpy(str, strrchr(str, ' ') + 1);/*otbrosit' lishnie probeli*/
	  if(!atoi(str))/*mesto svobodno!*/
	  {
	    lseek(wordfile, -reclen, SEEK_CUR);
	    sprintf(str, PATTERN_LOCATION, numOfFile, numOfWordreps);
	    write(wordfile, str, reclen);
/*DEBUG: fprintf(stderr, "save_word_2_file() returns after storing word\n");*/
	    return;
	  }
	}
      /*vse pozicii zapolneni - kto-to dolzhen umeret' (libo na odnoy iz poziciy libo pretendent)*/
      if (min < numOfWordreps)/*umret kto-to na pozicii*/
      {
	lseek(wordfile, (LEN_LEXEM + 1 +
			 repeater * reclen +
			 PLATFORM_STRING_END + 1) * (long)cod +
			 (LEN_LEXEM + 1) +
			 reclen*minindex, SEEK_SET);
	  sprintf(str, PATTERN_LOCATION, numOfFile, numOfWordreps);
	  write(wordfile, str, reclen);
      }
/*else;*/ /*umret pretendent*/
      return;
    }
  }
/*DEBUG: fprintf(stderr, "save_word_2_file() returns\n");*/
}
/*-------------------------------------*/
void open_word_file(char access)
{
/*DEBUG: fprintf(stderr, "open_word_file() is invoked with access='%d'\n", access);*/
  if(!access)
  {
    char filename[MAXPATH];
    strcpy(filename, cfgdir);
    strcat(filename, FILE_W_WORD);
    if(-1==(wordfile=open(filename, O_RDWR /*notlinux: | O_TEXT*/)))
      error_open_file(filename, EOF_CODE_15);
  }
  else close(wordfile);
}
/*-------------------------------------*/
unsigned no_this_file(char* filename)
{
  FILE* in;
  unsigned cod = hash(filename, 0), c2;
  char str[RFLEN], str2[RFLEN], *ptr;
#ifdef __TURBOC__
  str_tolower(filename); /*awdeew: also dangerous!!!*/
#endif

  char tmpfilename[MAXPATH];
  strcpy(tmpfilename, cfgdir);
  strcat(tmpfilename, FILE_W_TEMPFILES);
  if (!(in = fopen(tmpfilename, "a+t")))
    error_open_file(tmpfilename, EOF_CODE_7);

  fseek(in, 0L, SEEK_SET);
  while (!feof(in))
  {
    fseek(in, 0L, SEEK_CUR); /* eto dolzhnmo bit' ne nado? */
/*    if (fscanf(in, "%s", str) < 1) break;*/ /*konec fayla*/
    if (!fgets(str, RFLEN, in)) break; /*konec fayla*/
/*DEBUG: fprintf(std_err, "inside no_this_file: %s\n", str);*/
    strcpy(str2, str);
 /*otrezat' szadi znachenie hesha*/
    if ((ptr = strrchr(str, '*'))) /**strrchr(str, '*')*/ *ptr = 0;
    else continue;
 /*srezat' speredi imya vremennogo fayla*/
    if ((ptr = strchr(str, '*')))  strcpy(str, /*strchr(str, '*')*/ ptr + 1);
    else continue;
 /*perevesti hesh iz stroki v chislo*/
    if ((ptr = strrchr(str2,'*'))) c2 = atoi(/*(strrchr(str2, '*')*/ ptr + 1);
    else continue;
  /*awdeew?!: udalit' probeli*/
/*   while (strchr(str, ' ')) strcpy(str, strchr(str, ' ')); */
    if (!strlen(str)) break;
    if (!strcmp(str, filename)) /*ho-ho!!! fayl s takim imenem uzhe bil obrabotan*/
    {
      cod=0;
      break;
    }
 /*awdeew: pozicioniruem v konec fayla (bilo -- i rabotalo(!?) -- SEEK_SET)*/
/*    if (cod == c2) fseek(in, 0, SEEK_END);*/
 /*esli kodi sovpali, a imena raznia -> izmenim znachenie hesha (koda)*/
    while (cod == c2)
      cod = hash(filename, 1);
  }
  fclose(in);
  return cod;
}
