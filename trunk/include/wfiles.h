#ifndef _WFILES_H
#define _WFILES_H

void load_separators();
FILE* open_filelist();
char* take_filename_f_file(FILE*);
void load_lexems(char*, char);
void take_dir();
char* scan_dir(char*, char*);
char* take_file(FILE*);

#endif // _WFILES_H
