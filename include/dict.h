#ifndef _DICT_H
#define _DICT_H

#define PATTERN_LOCATION "%7u=%3u;"
#define PATTERN_LENGTH 12

void search_word(char*);
void create_dict(int);
void generate_word_file(int);
void save_word_2_file(int, char*, unsigned, char);
void open_word_file(char);
char* format_word(char*, unsigned, char);
unsigned no_this_file(char*);

#endif // _DICT_H

