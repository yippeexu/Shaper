
//#include <stddef.h>
#include <stdbool.h>
#include "stdint.h"
#include "string.h"

#define GetBytePtr(ptr) (uint8_t *)ptr;
#define GetSizePtr(ptr) (size_t *)ptr;

void *memcpy(void *dest, void* src, size_t count)
{
	uint8_t *srcb = GetBytePtr(srcb);
	uint8_t *destb = GetBytePtr(destb);
	for (int i = 0; i < count; i++)
	{
		destb[i] = srcb[i];
	}
}

int memcmp(void *ptr1, void *ptr2, size_t count)
{
	uint8_t *ptr1b = GetBytePtr(ptr1);
	uint8_t *ptr2b = GetBytePtr(ptr2);
	for (int i = 0; i < count; i++)
	{
		if (ptr1b[i] != ptr2b[i])
		{
			return false;
		}
	}
	return true;
}

void *memset(void *src, int value, size_t count)
{
	if (value > UINT8_MAX)
	{
		uint16_t* ptr = (uint16_t *)src;
		for (int i = 0; i < count; i++)
		{
			ptr[i] = value;
		}
	}
	else
	{
		uint8_t* ptr = (uint8_t *)src;
		for (int i = 0; i < count; i++)
		{
			ptr[i] = value;
		}
	}
	return src;
}

void *memclr(void *dest, size_t count)
{
	uint8_t *destb = GetBytePtr(dest);
	for (int i = 0; i < count; i++)
	{
		destb[i] = 0;
	}
	return dest;
}

void *memchr(void *dest, char chr, size_t count)
{
	uint8_t *destb = GetBytePtr(dest);
	for (int i = 0; i < count; i++)
	{
		if (destb[i] == chr)
		{
			destb += i;
			dest = destb;
			return dest;
		}
	}
	return NULL;
}

void *memmove(void *dest, void *src, size_t count)
{
	memcpy(dest, src, count);
}

string strcat(string dest, string src)
{
	string result;
	memcpy(result + lengthof(dest), src, lengthof(src));
	result[lengthof(result) + 1] = '\0';
	dest = result;
	return dest;
}

string strchr(string str, char chr)
{
	string result = memchr(str, chr, lengthof(str));
	return result;
}

int strcmp(string str1, string str2)
{
	for (int i = 0; i < lengthof(str2); i++)
	{
		if (str1[i] > str2[i])
		{
			return 1;
		}
		if (str1[i] < str2[i])
		{
			return -1;
		}
	}
	return 0;
}

int strcoll(string str1, string str2)
{
	strcmp(str1, str2);
}

string strcpy(string dest, string src)
{
	memcpy(dest, src, lengthof(dest));
	return dest;
}

size_t strcspn(string str1, string str2)
{
	for (int c = 0; c < lengthof(str1); c++)
	{
		for (int k = 0; k < lengthof(str2); k++)
		{
			if (str1[c] == str2[k])
			{
				return c;
			}
		}
	}
}

string strerror(int errornum)
{
	return "Error messages are not supported yet, To be supported";
}

size_t strlen(string str)
{
	size_t len;
	for (len = 0; str[len]; len++);
	return len;
}

string strncat(string dest, string src, size_t num)
{
	string result;
	memcpy(result + lengthof(dest), src, lengthof(src) - num);
	result[lengthof(result) + 1] = '\0';
	dest = result;
	return dest;
}

int strncmp(string str1, string str2, size_t num)
{
	for (int i = 0; i < lengthof(str2) - num; i++)
	{
		if (str1[i] > str2[i])
		{
			return 1;
		}
		if (str1[i] < str2[i])
		{
			return -1;
		}
	}
	return 0;
}

string strncpy(string dest, string src, size_t num)
{
	string result = strcpy(dest, src);
	memclr(result + lengthof(dest) + (lengthof(src) - num), num);
	dest = result;
	return dest;
}

