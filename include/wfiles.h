#ifndef _WFILES_H
#define _WFILES_H

void load_separators();
int open_filelist();
char* take_filename_f_file();
void load_lexems(char*, char);
void take_dir();
char* scan_dir(char*, char*);
char* take_file();

#endif // _WFILES_H
