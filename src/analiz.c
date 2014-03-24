/*---------generaciya slovarya---------*/
/*-------------------------------------*/
#include <ctype.h> /*eto dlya funkcii tolower()*/
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/findw.h"
/*-------------------------------------*/
#define LEN_TAGINSIDE (100 * LEN_TAG)
#define LEN_LEX_NEW (10 * LEN_LEXEM)
/*-------------------------------------*/
char lex[LEN_LEX_NEW], tag[LEN_TAG], taginside[LEN_TAGINSIDE];
char separators[256];/*dlina == chislu simvolov*/
unsigned char T/*mozhno analozirovat'*/;
unsigned char F/*nel'zya analozirovat'*/;
extern char lexem[LEN_ARRAY_LEXEM][LEN_TAG];
extern char unlexem[LEN_ARRAY_LEXEM][LEN_TAG];
extern char FILE_4_ERRORS[MAXPATH];
extern FILE* std_err;
/*-------------------------------------*/
char read_tag(FILE*);
int is_separator(char);
int is_mini_separator(char);
int str_lwr(char*);
int is_true_tag(char*);
int is_false_tag(char*);
int parse_href(char*, char*);
int split_path_f_filename(char*, char*);
/*-------------------------------------*/
/* sdelaem vse razdeliteli katalogov odnogo vida */
void convert_directory_separators(char *str1)
{
  char *ptr;
  while ((ptr = strchr(str1, '\\')))
    *ptr = '/';
}
/*-------------------------------------*/
/*vityanut' iz fayla ego put' i sohranit' v stroku 'path'*/
split_path_f_filename(char *filename, char *path)
{
/*DEBUG: fprintf(stderr, "split_path_f_filename is invoked with filename '%s' and path '%s'\n", filename, path);*/
  char *ptr2;
  if (!strlen(filename)) return 255;
  strcpy(path, filename);
  convert_directory_separators(path);  
  ptr2 = strrchr(path, '/');
  if (ptr2) 
  {
    char *pwork;
/* razdelitel' "/" v nachale puti i v seredine obrabativaetsya po-raznomu */
    if ((pwork = strstr(path, "./"))) 
      if (pwork + 1 == ptr2) ptr2++; /* v nachale puti */
    *(ptr2 + 1) = 0;
  }
  else strcpy(path, "./"); /* razdelenie ne trebuetsya */
/*  if (*(path + strlen(path) - 1) != '/') strcat(path, "/");*/ /* uzhe vrode ne nuzhno */
  return 0;
}
/*-------------------------------------*/
false_file_type(char* filename)
{
/*DEBUG: fprintf(stderr, "false_file_type() is invoked with filename '%s'\n", filename);*/
  FILE* source;
  unsigned char sim, str1[LEN_PROBE + 1]; /* LEN_PROBE = 255 (by default) */
  int i;
  DIR* dir = opendir(filename);
/*DEBUG: fprintf(stderr, "opendir() returned '%p' and errno=%d\n", dir, errno);*/
  if (errno==ENOENT) 
  {
    error_open_file(filename, 3); /* TODO: replace to 31 */
    return ERROR_OPEN_FILE;
  }
  if (dir!=NULL)
  {
    struct dirent* dp = readdir(dir);
    if (dp != NULL && dp->d_type == DT_DIR) {
      error_open_file(filename, 3); /* TODO: replace to 32 */
      return FILE_IS_DIRECTORY;
    }
  }
  if (!(source = fopen(filename, "rb"))) 
  {
    error_open_file(filename, 3);
    return ERROR_OPEN_FILE;
  }
  for (i = 0, sim = fgetc(source); !feof(source) && i < LEN_PROBE; sim = fgetc(source), str1[i++] = sim)
  {
    if (sim < '\t')
      return BAD_FILE_TYPE; /* vstretilsya simvil kotorogo v tekstovom fayle bit' ne mozhet */
  }
  str1[i] = 0;
  fclose(source);
  return 0;
  
}
/*-------------------------------------*/
/*vychlenenie i podschet slov v zadannom fayle*/
analiz_file(char* filename)
{
/*DEBUG: fprintf(stderr, "analiz_file() is invoked with filename '%s'\n", filename);*/
  FILE* source;
  unsigned char sim, r, i;
  char linkname[MAXPATH] = "", path[MAXPATH] = "";
  if (false_file_type(filename)) /* proverim html-fyal li podsunut dlay obrabotki */
    return BAD_FILE_TYPE;
  if (!(source = fopen(filename, "rt"))) 
  {
    error_open_file(filename, 3);
    return ERROR_OPEN_FILE;
  }
  strcpy(lex, "");
  split_path_f_filename(filename, path);
  for (sim = fgetc(source), T = F = i = 0; !feof(source); sim = fgetc(source))
  {
     if (sim == '<')
     {
       r = read_tag(source);
/*DEBUG: fprintf(std_err, "tag = '%s', taginside = '%s', separators = '%s'\n", tag, taginside, separators);*/
       if (r=='/')/*vozmozhno vstretilsya zakryvayushiy teg*/
       {
         if (is_true_tag(tag))/*kakogo tipa etot zakryvayushiy teg*/
         {
            if (T>0) T--; /*...i stoit li ego zakryvat'*/
         }
         else 
         {
            if (is_false_tag(tag))
              if (F>0) F--;
         }
       }
       else
       {
         if (is_true_tag(tag)) T++;
         else if (is_false_tag(tag)) F++;
         if (!parse_href(tag, linkname)) /*razberemsya s drugimi poleznimi tegami*/
         {
/*DEBUG: fprintf(stderr, "linkname = %s\n", linkname);*/
           char strt[MAXPATH];
           if (strchr(linkname, '\\')) convert_directory_separators(linkname);
           strcpy(strt, linkname);
           str_tolower(strt);
/*DEBUG: fprintf(stderr, "linkname before adding is '%s'\n", strt);*/
           if (strstr(strt, "mailto:") == strt || strstr(strt, "javascript:") == strt ||
               strstr(strt, "http://") != NULL || strstr(strt, "https://") != NULL ||
               strchr(strt, '?') != NULL || strchr(strt, '*') != NULL)
             continue; /* takogo vida ssilki ne nuzhni */
#ifdef HTML_URI
           if (strrchr(linkname, '#')) *strrchr(linkname, '#') = 0; /*rezat' ssilki vnutri odnogo dokumenta*/
           if (strlen(linkname))
           {
#endif
#ifdef __TURBOC__
  	          add_new_link_2_filelist(path, strt); /*stroka v nozhnem registre <awdeew: dangerous!!!*/
#else
             add_new_link_2_filelist(path, linkname);
#endif
           }
         }
       }
     }
     else /*eto vot chto:  if(sim<>'<')*/
     {
       if (is_separator(sim)) /*slovo konchilos'*/
       {
         if (strlen(lex) > MIN_LEN_OF_LEXEM) /*korotkie slova budem ignorirovat'*/
         {

/*DEBUG: fprintf(std_err, "raw lex = %s\n", lex);*/
            if (strlen(lex) >= LEN_LEXEM) i = LEN_LEXEM - 1;
            lex[i] = 0;
            convert_2_uppercase(lex);
            place_lex_to_tree(lex); /*zapomnim slovo*/
         }
         i = lex[0] = 0;
       }
       else if (!F && T) /*esli (ne zapresheno i mozhno) to sohranim simvol dlya analiza*/
	    {
	      if (strlen(lex)+1 < LEN_LEX_NEW) {
	        lex[i++] = sim;
	        lex[i] = 0;
	      }
	      else
	      {
/*DEBUG: fprintf(std_err, "lex '%s' is full!\n", lex);*/
	      }
	    }
     }
  }
  return 0;
}
/*-------------------------------------*/
/*konvertaciya v nizhniy registr (nuzhno dlya sravneniya tegov)*/
void str_tolower(char *str)
{
  int i = 0, len = strlen(str);
  for ( ; i < len; i++)
    str[i] = tolower(str[i]);
}
/*-------------------------------------*/
/*schitaem teg v peremennuyu 'tag'*/
char read_tag(FILE* source)
{
/*DEBUG: fprintf(stderr, "read_tag() is invoked. tag='%s' and separators is %s'\n", tag, separators);*/
  char sim=' ', r = 0, end = 0;
  int i = 0, j = 0;
  while (is_separator(sim) && !feof(source))
    sim = fgetc(source); /*schitaem razdeliteli mezhdu '<' i nachalom tega*/
  if (sim=='/') r = sim; /*schitali priznak zakryvayushego tega*/
  else tag[i++] = sim; /*schitali nachalo tega*/
/*DEBUG: fprintf(stderr, "read_tag() reads sim '%c' and is_separator(sim) = %d\n", sim, is_separator(sim));*/
  for (sim=fgetc(source); sim != '>' && !feof(source); sim = fgetc(source))
  {
    if (!end)
    {
      if (is_separator(sim))
      {
        end = 1;
        tag[i] = 0;
      }
      else if (i < LEN_TAG - 1) tag[i++] = sim;
    }
    else /*sohranim atributi tega v otdel'nom bufere*/
      if (j < LEN_TAGINSIDE - 1) taginside[j++] = sim;
  }
  tag[i] = taginside[j] = 0; 
/* vozmozhno schitan ne ves' teg ili ego dlina == 'LEN_TAG' */
  str_tolower(tag); /* perevod v nizhniy registr */
  return r;
}
/*-------------------------------------*/
/*yavlyaetsya li schitannyy simvol razdelitelem?*/
is_separator(char sim)
{
  if (strchr(separators, sim)) return 1;
  return 0;
}
/*-------------------------------------*/
/*est' li zaproshennyy teg v massive razreshennyh tegov*/
is_true_tag(char* str1)
{
  int i;
  for (i = 0; i < LEN_ARRAY_LEXEM && lexem[i][0]; i++)
    if (!strcmp(lexem[i], str1)) return 1;
  return 0;
}
/*-------------------------------------*/
/*est' li zaproshennyy teg v massive zapreshennyh tegov*/
is_false_tag(char* str1)
{
  int i;
  for (i = 0; i < LEN_ARRAY_LEXEM && unlexem[i][0]; i++)
    if (!strcmp(unlexem[i], str1)) return 1;
  return 0;
}
/*-------------------------------------*/
/*perevod v nizhniy registr kuska stroki do znaka '='*/
int str_lwr(char* string)
{
   int length = strlen(string), i;
   for (i = 0; i < length && string[i] != '='; i++)
     if (isalpha(string[i])) string[i] = tolower(string[i]);
   return i;
}
/*-------------------------------------*/
/*dopustimie razdeliteli mezhdu atributami tega*/
is_mini_separator(char sim)
{
  static char mini_separators[] = " \"\t\n\r";
  if (strchr(mini_separators, sim)) return 1;
  return 0;
}
/*-------------------------------------*/
int is_teg_w_link(char* str1, char* href)
{
  if (!strcmp("a", str1)) /*strcasecmp() uzhe ne nado (k tomu zhe v win/dos ne rabotaet)*/
  {
    strcpy(href, "href");
    return 1;
  }
  else 
    if (!strcmp("frame", str1)) 
    {
      strcpy(href, "src");
      return 1;
    }
  return 0;
}
/*-------------------------------------*/
/*vichlenenie imeni ssilki iz tega '<a href = "...">' v stroku 'linkname'*/
parse_href(char* str1, char* linkname)
{
/*DEBUG: fprintf(stderr, "parse_href() is invoked with str1 '%s' and filename '%s'\n", str1, linkname);*/
  char href[5];
  if (is_teg_w_link(str1, href) /*&& NULL!=strstr(taginside, href)*/)
  {
    char *tag_ptr = taginside;
    unsigned int len = 0;
    if (!strlen(taginside))
      return 3; /*u tega net atributov*/
    while (*tag_ptr && is_separator(*tag_ptr))
      tag_ptr++;
/*DEBUG: fprintf(stderr, "tag_ptr after the strip is '%s'\n", tag_ptr);*/
    while (1) /*na sluchay, esli pered 'href' kakoy-to musor, opirayas' na znak "=" sdelaem neskol'ko prohodov*/
    {
      char *ptr;
      if (len >= strlen(tag_ptr)) break;
      len += str_lwr(tag_ptr + len) + 1;
      ptr = strstr(tag_ptr, href);
/*DEBUG: fprintf(stderr, "len=%d, ptr is '%s'\n", len, ptr);*/
      if (ptr)
      {
/*DEBUG: fprintf(stderr, "parse_href() found '%s' on '%s'\n", href, ptr);*/
        int i;

        if (strchr(ptr, '=')) ptr = strchr(ptr, '=') + 1;
        else return 1; /*<a href> s oshibkoy -- bez '=' :-?*/

        while (is_mini_separator(*ptr) && *ptr)
          ptr++;
/*DEBUG: fprintf(stderr, "ptr is '%s'\n", ptr);*/
        for (i = 0; !is_mini_separator(*ptr) && *ptr; ptr++)
        {
          if (i < MAXPATH - 1) linkname[i++] = *ptr;
          else return 255; /*slishkom dlinnoe imya fayla*/
        }
        linkname[i] = 0;
/*DEBUG: fprintf(stderr, "parse_href() found linkname = '%s'\n", linkname);*/
        return 0;
      }
    }
    return 1; /*missiya provalena :-)*/
  }
  return 2; /*missiya beznadyozhno provalena :-]*/
}
/*-------------------------------------*/
/*podhodit li leksema pod opredelenie leksemy: pravila sm. v funkcii*/
/*is_true_lexem(char*lex)
{
  char l2[LEN_LEX_NEW];
  if(strlen(lex)<MIN_LEN_OF_LEXEM)return 0;
  if(!strcmp(lex,ltoa(atol(lex),l2,10)))return 1;
  return 0;
}*/
/*-------------------------------------*/
