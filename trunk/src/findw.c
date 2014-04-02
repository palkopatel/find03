/*---------glavnyy faylik--------------*/
/*-------------------------------------*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/analiz.h"
#include "../include/bintree.h"
#include "../include/dict.h"
#include "../include/findw.h"
#include "../include/list.h"
#include "../include/wfiles.h"
/*-------------------------------------*/
FILE* workfile;
char lexem[LEN_ARRAY_LEXEM][LEN_TAG];
char unlexem[LEN_ARRAY_LEXEM][LEN_TAG];
char tempname[MAXPATH];
char cfgdir[MAXPATH];
/*-------------------------------------*/
extern struct LIST *lst_curr, *lst_root;
FILE *std_out = NULL, *std_err = NULL;
char FILE_4_LOG[MAXPATH] = "2log";
char FILE_4_ERRORS[MAXPATH] = "2errors";
/*-------------------------------------*/
void print_usage_message()
{
  fprintf(stderr,"\nusage: findw -i <path to cfg directory> -{c|d|f {words for search}}\
    \n\t-c\t\t\tcreate temporary bases from file '%s'\
    \n\t-d\t\t\tcreate dictinary (use after '-c')\
    \n\t-f {words for search}\tsearch words from temporary bases\n\n", FILE_W_FILES);
}
/* get path to cfg directory from app args */
int get_cfg_dir(int argc, char *argv[])
{
  int i=1;
  for (; i < argc; i++)
    if (strstr(argv[i], "-i") && i+1 < argc && NULL == strchr(argv[i+1], '-'))
    {
      if (!access(argv[i+1], W_OK))
      {
        return i+1;
      }
      else
      {
        fprintf(stderr, "\nWe don't have access to '%s'!\n", argv[i + 1]);
        return -1;
      }

    }
  return 0;
}
/*-------------------------------------*/
int main(int argc, char *argv[])
{
  int i;
  if (argc <= 1)
  {
    print_usage_message();
    return 1;
  }

  int index = get_cfg_dir(argc, argv);
  if (index < 0)
  {
    print_usage_message();
    return 2;
  }
  else if (!index)
    strcpy(cfgdir, ""); /* current dir is default cfg directory */
  else
    strcpy(cfgdir, argv[index]);

/*DEBUG: fprintf(stderr, "cfgdir is '%s'\n", cfgdir);*/

  for (i = 1; i < argc; i++)
    if (strstr(argv[i], "-l"))
    {
/*      if (i + 1 < argc)*/
/*        if (!strstr(argv[i + 1], "-d") && !strstr(argv[i + 1], "-c")) strcpy(FILE_4_LOG, argv[i + 1]);*/
      char filename[MAXPATH];
      strcpy(filename, cfgdir);
      strcat(filename, FILE_4_LOG);
      std_out = fopen(filename, "wt");
      break;
    }
  for (i = 1; i < argc; i++)
    if (strstr(argv[i], "-e"))
    {
      char filename[MAXPATH];
      strcpy(filename, cfgdir);
      strcat(filename, FILE_4_ERRORS);
      std_err = fopen(filename, "at");
      break;
    }
  if (!std_err) std_err = stderr;
  if (!std_out) std_out = stdout;
  for (i = 1; i < argc; i++)
    if (strstr(argv[i], "-c"))
    {
      create_tempfiles();
      remove_log(0);
      break;
    }
  for (i = 1; i < argc; i++)
    if (strstr(argv[i], "-d"))
    {
      int repeater;
      if (2 == strlen(argv[i]))
        if (i + 1 < argc) repeater = atoi(argv[i + 1]);
        else repeater = 1;
      else
        repeater = atoi(strcpy(argv[i], strstr(argv[i], "-d") + 2));
      if (!repeater) repeater = 1;
/*DEBUG:*/ fprintf(stderr, "main(): repeater is %d\n", repeater);
      create_dict(repeater);
      return 0;
    }
  for (i = 1; i < argc; i++)
    if (strstr(argv[i], "-f"))
      for (i++; i < argc; i++) search_word(argv[i]);
  return 0;
}
/*-------------------------------------*/
void mkstemp2()
{
  char *ptr;
  if(!(ptr=tmpnam(tempname)))error_open_file("tmpnam()", EOF_CODE_8);
}
/*-------------------------------------*/
int list_empty(void)
{
  for (lst_curr = lst_root; lst_curr; lst_curr = lst_curr->pnext)
    if (!lst_curr->fl) return 0; /*nayden ne obrabotanniy element*/
  lst_curr = lst_root; /*na vsyakiy sluchay...*/
  return 1;
}
/*-------------------------------------*/
void destroy_returns(char* str1)
{
  char *ptr;
  if ((ptr = strrchr(str1, '\n'))) *ptr = 0;
  if ((ptr = strrchr(str1, '\r'))) *ptr = 0;
}
/*-------------------------------------*/
void read_old_hrefs(void)
{
  FILE *hrefsfile;
  char str1[MAXPATH];
  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_HREFS);
  if ((hrefsfile = fopen(filename, "rt")))
  {
    if (fgets(str1, MAXPATH, hrefsfile))
    {
      if (!lst_root)
      {
        destroy_returns(str1);
        make_first_inlist(str1);
      }
      while (fgets(str1, MAXPATH, hrefsfile))
      {
        destroy_returns(str1);
        insert_elem_inlist(str1);
      }
    }
    fclose(hrefsfile);
  }
}
/*-------------------------------------*/
void working_list(void)
{
/*DEBUG: fprintf(stderr, "working_list() is invoked\n");*/
  unsigned i = 1;
  char str2[MAXPATH], str1[MAXPATH];
  int code;
/*DEBUG: for (lst_curr = lst_root; lst_curr; lst_curr = lst_curr->pnext) fprintf(stderr, "\t%s\n", lst_curr->info);*/
/*#ifdef FILE_W_HREFS*/
  read_old_hrefs();
/*#endif*/
  while (!list_empty())
  {
    strcpy(str1, lst_curr->info);
    if ((i = no_this_file(str1)) != 0) /*noviy li eto fayl*/
    {
      code = analiz_file(str1);
/*DEBUG: fprintf(stderr, "analiz_file() returned code '%d'\n", code);*/
      if (!code) /*mozhet bit' oshibka pri otkritii fayla*/
      {
        mkstemp2(str2);
        save_tree_2_file(str1, i);
      }
    }
    lst_curr->fl = 1; /*fayl obrabotan*/
  }
/*DEBUG: for (lst_curr = lst_root; lst_curr; lst_curr = lst_curr->pnext) fprintf(stderr, "\t%s\n", lst_curr->info);*/
}
/*-------------------------------------*/
void create_tempfiles(void)
{
/*DEBUG: fprintf(stderr, "create_tempfiles() is invoked\n");*/
  FILE* filelist;
  char str1[MAXPATH], str2[MAXPATH], *ptr;
  unsigned i = 1;
  load_separators();
  filelist = open_filelist();

  char filename[MAXPATH];
  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_LEXEM);
  load_lexems(filename, 'L');

  strcpy(filename, cfgdir);
  strcat(filename, FILE_W_UNLEXEM);
  load_lexems(filename, 'U');

  if (NULL == filelist) /*spisok s faylami ne otkrit -- poprobuem iz 'FILE_W_HREFS'*/
  {
    working_list();
    return;
  }
  while (1)
  {
    ptr = take_file(filelist);
/*DEBUG: fprintf(std_err, "after take_file\n");*/
    if (ptr)
      if (strstr(ptr, BROKEN_SEARCH))
      {
/*DEBUG: fprintf(std_err, "\nU have broken search\n");*/
        working_list();
        break;
      }
    if (ptr) strcpy(str1, ptr);
    else str1[0] = 0;
    if (!strlen(str1)) continue;
/*DEBUG: fprintf(std_err, "after misc\n");*/
    if (!(i = no_this_file(str1))) continue;
/*DEBUG: fprintf(std_err, "after no_this_file\n");*/
    if (!analiz_file(str1))
    {
      mkstemp2(str2);
/*DEBUG: fprintf(std_err, "after mktemp2\n");*/
      save_tree_2_file(str1, i);
/*DEBUG: fprintf(std_err, "after save_tree_2_file\n");*/
    }
/*DEBUG: fprintf(std_err, "after analiz_file\n");*/
  }
}
/*-------------------------------------*/
void remove_log(int c)
{
  if (std_out != stdout)
  {
    fclose(std_out);
    char filename[MAXPATH];
    strcpy(filename, cfgdir);
    strcat(filename, FILE_4_LOG);
    if (!c) remove(filename);
  }
}
/*-------------------------------------*/
void error_open_file(char* filename, int errorcod)
{
  int errsv = errno;
  fprintf(std_err, "\nerror %d: can't open file '%s'!\n", errorcod, filename);
  fprintf(std_err, "errno is %d. Message is '%s'\n", errsv, strerror(errsv));
/*                                       FILE_W_FILES  */
  if (errorcod != EOF_CODE_3 && errorcod != EOF_CODE_13 && errorcod != EOF_CODE_1 && errorcod != EOF_CODE_31 && errorcod != EOF_CODE_32)
    my_exit(ERROR_OPEN_FILE, "see up");
}
/*-------------------------------------*/
void my_exit(int error_code, char *fromf)
{
  int c = 0;
  fprintf(std_err, "\nIncorrect exit with code = %d\
  \n(see macro's value in ../include/findw.h)\
  \n(hint: func name = %s)\n", error_code, fromf);
  if (lst_root)
    for (c = 0, lst_curr = lst_root; lst_curr; lst_curr = lst_curr->pnext)
      if (!lst_curr->fl)
      {
        fprintf(std_out, "%s\n", lst_curr->info);
        c++;
      }
  remove_log(c);
  exit(error_code);
}
/*-------------------------------------*/
