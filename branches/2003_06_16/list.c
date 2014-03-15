/*---------rabota so spiskom-----------*/
/*-------------------------------------*/
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "findw.h"
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
/*inversiya stroki*/
void str_rev(char *str)
{
  int i, len = strlen(str), len2;
  char sim;
  len2 = len-- / 2;
  for (i = 0; i < len2; i++)
  {
    sim = str[i];
    str[i] = str[len - i];
    str[len - i] = sim;
  }    
}
/*-------------------------------------*/
/*dobavit' v spisok imay fayla, na kotoroe est' ssilka iz tekushego*/
add_new_link_2_filelist(char* path, char* linkname)
{
  char *ptr1 = strstr(linkname, "./"),
       *pathdup = strdup(path),
       *ptr2;
/*DEBUG: fprintf(stderr, "before adding %s <--> %s\n", linkname, path);*/
  if (!pathdup) return 2; /*NOT_ENOUGH_MEMORY!!!*/
  /*else */ptr2 = /*strstr(pathdup, "./")*/pathdup;
  if (ptr1)
    if (ptr1 == linkname) /*if (ptr1 - 1 != linkname)*/
      strcpy(linkname, linkname + 2); /* vikinem priznak tekushego kataloga ("./")*/
  if (strlen(ptr2) > 1)
    if (ptr2[0] == '.' && ptr2[1] == '/')
      ptr2 += 2; /*'pathdup' menyat' nel'zya, t.k. nado sdelat' free(pathdup)*/
/*  if (ptr2)
  {
   if (ptr2 == pathdup)
     ptr2 += 2;
  }
  else ptr2 = pathdup;*/
  str_rev(ptr2);
  while (1) /*pereborka otnositel'nogo puti v absolyutniy (koren' -- v startovom kataloge)*/
  {
/*DEBUG: fprintf(stderr, "inside while(1) %s <--> %s\n", linkname, ptr2);*/
    ptr1 = strstr(linkname, "../");
    if (!ptr1) break; /*razbor zavershyon*/
    ptr2 = strchr(ptr2, '/');
    if (ptr2) /*ptr1 zdes' vsegda istina*/
    {
      ptr2++;
      strcpy(linkname, ptr1 + 3);
    }
    else /*noviy put' vozmozhno s oshibkoy*/
    {
      free(pathdup); 
      return 1; 
    }
  }
/*DEBUG: fprintf(stderr, "after normalizing %s <--> %s\n", linkname, ptr2);*/
  if (ptr2)
  {
    char str1[MAXPATH], *pwork;
    int end = strlen(ptr2) - 1;
    str_rev(ptr2);
    if (ptr2[end] != '/')
      if (!(pwork = strrchr(ptr2, '/'))) goto NO_CHANGE;
      else *(pwork + 1) = 0;
    strcpy(str1, linkname);
    strcpy(linkname, ptr2);
    strcat(linkname, str1);
  }
/*DEBUG: fprintf(stderr, "after reversing %s <--> %s\n", linkname, ptr2);*/
NO_CHANGE:;
  if (!lst_root) make_first_inlist(linkname); 
  else 
    if (!exist_inlist(linkname))
      insert_elem_inlist(linkname);
/*DEBUG { struct LIST *pwork; for (pwork = lst_root; pwork; pwork = pwork->pnext) fprintf(stderr, "a-r m_f_inlist() %s (%d)\n", pwork->info, strlen(pwork->info)); }
*/
  free(pathdup);
  return 0;
}
/*-------------------------------------*/
