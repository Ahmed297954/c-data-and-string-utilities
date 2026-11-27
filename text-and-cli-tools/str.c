/*
 * Author: Ahmed BOUAZZA
 * Description: Pointer-based implementations of str_chr, str_rchr, str_pbrk, and str_spn matching pre-C23 <string.h> specifications.
 */

#include "str.h"

extern char *str_chr(const char *s, int c){
    const char *p = s;
  //IB:
  //QC:
  while(*p != '\0'){
    if(*p == (char)c){
      return (char*)p;
    }
    ++p;
  }
  if((char)c == '\0'){
      return (char*)p;
    }
  return nullptr;
}

extern char *str_pbrk(const char *s1, const char *s2) {
  //IB:
  //QC:
   while(*s1 != '\0'){
      if(str_chr(s2,*s1) != nullptr){
       return (char*)s1;
      }
      ++s1;
    }
  return nullptr;
}

extern char *str_rchr(const char *s, int c){
  const char *last = nullptr;
  //IB:
  //QC:
  while(*s != '\0'){
    if(*s == (char)c){
      last = (char*)s;
    }
    ++s;
  }
  if((char)c == '\0'){
    return (char*)s;
  }
  return (char*)last;
}

extern size_t str_spn(const char *s1, const char *s2){
  const char *p = s1;
  //IB:
  //QC:
  while(*p != '\0'){
    if(str_chr(s2,*p) != nullptr){
      ++p;
    }else{
      break;
    }
  }

  return (size_t)(p-s1);
}


