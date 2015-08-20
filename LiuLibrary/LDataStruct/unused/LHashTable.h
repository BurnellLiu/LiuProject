#ifndef _DATASTRUCT_LHASHTABLE_H_
#define _DATASTRUCT_LHASHTABLE_H_

class LHashTable
{
public:
	static unsigned int BKDRHash(char* str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;

		while (*str)
		{
			hash = hash * seed + (*str++);
		}

		return (hash & 0x7FFFFFFF);
	}
};
#endif