string strpbrk(string str1, string str2)
{
	string result;
	int charIndex;
	for (int c = 0; c < lengthof(str1); c++)
	{
		for (int k = 0; k < lengthof(str2); k++)
		{
			if (str1[c] == str2[k])
			{
				result[charIndex] = str2[k];
				charIndex++;
			}
		}
	}
}

string strrchr(string str, char chr)
{
	int* buffer;
	size_t charindex;
	for (int i = 0; i < lengthof(str); i++)
	{
		if (str[i] == chr)
		{
			charindex = i;
		}
	}
	str += charindex;
	return str;
}

size_t strspn(string str1, string str2)
{
	size_t result;
	for (int c = 0; c < lengthof(str1); c++)
	{
		for (int k = 0; k < lengthof(str2); k++)
		{
			if (str1[c] == str2[k] && str1[c] != '\0')
			{
				result++;
			}
		}
	}
	return result;
}

string strstr(string str1, string str2)
{
	bool found = false;

	for (int i = 0; i < lengthof(str1); i++)
	{
		if (str1[i] == str2[0])
		{
			for (int c = 0; c < lengthof(str2); c++)
			{
				if (str1[i+c] != str2[c])
				{
					found = false;
					break;
				}
				else found = true;
			}
			if (found)
			{
				str1 += i;
				return str1;
			}
		}
	}
	return NULL;
}

string strtok(string str, string delimiters)
{
	bool notdelimiter = false;
	string buffer[512];
	int index = 0;
	
	for (int c = 0; c < lengthof(str); c++)
	{
		notdelimiter = true;
		for (int d = 0; d < lengthof(delimiters); d++)
		{
			if (str[c] == delimiters[d])
			{
				notdelimiter = false;
				break;
			}
		}
		if (notdelimiter)
		{
			int cc = c;
			for (; !notdelimiter; cc++)
			{
				for (int d = 0; d < lengthof(delimiters); d++)
				{
					if (str[cc] == delimiters[d])
					{
						notdelimiter = false;
						index++;
						break;
					}
				}
			}
			if (notdelimiter)
			{
				memcpy(buffer[index], str + c, cc);
				return str + c;
			}
		}
	}
}

string int2str(int integer)
{
	string result;
	int i = 0;

	bool isNegative = integer < 0;
	
	uint16_t integer0 = (isNegative ? -integer : integer);

	while (integer0 != 0)
	{
		result[i++] = (char)(integer0 % 10 + '0');
		integer0 /= 10;
	}

	if (isNegative)
		result[i++] = '-';

	result[i] = 0x00;

	for (int t = 0; t < i / 2; t++)
	{
		result[t] ^= result[i - t - 1];
		result[i - t - 1] ^= result[t];
		result[t] ^= result[i - t - 1];
	}

	if (integer == 0)
	{
		result[0] = '0';
		result[1] = 0x00;
	}

	return result;
}
char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7',
				'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

string hex2str(uint32_t integer, size_t length)
{
	string buffer;
	int internalinteger = integer;
	int len = 0;
	
	// Get the length of hex string
	do
	{
		len++;
		internalinteger >>= 4;
	} while (internalinteger != 0);
	
	// Fill ASCII zeros in the string
	for (int i = 0; i < length; i++)
		buffer[i] = '0';

	if (integer == 0)
	{
		buffer[length] = '\0';
		return buffer;
	}
	
	// Put the hex string in the string
	internalinteger = integer;
	int index = length - len;
	do
	{
		buffer[index] = hex[internalinteger & 0xF];
		index++;
		internalinteger >>= 4;
	} while (internalinteger != 0);

	for (int t = 0; t < len / 2; t++)
	{
		buffer[t] ^= buffer[len - t - 1];
		buffer[len - t - 1] ^= buffer[t];
		buffer[t] ^= buffer[len - t - 1];
	}

	// Null-terminator
	buffer[length] = '\0';

	return buffer;
}