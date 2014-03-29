#ifndef _BT_H_
#define _BT_H_

typedef struct BT{
       char *info/*[LEN_LEXEM + 1]*/;
       char counter;
       struct BT *pFather, *pLeft, *pRight;
}BinTree;

int MakeTree(char*);
int SetLeft(char*);
int SetRight(char*);
char* create_and_goto(char, char*);
int place_lex_to_tree(char*);
int save_tree_2_file(char*, unsigned);
int round_tree(BinTree*, FILE*);

#endif // _BT_H_
