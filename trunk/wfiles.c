/*---------rabota s faylami------------*/
/*-------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "findw.h"
#include "fnmatch.h"
#include "platform.h"
/*-------------------------------------*/
extern FILE* filelist;
extern char lexem[LEN_ARRAY_LEXEM][LEN_TAG];
extern char unlexem[LEN_ARRAY_LEXEM][LEN_TAG];
extern char separators[256];/*po chislu simvolov*/
char spath[MAXPATH + 1]/*maska*/,sdir[MAXPATH + 1];
extern FILE *std_err;
extern char cfgdir[MAXPATH];
/*-------------------------------------*/
load_separators()
{
/*DEBUG: fprintf(stderr, "load_separators() is invoked\n");*/
  FILE* psep;
  int sim=1, i=0;
  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_SEPARATORS);
/*DEBUG: fprintf(stderr, "filename is '%s'\n", filename); */
  if (!(psep = fopen(filename, "rt")))
    error_open_file(filename, 17);
#ifdef _MSDOS
  separators[i++] = 13;
#endif
  while(EOF != (sim = fgetc(psep)))
    if(!strchr(separators, (char)sim))
    {
      separators[i++] = (char)sim;
      separators[i] = 0;
/*DEBUG: fprintf(stderr, "\n%d",sim);*/
    }
  fclose(psep);
/*DEBUG:delay(2000);*/
}
/*-------------------------------------*/
open_filelist(void)
{
/*DEBUG: fprintf(stderr, "open_filelist() is invoked\n");*/
  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_FILES);
  if (!(filelist=fopen(filename, "rt")))
    error_open_file(filename, 1);
  else return 0;
  return ERROR_OPEN_FILE;
}
/*-------------------------------------*/
char* take_filename_f_file(void)
{
/*DEBUG: fprintf(stderr, "take_filename_f_file() is invoked\n");*/
  char* pstr1 = NULL, str1[MAXPATH + 1] = "";
/*  fscanf(filelist, "%s", str1);*/
  fgets(str1, MAXPATH + 1, filelist);
  if (str1[strlen(str1) - 1] == '\n') str1[strlen(str1) - 1] = 0;
/*DEBUG: fprintf(std_err, "take_filename_from_file(): %s (len = %d)\n", str1, strlen(str1));*/
  pstr1 = str1;
  if (!strlen(str1) || feof(filelist))
  {
    fclose(filelist);
    return (char*)NULL;
    my_exit(EXIT_ON_END_FILELIST, "unreahable code");/*exit WHERE! - awdeew*/
  }
  return pstr1;
}
/*-------------------------------------*/
void load_lexems(char* filename, char type)
{
/*DEBUG: fprintf(stderr, "load_lexems() is invoked with file '%s' and type '%c'\n", filename, type);*/
  FILE* lexemfile;
  int i=0;
  char str1[MAXPATH];
  if(!(lexemfile=fopen(filename,"rt")))error_open_file(filename,2);
  while(1)
  {
    if(!fscanf(lexemfile,"%s",str1));/*break;*/ /*kazhetsya eto ne nuzhno*/
    if(feof(lexemfile))break;
    if(strlen(str1)>LEN_TAG)*(str1+LEN_TAG-1)=0;/*obrezhem slishkom dlinnye leksemy*/
    str_tolower(str1); /* konvertiruem v nizhniy registr */
    if(type=='L')strcpy(lexem[i++],str1);/*iz kakogo fayla chitaem*/
    else strcpy(unlexem[i++],str1);
    if(i>=LEN_ARRAY_LEXEM)break;/*massiv leksem perepolnen*/
  }
  fclose(lexemfile);
}
/*-------------------------------------*/
void take_dir(void)
{
/*DEBUG: fprintf(stderr, "take_dir() is invoked\n");*/
  convert_directory_separators(spath); /*znaki '\\' v znaki '/'*/
  strcpy(sdir, spath);
/*nayti posledniy razdelitel' katalogov i obrezat' posle nego stroku*/
  if (strrchr(sdir, '/'))
  {
    *((strrchr(sdir, '/')) + 1) = 0;
    strcpy(spath, (strrchr(spath, '/') + 1));
  }
  else strcpy(sdir, "./");
}
/*-------------------------------------*/
char *scan_dir(char *dirname, char *mask)
{
/*DEBUG: fprintf(stderr, "scan_dir() is invoked with dirname '%s' and mask '%s'\n", dirname, mask);*/
  static DIR *dir = NULL;
  struct dirent *ent = NULL;
/*DEBUG: fprintf(std_err, "DIR=%s, MASK=%s\n", dirname, mask);*/
  if (!dir)
    if (!(dir = opendir(dirname))) return NULL;
  while (1)
  {
    ent = readdir(dir);
    if (!ent)
    {
      closedir(dir);
      dir = NULL;
      return (char*)NULL;
    }
/*DEBUG: fprintf(std_err, "file %s%s\n", dirname, ent->d_name);*/
#ifdef __TURBOC__
    if (!(fnmatch(mask, ent->d_name, FNM_PERIOD | FNM_CASEFOLD))) /*dlya chistogo DOS'a <awdeew:-)*/
#else
    if (!(fnmatch(mask, ent->d_name, FNM_PERIOD)))/*fayl podhodit pod masku*/
#endif
    {
/*DEBUG:*/ fprintf(std_err, "found file %s%s\n", dirname, ent->d_name);
      return ent->d_name;
    }
  }  
}
/*-------------------------------------*/
char* take_file(void)
{
/*DEBUG: fprintf(stderr, "take_file() is invoked\n");*/
  char str1[MAXPATH], filename1[MAXPATH], *ptr;
  if (!(strlen(spath)))/*posledniy fayl po etoy maske nayden v proshlom vyzove*/
  {
    if (!(ptr = take_filename_f_file())) /*poisk iz fayla zavershyen*/
      return BROKEN_SEARCH;
    strcpy(spath, ptr);/*vzyat' sleduyuchuyu masku poiska*/
    take_dir();/*vychlenit' iz maski katalog ('sdir')*/
  }
/*DEBUG: fprintf(std_err,"before scandir\n");*/
  if (!(ptr = scan_dir(sdir, spath))) 
    strcpy(filename1, "");
  else 
    strcpy(filename1, ptr);
/*DEBUG: fprintf(std_err,"after scandir\n");*/
  if (!strlen(filename1))
  {
    strcpy(spath, "");
    return (char*)NULL;
  }
  strcpy(str1, sdir);
  strcat(str1, filename1);/*vozvratit' imya fayla i put' k nemu (polnyy ili otnositel'nyy)*/
  if (strlen(str1)) ptr = str1;
  else ptr = NULL;
  return ptr;
}

