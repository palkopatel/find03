#ifndef _LIST_H
#define _LIST_H

struct LIST {
       char fl;
       char* info;
       struct LIST* pnext;
};

void make_first_inlist(char*);
void insert_elem_inlist(char*);
int exist_inlist(char *);
int add_new_link_2_filelist(char*, char*);

#endif // _LIST_H
