#ifndef _ANALIZ_H
#define _ANALIZ_H

void convert_directory_separators(char*);
int split_path_f_filename(char*, char*);
int false_file_type(char*);
int analiz_file(char*);
void str_tolower(char*);
char read_tag(FILE*);
int is_separator(char);
int is_true_tag(char*);
int is_false_tag(char*);
int str_lwr(char*);
int is_mini_separator(char);
int is_teg_w_link(char*, char*);
int parse_href(char*, char*);

#endif // _ANALIZ_H
