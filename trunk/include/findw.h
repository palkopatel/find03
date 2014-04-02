#ifndef _FINDW_H
#define _FINDW_H
/*------------------------------------*/
#define HTML_URI 1 /*rezat' ssilki vnutri odnogo dokumenta*/
/*------------------------------------*/
#define MAXPATH 1024
/*#define TMPFILE(str) mkstemp2(str)*/
/*------------------------------------*/
enum {
  ERROR_OPEN_FILE = 1,    /* 1 */
  EXIT_ON_END_FILELIST,   /* 2*/
  NOT_ENOUGH_MEMORY,      /* 3 */
  TOO_MANY_FILES,         /* 4 */
  BAD_FILE_TYPE,          /* 5 */
  FILE_IS_DIRECTORY,      /* 6 */
};

enum {
  EOF_CODE_1 = 1,
  EOF_CODE_2,
  EOF_CODE_3,
  EOF_CODE_5 = 5,
  EOF_CODE_6,
  EOF_CODE_7,
  EOF_CODE_8,
  EOF_CODE_9,
  EOF_CODE_11 = 11,
  EOF_CODE_12,
  EOF_CODE_13,
  EOF_CODE_14,
  EOF_CODE_15,
  EOF_CODE_16,
  EOF_CODE_17,
  EOF_CODE_31 = 31,
  EOF_CODE_32,
};

#define LEN_ARRAY_LEXEM 100
#define LEN_TAG 15
#define LEN_LEXEM 20
#define MIN_LEN_OF_LEXEM 3
#define MAX_NUM_ONE_LEXEM 64
#define HASH_ARRAY_SEPARATOR (0xff/LEN_LEXEM)
#define LEN_FILE_W_WORD 0xffffe
#define LEN_FILE_W_PATH LEN_FILE_W_WORD
/* skol'ko znakov analizirovat' dlya opredeleniya tipa fayla (fayl 'analiz.c') */
#define BINARY_PROBE_LENGHT 255
/* dopustimoe chislo binarnyh simvolov */
#define BINARY_SIMBOLS_LIMIT 1
/* chislo popitok pri otkritii faylov (ne realizivano -- variant smotri v fayle 'bintree.c' funkciya 'save_tree_2_file(...)') */
#define NUM_TRIAL 4
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

/*-------------------------------------*/
void remove_log(int);
void error_open_file(char*, int);
void create_tempfiles();
void my_exit(int, char*);
#endif
