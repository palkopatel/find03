/*---------fayl s hech-funkciey--------*/
/*-------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/findw.h"
/*-------------------------------------*/
extern FILE *std_err;
/*-------------------------------------*/
unsigned hash_old(char* word,char enter)
{
  int i=0,len=strlen(word);
  static unsigned cod,loop;
  if(!enter)/*kod uzhe est', no on zanyat*/
  {
    for(loop=cod=i=0; i<len; i++)
      cod+=((unsigned)word[i])%LEN_FILE_W_WORD;
    cod%=LEN_FILE_W_WORD;/*awdeew: long vmesto unsigned =:(  )*/
    cod*=HASH_ARRAY_SEPARATOR;
    cod%=LEN_FILE_W_WORD;/*awdeew: long vmesto unsigned =:(  )*/
  }
  else/*sgenerit' sleduyushee znachenie*/
    if(cod+1==LEN_FILE_W_WORD)
    {
      if(!loop) loop++;
      else
      {
        fprintf(std_err,"\nERROR! Found many hash-values too!\n");
	my_exit(TOO_MANY_FILES, "hash-old");
      }
      cod=0;
    }
    else cod++;
  return cod;
}
/*-------------------------------------*/
unsigned hash(char* word,char enter)
{
  int i = 0, len = strlen(word);
  static unsigned long cod;
  static int loop;
  if (!enter) /*kod uzhe est', no on zanyat*/
  {
    for (loop = cod = i = 0; i < len; i++)
      cod += word[i] << 2 + word[i];
    cod *= 29; /*eto koefficient "rasseivaniya" (zhelatel'no prostoe chislo)*/
    if (cod > LEN_FILE_W_WORD) cod &= LEN_FILE_W_WORD; /*voz'meym ostatok ot deleniya :-) */
  }
  else /*sgenerit' sleduyushee znachenie*/
    if (cod + 1 >= LEN_FILE_W_WORD)
    {
      if (!loop) loop++;
      else
      {
        fprintf(std_err, "\nERROR! Found many hash-values too!\n");
        my_exit(TOO_MANY_FILES, "hash");
      }
      cod = 0;
    }
    else cod++;
  return cod;
}
/*-------------------------------------*/
