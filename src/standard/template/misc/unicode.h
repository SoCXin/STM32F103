#ifndef __UNICODE_H
#define __UNICODE_H

//#include <assert.h>
#include "stm32f10x.h"
#include <stdio.h>
//#include "stdio.h"
//#include <stdlib.h>

int enc_utf8_to_unicode_one(const unsigned char* pInput, u32 *Unic);
void GBK_STRING(char *p,u8 Outstring[]);
char IsTextUTF8(char* str)  ;

#endif

