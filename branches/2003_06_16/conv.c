/*---------konvertirovanie-------------*/
/*-------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/*-------------------------------------*/
unsigned char rus_toupper(unsigned char sim)
{
  if (sim == 184) return 168; /* '¸' -> '¨'*/
  if (sim == 163) return 179; /* '£' -> '³'*/
  if (sim > 223)  return (sim - 32);
  return sim;
}
/*-------------------------------------*/
unsigned char is_rus_alpha(unsigned char sim)
{
  /*    'À'          'ÿ'               '¨'           '¸'             '³'             '£'    */
  if ((sim > 191 && sim <= 255) || (sim == 168) || (sim == 184) || (sim == 179) || sim == 163)
    return 1;
  return 0;
}
/*-------------------------------------*/
void convert_2_uppercase(char *str)
{
  int i, len = strlen(str);
  for (i = 0; i < len; i++)
  {
    if (is_rus_alpha(str[i]))
      str[i] = rus_toupper(str[i]);
    else 
      if (isalpha(str[i]))
        str[i] = toupper(str[i]);
  }
}
/*-------------------------------------*/
