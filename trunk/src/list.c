/*---------rabota so spiskom-----------*/
/*-------------------------------------*/
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "../include/findw.h"
/*-------------------------------------*/
struct LIST *lst_curr = NULL, *lst_root = NULL;
/*-------------------------------------*/
void make_first_inlist(char*);
void insert_elem_inlist(char*);
int exist_inlist(char *);
/*-------------------------------------*/
/*sozdanie pervogo elementa spiska*/
void make_first_inlist(char *info)
{
  if (!(lst_root = malloc(sizeof(struct LIST)))) my_exit(NOT_ENOUGH_MEMORY, "mlist-1");
  if (!(lst_root->info = strdup(info))) my_exit(NOT_ENOUGH_MEMORY, "mlist-2");
  lst_root->pnext = NULL;
  lst_root->fl = 0;
  lst_curr = lst_root;
}
/*-------------------------------------*/
/*vklyuchenie elementa v spisok*/
void insert_elem_inlist(char *info)
{
  struct LIST* q;
  if (!(q = malloc(sizeof(struct LIST)))) my_exit(NOT_ENOUGH_MEMORY, "insertl-1");
  if (!(q->info = strdup(info))) my_exit(NOT_ENOUGH_MEMORY, "insertl-2");
  q->pnext = lst_curr->pnext;
  q->fl = 0;
  lst_curr->pnext = q;
}
/*-------------------------------------*/
/*sushestvuet li fayl v spiske*/
exist_inlist(char *linkname)
{
  struct LIST *pwork;
  for (pwork = lst_root; pwork; pwork = pwork->pnext)
    if (!strcmp(linkname, pwork->info)) return 1;
  return 0; /*eto noviy fayl*/
}
/*-------------------------------------*/
/*-------------------------------------*/
/*dobavit' v spisok imay fayla, na kotoroe est' ssilka iz tekushego*/
add_new_link_2_filelist(char* path, char* linkname)
{
/*DEBUG: fprintf(stderr, "add_new_link_2_filelist is invoked with path '%s' and linkname '%s'\n", path, linkname);*/
  static char* rel_name = NULL;
  static int rel_name_size = 0;
  char abs_name[PATH_MAX];
  char* ptr = rel_name;
  int new_size = strlen(path)+strlen(linkname)+1;

  if (rel_name_size < new_size || rel_name == NULL)
  {
    ptr = realloc(rel_name, new_size);
    rel_name_size = new_size;
    if(ptr == NULL)
    {
/*DEBUG:*/ fprintf(stderr, "add_new_link_2_filelist(): NOT_ENOUGH_MEMORY for '%d' bytes\n", rel_name_size);
      free(rel_name);
      rel_name_size = 0;
      rel_name = NULL;
      return 2; /* NOT_ENOUGH_MEMORY!!! */
    }
    else
      rel_name = ptr;
  }

  strcpy(rel_name, path);
  strcat(rel_name, linkname);
  if (!realpath(rel_name, abs_name))
  {
/*DEBUG:*/ fprintf(stderr, "add_new_link_2_filelist(): an error is contained in the filename '%s' or it isn't exist!\n", rel_name);
    return 1;
  }

/*DEBUG: fprintf(stderr, "add_new_link_2_filelist(): abs_name is '%s'\n", abs_name);*/
  if (!lst_root)
    make_first_inlist(abs_name);
  else
    if (!exist_inlist(abs_name))
      insert_elem_inlist(abs_name);

/*  free(rel_name); TODO: add 'rel_name' to GC and free this memory on program exit */
  return 0;
}
/*-------------------------------------*/
