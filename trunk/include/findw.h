#ifndef _FINDW_H
#define _FINDW_H
/*------------------------------------*/
#define HTML_URI 1 /*rezat' ssilki vnutri odnogo dokumenta*/
/*------------------------------------*/
#define MAXPATH 1024
/*#define TMPFILE(str) mkstemp2(str)*/
/*------------------------------------*/
#define ERROR_OPEN_FILE 1
#define EXIT_ON_END_FILELIST 2
#define NOT_ENOUGH_MEMORY 3
#define TOO_MANY_FILES 4
#define BAD_FILE_TYPE 5
#define FILE_IS_DIRECTORY 6

#define LEN_ARRAY_LEXEM 100
#define LEN_TAG 15
#define LEN_LEXEM 20
#define MIN_LEN_OF_LEXEM 3
#define MAX_NUM_ONE_LEXEM 64
#define HASH_ARRAY_SEPARATOR (0xff/LEN_LEXEM)
#define LEN_FILE_W_WORD 0xffffe
#define LEN_FILE_W_PATH LEN_FILE_W_WORD
#define LEN_PROBE 255 /* skol'ko znakov analizirovat' dlya opredeleniya tipa fayla (fayl 'analiz.c') */
#define NUM_TRIAL 4 /* chislo popitok pri otkritii faylov (ne realizivano -- variant smotri v fayle 'bintree.c' funkciya 'save_tree_2_file(...)') */
#define LEN_META_DESC 10

#define FILE_W_FILES "1filelst"
#define FILE_W_LEXEM "1lexemfl"
#define FILE_W_UNLEXEM "1unknwnl"
#define FILE_W_SEPARATORS "1separat"
#define FILE_W_HREFS "1hrefs"
#define FILE_W_TEMPFILES "2tmpfile"
#define FILE_W_WORD "3words"
/*#define FILE_W_PATH "3paths"*/
#define FILE_W_PREFIX "3prefix"
#define FILE_W_RESULTS "4res"

#define BROKEN_SEARCH "[a*a*1*0*a]"

/*FNLEN -- maksimal'naya dlina zapisi iz FILE_W_TEMPFILES*/
#define RFLEN (2 * MAXPATH + 2 + 10)

#define CREATE_L 48
#define CREATE_R 49
#define GOTO_L 50
#define GOTO_R 51
#define GOTO_F 52
/*-------------------------------------*/
void remove_log(int);
void error_open_file(char*, int);
void create_tempfiles();
void my_exit(int, char*);
#endif
