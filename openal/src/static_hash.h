/* static_hash.h
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
#ifndef STATIC_HASH_H
#define STATIC_HASH_H

/*
 *	Static HASH algorithm implementation.
 *
 *	C does not allow strings to be used in switch/case constructions. One logical solution is to use an if else if construction with string compares. But
 *	that are very slow and in my application (an embedded system that parse XML data) this overhead was a problem.
 *
 *	I searched for a static HASH algorithm, but could not found an algorithm that could be used in a switch case construction. Basically because C expect
 *	a constant value behind a case, not a function call. On one site I found an almost static HASH algorithm, and the inspiration how to solve the issue.
 *
 *	The static HASH function defined here could only be used with character strings in its call. For example: HASH_S16(test) the value could not be a
 *	pointer to an array, or just a plain pointer. When the hash value from a generic C string is required, a separate function is available to calculate
 *	the HASH.
 *
 *	Example how to use:
 *
 *	swich (hash_string(*p))
 *	{
 *	case HASH_S16("test"):
 *		...
 *		break;
 *	case HASH_S256("An example with a long text!!!!!!!!!!!!!!!!!"):
 *		...
 *		break;
 *	}
 *
 *	Simple HASH functions like this one have a serious risk on collisions. I wrote a small C# test program to test if this will be a big issue. I load a
 *	couple of lists with words and some XSD files, 144928 different words in total, and found this collisions (I represent the words in the iso_8859_1
 *	character set):
 *	APPOINTEE	 FORELEGS
 *	HOOP		 referents
 *	REFLATION	 sociable
 *
 *	Lists:
 *	http://www.mieliestronk.com/corncob_caps.txt
 *	http://www.mieliestronk.com/corncob_lowercase.txt
 *	And 4 lists from (in English, Dutch, German and French):
 *	http://www.streetsmartlanguagelearning.com/2008/12/top-10000-words-in-dutch-english-french.html
 *
 *	So collisions with normal text strings are possible but happen not too often. In my case, no problem at all because the switch case construction give
 *	compile time errors in case of equal values that could be fixed by doing a final string compare.
 *
 *	The HASH_S256 macro could work with strings up to 256 bytes, but my pre-processor work terrible slow with this macro. So I decide to also make a
 *	smaller one, HASH_S16 that take up to 16 byte. That work much, much faster.
 */
/*
 *	C# test program
 *
 *	HASH as used in C# program:

uint generateHash(byte[] str)
{
	uint hash = 0;
	uint i;
	for(i = 0; i < str.Length; ++i)
		hash = 65599 * hash + str[i];
	return hash ^ (hash >> 16);
}

 *
 *	Algorithm used to find the collisions:

char[] stringSeparators = new char[] {' ', ':', '<', '>', '.', ',', '\n', '\r', '\t'};
string[] inputdata = inputstr.Split(stringSeparators, StringSplitOptions.RemoveEmptyEntries);
Dictionary<uint, string> dict = new Dictionary<uint, string>();
Dictionary<string, string> collisions = new Dictionary<string, string>();

foreach (string inp in inputdata)
{
	uint hash;
	System.Text.Encoding iso_8859_1 = System.Text.Encoding.GetEncoding("iso-8859-1");
	byte[] s = iso_8859_1.GetBytes(inp);
	hash = generateHash(s);

	if (dict.ContainsKey(hash))
	{
		if (dict[hash] != inp)
		{
			if (!collisions.ContainsKey(inp))
			{
				collisions.Add(inp, dict[hash]);
			}
		}
	}
	else
		dict.Add(hash, inp);
}

 *	Dictionary collisions contains the list of collisions, inputdata is a string with all the word lists.
 */

/*
 *	Internal defines, loop unrolling for the static hash macro's.
 */
#define HASH_INT_LEN(s)			((sizeof(s)/sizeof(s[0])) - 1)
#define HASH_INT_1(s,i,x)		(x*65599u+(unsigned char)s[(i)<HASH_INT_LEN(s)?HASH_INT_LEN(s)-1-(i):HASH_INT_LEN(s)])
#define HASH_INT_4(s,i,x)		HASH_INT_1(s,i,HASH_INT_1(s,i+1,HASH_INT_1(s,i+2,HASH_INT_1(s,i+3,x))))
#define HASH_INT_16(s,i,x)		HASH_INT_4(s,i,HASH_INT_4(s,i+4,HASH_INT_4(s,i+8,HASH_INT_4(s,i+12,x))))
#define HASH_INT_64(s,i,x)		HASH_INT_16(s,i,HASH_INT_16(s,i+16,HASH_INT_16(s,i+32,HASH_INT_16(s,i+48,x))))
#define HASH_INT_256(s,i,x)		HASH_INT_64(s,i,HASH_INT_64(s,i+64,HASH_INT_64(s,i+128,HASH_INT_64(s,i+192,x))))


/*
 *	Assert macro.
 */
#define assert_static(e) \
	do { \
		enum { assert_static__ = 1/(e) }; \
	} while (0)



/*
 *	Hash macro's for external use.
 */
#define HASH_S256(s)			((unsigned int)(HASH_INT_256(s,0,0)^(HASH_INT_256(s,0,0)>>16)))	///< Hash value from string with a max length of 256 bytes.
#define HASH_S64(s)				((unsigned int)(HASH_INT_64(s,0,0)^(HASH_INT_64(s,0,0)>>16)))	///< Hash value from string with a max length of 64 bytes.
#define HASH_S16(s)				((unsigned int)(HASH_INT_16(s,0,0)^(HASH_INT_16(s,0,0)>>16)))	///< Hash value from string with a max length of 16 bytes.


// TODO: Make it work with g++ for android
/*#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<strlen(s)?strlen(s)-1-(i):strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))

#define HASH(s)    ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))
#define HASH_S16(s) HASH(s)
#define HASH_S64(s) HASH(s)
*/

/*uint64_t constexpr mix(char m, uint64_t s);
uint64_t constexpr hash_string(const char * m);

#define HASH_S16(s) hash_string(s)
#define HASH_S64(s) hash_string(s)
*/

/**
 *	\brief	Hash function
 *
 *	This HASH function produce the same hash value as the static hash functions HASH_S256 and HASH_S16. This function can be used on
 *	any character string/pointer.
 *
 *	\param	String is a pointer to a null terminated character string.
 *	\return	HASH value.
 */
unsigned int hash_string (const char *String);

#endif // STATIC_HASH_H
