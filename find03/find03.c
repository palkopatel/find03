/*---------glavnyy faylik--------------*/
/*-------------------------------------*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/conv.h"
#include "../include/findw.h"
#include "../include/hash.h"
#include "../include/platform.h"
/*-------------------------------------*/
#define MAXRES 30
#define CONTENT_TYPE "Content-Type: text/html\n\n"

#ifdef _MSDOS_
  #define STRING_HEADER "<html><meta charset=\"CP1251\"><head>\n<style type=\"text/css\">\
\np{color:#000077;\nfont-family:\nTahoma,Verdana,Arial;\nfont-size:10pt;}\
\nh3,h2{color:#ffaa00;\nfont-family:\nTahoma,Verdana,Arial;\ntext-align:center;\nfont-size:18pt;}\
\n</style>\n</head>\n<body bgcolor=ffffc2>\
\n<h2>Ïמטסך חאגונרום.</h2>\n"
  #define STRING_TITLE "<h3>Ñכמגמ <b>'%s'</b>&nbsp םאיהוםמ ג %d פאיכאץ (פאיכו)</h3>"
  #define STRING_FOUND "<p>Ñכמגמ '%s' םאיהוםמ ג פאיכו <a href=\"%s/%s\">\"%s\"</a><br>×טסכמ ןמגעמנמג = %u</p><hr width=320 height=1>\n"
#else
  #define STRING_HEADER "<html><meta charset=\"KOI8-R\"><head>\n<style type=\"text/css\">\
\np{color:#000077;\nfont-family:\nTahoma,Verdana,Arial;\nfont-size:10pt;}\
\nh3,h2{color:#ffaa00;\nfont-family:\nTahoma,Verdana,Arial;\ntext-align:center;\nfont-size:18pt;}\
\n</style>\n</head>\n<body bgcolor=ffffc2>\
\n<h2>נÏÉÓË ÚÁ×ÅÒÛÅÎ.</h2>\n"
  #define STRING_TITLE "<h3>ףÌÏ×Ï <b>'%s'</b>&nbsp ÎÁÊÄÅÎÏ × %d ÆÁÊÌÁÈ (ÆÁÊÌÅ)</h3>"
  #define STRING_FOUND "<p>ףÌÏ×Ï '%s' ÎÁÊÄÅÎÏ × ÆÁÊÌÅ <a href=\"%s/%s\">\"%s\"</a><br>‏ÉÓÌÏ ÐÏ×ÔÏÒÏ× = %u</p><hr width=320 height=1>\n"
#endif
/*-------------------------------------*/
FILE *std_out = NULL;
/*-------------------------------------*/
struct Res{
	unsigned fnum;
	unsigned count;
	char *filename;
} res[MAXRES];
/*-------------------------------------*/
int search_machine(char*);
void results_analiz(int, char*);
void sort_results(int);
/*-------------------------------------*/
unsigned char hex_2_i(unsigned char sim)
{
  if (isalpha(sim)) sim = toupper(sim);
  if (sim >= '0' && sim <='9') return (sim - '0');
  if (sim >= 'A' && sim <='F') return (sim - 55); /* 'A' == 65*/
  return '-';
}
/*-------------------------------------*/
int convert_percent_symbols(char *dest)
{
  int len = strlen(dest), i, j;
  char *ptr;
  if (!(ptr = strdup(dest)))
    return 1; /*net pamyati!!!*/
  for (j = i = 0; i < len; i++)
  {
    if (ptr[i] == '%')
    {
      dest[j++] = (hex_2_i(ptr[i + 1]) << 4) + hex_2_i(ptr[i + 2]);
      i += 2;
    }
    else dest[j++] = ptr[i];
  }
  dest[j] = 0;
  free(ptr);
  return 0;
}
/*-------------------------------------*/
int main(int argc,char**argv)
{
  int i;
  if(argc==1)
  { /* TODO: http code looks like un-working. */
    fprintf(stdout, CONTENT_TYPE);
    fprintf(stdout, STRING_HEADER);
    char *ptr, str[MAXPATH];
    *str = 0;
/*    fprintf(stdout,"\nusage: find03 {word for search}\n\n");*/
    ptr = getenv("CONTENT_LENGTH");
    if (ptr != NULL && strlen(ptr) > 0)
    {
      fgets(str, atoi(ptr) + 1, stdin);
      std_out = stdout; /*smotret' nizhe*/
      if ((ptr = strchr(str, '=')))
      {
        strcpy(str, ptr + 1);
        convert_percent_symbols(str);
        convert_2_uppercase(str);
        while (str)
        {
          if (!(ptr = strrchr(str, '+')))
            break;
          else
          {
            results_analiz(search_machine(ptr + 1), ptr + 1);
            *ptr = 0;
          }
        }
        results_analiz(search_machine(str), str);
        return 0;
    }
    }
    return 1;
  }
  for(i = 1; i < argc; i++)
    if (!strcmp(argv[i], "--file"))
      std_out = fopen("res.htm", "wt");
  if (!std_out) std_out = stdout;
  else fprintf(std_out, STRING_HEADER);
  for(i = 1; i < argc; i++)
    if (strcmp(argv[i], "--file"))
    {
      convert_2_uppercase(argv[i]);
      results_analiz(search_machine(argv[i]), argv[i]);
    }
  if (std_out) fclose(std_out);
  return 0;
}
/*-------------------------------------*/
void cut_word(char *str1, int i)
{
  if (strlen(str1) >= i)
    str1[i - 1] = 0;
}
/*-------------------------------------*/
int search_machine(char *word)
{
  FILE *dictfile;
  int sim = 0, i = 0;
  unsigned wlenrec, fcod;
  char *str1, *str2, *ptr;

/*DEBUG: fprintf(stderr, "Start to find the word '%s'\n", word);*/

  if (!(dictfile = fopen(FILE_W_WORD, "rt")))
    my_exit(ERROR_OPEN_FILE, FILE_W_WORD);
  for (wlenrec = PLATFORM_STRING_END; !feof(dictfile) && sim != '\n'; wlenrec++)
  {
    sim = getc(dictfile);
    if (sim == ';' &&  !i)
      i = wlenrec - PLATFORM_STRING_END; /*vichislim maksimal'nuyu dlinu slova*/
  }

/*DEBUG: fprintf(stderr, "Allocate memory for two strings in %u bytes\n", wlenrec);*/

  if (!(str1 = (char*)malloc(wlenrec + 1))) my_exit(NOT_ENOUGH_MEMORY, "");
  if (!(str2 = (char*)malloc(wlenrec + 1))) my_exit(NOT_ENOUGH_MEMORY, "");
  cut_word(word, i); /*obrezhem dlinnie slova*/
  i = 0;
  fcod = hash(word, 0);
  while (1)
  {
    fseek(dictfile, (long)wlenrec * (long)fcod, SEEK_SET);
    fgets(str1, wlenrec, dictfile);
/*DEBUG: fprintf(stderr, "\n%s\n", str1);*/
    strcpy(str2, str1);
    if (strchr(str2, ';')) *strchr(str2, ';') = 0;
    if (strchr(str2, ' ')) *strchr(str2, ' ') = 0;
/*DEBUG: fprintf(stderr, "\nWord '%s' was found\n", str2); */
    if (!strcmp(word, str2))
      break; /*slovo naydeno*/
    if (!strlen(str2))
      return 0; /*slovo ne naydeno*/
    fcod = hash(word, 1);
  }
  if (strchr(str1, ';'))
    strcpy(str1, strchr(str1, ';') + 1);
  while ((ptr = strchr(str1, ';')) && i < MAXRES)
  {
    strncpy(str2, str1, sim = strlen(str1) - strlen(ptr));
    str2[sim] = 0;
    strcpy(str1, ptr + 1);
    if ((ptr = strchr(str2, '=')))
    {
      sim = atoi(ptr + 1);
      ptr = 0;
      fcod = atoi(str2);
      if (sim && fcod)
      {
	res[i].fnum = fcod;
	res[i++].count = sim;
      }
    }
  }
  free(str1);
  free(str2);
  fclose(dictfile);
  return i;
}
/*-------------------------------------*/
void sort_results(int count)
{
  struct Res work;
  int i, j;
/*DEBUG: for (i = 0; i < count; i++) fprintf(std_out,"<br><b>before sorting... %d</b>", res[i].fnum);*/
  for (i = 0; i < count; i++)
    for (j = i; j < count; j++)
      if (res[j].count > res[i].count)
      {
        work = res[i];
        res[i] = res[j];
        res[j] = work;
      }
}
/*-------------------------------------*/
void results_analiz(int count, char* word)
{
  FILE *in, *prefixfile;
  char str[RFLEN], *ptr, prefix[MAXPATH] = "";
  unsigned num, i, j;
  if (!(in = fopen(FILE_W_TEMPFILES, "rt")))
    my_exit(ERROR_OPEN_FILE, FILE_W_TEMPFILES);
  if ((prefixfile = fopen(FILE_W_PREFIX, "rt")))
  {
    fgets(prefix, MAXPATH - 1, prefixfile);
    if (strlen(prefix) >= MAXPATH) prefix[MAXPATH - 1] = 0;
    if (strchr(prefix, '\n')) *strchr(prefix, '\n') = 0;
    if (strchr(prefix, '\r')) *strchr(prefix, '\r') = 0;
    fclose(prefixfile);
  }
  else
  {
/*    my_exit(ERROR_OPEN_FILE, FILE_W_PREFIX);*/
  }
  sort_results(count);
  for (j = 0 ; !feof(in) && j < count; )
  {
/*    if (fscanf(in,"%s",str) < 1) break;*/
    if (!fgets(str, RFLEN, in)) break; /*konec fayla*/
    if ((ptr = strchr(str, '*')))
      strcpy(str, ptr + 1);
    if ((ptr = strchr(str, '*')))
    {
      num = atoi(ptr + 1);
      *strchr(str, '*') = 0;
    }
    for (i = 0; i < count; i++)
      if (res[i].fnum == num)
      {
/*	fprintf(stdout, "<p>word '%s' (counter: %u) found in the file <a href=\"%s/%s\">\"%s\"</a></p>\n", word, res[i].count, prefix, str, str);*/
	res[i].filename = strdup(str);
	j++;
	break;
      }
  }
  fprintf(std_out, STRING_TITLE, word, count);
/*  if (!j)
    fprintf(stdout, "<p>word '%s' not found</p>\n", word);
  else*/
    for (i = 0; i < count; i++)
    {
      fprintf(std_out, STRING_FOUND, word, prefix, res[i].filename, res[i].filename, res[i].count);
      free(res[i].filename);
    }
  fclose(in);
}
/*-------------------------------------*/
void my_exit(int error_code, char *fromf)
{
  exit(error_code);
}
/*-------------------------------------*/
