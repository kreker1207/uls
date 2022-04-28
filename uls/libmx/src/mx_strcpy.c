#include "../inc/libmx.h"

char *mx_strcpy(char *dst, const char *src) {
    	int count = 0;
  	  while(*src) {
  	      *dst++ = *src++;
  	      count++;
  	  }
  	  while (*dst) {
  	      *dst++ = 0;
  	      count++;
  	  }
  	  dst -= count;
     return dst;
}

