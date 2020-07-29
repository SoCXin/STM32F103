#include "unicode.h"

int enc_get_utf8_size(const unsigned char pInput)
{
		unsigned char c = pInput;
		// 0xxxxxxx ??0
		// 10xxxxxx ???
		// 110xxxxx ??2
		// 1110xxxx ??3
		// 11110xxx ??4
		// 111110xx ??5
		// 1111110x ??6
		if(c< 0x80) return 0;
		if(c>=0x80 && c<0xC0) return -1;
		if(c>=0xC0 && c<0xE0) return 2;
		if(c>=0xE0 && c<0xF0) return 3;
		if(c>=0xF0 && c<0xF8) return 4;
		if(c>=0xF8 && c<0xFC) return 5;
		if(c>=0xFC) return 6;
}

int enc_utf8_to_unicode_one(const unsigned char* pInput, u32 *Unic)
{
//    assert(pInput != NULL && Unic != NULL);

    // b1 ??UTF-8???pInput?????, b2 ??????, ...
    char b1, b2, b3, b4, b5, b6;

    *Unic = 0x0; // ? *Unic ??????
    int utfbytes = enc_get_utf8_size(*pInput);
    unsigned char *pOutput = (unsigned char *) Unic;

    switch ( utfbytes )
    {
        case 0:
            *pOutput     = *pInput;
            utfbytes    += 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80 )
                return 0;
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                    || ((b6 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}

void GBK_STRING(char *pointer,u8 Outstring[])
{
    unsigned long gbknum;
	  gbknum=(*pointer<<8)+*(pointer+1);
	  sprintf((char*)Outstring,"%d",(int)gbknum);
}



char IsTextUTF8(char* str)  
{  
    u8 nBytes=0;//UFT8??1-6?????,ASCII?????  
    u8 chr;  
 
     
    chr= *(str);  
	
    if(chr>=0x80)  
    {  
        if(chr>=0xFC&&chr<=0xFD)  
            nBytes=6;  
        else if(chr>=0xF8)  
            nBytes=5;  
        else if(chr>=0xF0)  
            nBytes=4;  
        else if(chr>=0xE0)  
            nBytes=3;  
        else if(chr>=0xC0)  
            nBytes=2;  
    
        return nBytes;

     }  
   
     else 
     {  
         if( (chr&0xC0) != 0x80 )  
           return 1;  
     }  
     

}



