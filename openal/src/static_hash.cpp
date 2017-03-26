/* static_hash.c
 *
 * Copyright (c) 2015 by Lukas S. van der Heeden
 * All rights reserved.
 *
 * Use license: Modified from standard BSD license.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation, advertising
 * materials, Web server pages, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Lukas S. van der Heeden. The name "Lukas S. van der Heeden" may
 * not be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ''AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "static_hash.h"

/**
 *	\brief	Hash function
 *
 *	This HASH function produce the same hash value as the static hash functions HASH_S256 and HASH_S16. This function can be used on
 *	any character string/pointer.
 *
 *	\param	String is a pointer to a null terminated character string.
 *	\return	HASH value.
 */
unsigned int hash_string (const char *String)
{
    unsigned int hash = 0, i;
	assert_static(sizeof(unsigned int) == 4);
	assert_static(sizeof(unsigned char) == 1);

    for(i = 0; String[i]; ++i)
        hash = 65599 * hash + String[i];
    return hash ^ (hash >> 16);
}

//C++11
/*
uint64_t constexpr mix(char m, uint64_t s) {
  return ((s<<7) + ~(s>>3)) + ~m;
}

uint64_t constexpr hash_string(const char * m) {
  return (*m) ? mix(*m,hash_string(m+1)) : 0;
}*/
