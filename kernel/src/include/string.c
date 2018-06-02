
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <heap.h>

#define GetBytePtr(ptr) (uint8_t *)ptr;
#define GetSizePtr(ptr) (size_t *)ptr;

extern void asm_memcpy(void *dest, void *src, size_t count);
extern void asm_memset(void *dest, int value, size_t count);

void *memcpy(void *dest, void *src, size_t count)
{
	uint8_t *destb = (uint8_t *)dest;
	uint8_t *srcb = (uint8_t *)src;
	for (int i = 0; i < count; i++)
	{
		destb[i] = srcb[i];
	}

	//uint32_t *destl = (uint32_t *)dest, *srcl = (uint32_t *)src;
	//uint8_t *destb = (uint8_t *)dest, *srcb = (uint8_t *)src;
	//if (count % 4) {
	//	for (int i = 0; i < (count - (count % 4)) / 4; i++)
	//		*destl++ = *srcl++;
//
	//	destb = destl;
	//	srcb = srcl;
//
	//	for (int i = 0; i < count % 4; i++)
	//		*destb++ = *srcb++;
	//} else {
	//	for (int i = 0; i < count/4; i++)
	//		*destl++ = *srcl++;
	//}
	return dest;
}

void *kmemcpy(void *dest, void *src, size_t count)
{
	asm_memcpy(dest, src, count);

	return dest;
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

void *memset(void *dest, int value, size_t count)
{
	if (value >= UINT8_MAX)
	{
		uint16_t *ptr = (uint16_t *)dest;
		for (int i = 0; i < count; i++)
		{
			ptr[i] = value;
		}
	}
	else
	{
		uint8_t *ptr = (uint8_t *)dest;
		for (int i = 0; i < count; i++)
		{
			ptr[i] = value;
		}
	}
	return dest;
	
	//uint32_t *destl = (uint32_t *)dest;
	//uint8_t *destb = (uint8_t *)dest;
	//if (count % 4) {
	//	for (int i = 0; i < (count - (count % 4)) / 4; i++)
	//		*destl++ = value | value << 8 | value << 16 | value << 24;
//
	//	destb = destl;
//
	//	for (int i = 0; i < count % 4; i++)
	//		*destb++ = value;
	//} else {
	//	for (int i = 0; i < count/4; i++)
	//		*destl++ = value | value << 8 | value << 16 | value << 24;
	//}
}

void *kmemset(void *dest, int value, size_t count)
{
	asm_memset(dest, value, count);
	
	return dest;
}

void *memclr(void *dest, size_t count)
{
	uint8_t *destb = (uint8_t *)dest;
	for (int i = 0; i < count; i++)
	{
		destb[i] = 0;
	}
	
	//uint32_t *destl = (uint32_t *)dest;
	//uint8_t *destb = (uint8_t *)dest;
	//if (count % 4) {
	//	for (int i = 0; i < (count - 4) / 4; i++)
	//		*destl++ = 0;
//
	//	destb = destl;
	//		
	//	for (int i = 0; i < count % 4; i++)
	//		*destb++ = 0;
	//} else {
	//	for (int i = 0; i < count/4; i++)
	//		*destl++ = 0;
	//}
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

char *strcat(char *dest, char *src)
{
	char *result;
	memcpy(result + lengthof(dest), src, lengthof(src));
	result[lengthof(result)] = '\0';
	dest = result;
	return dest;
}

char *strchr(char *str, char chr)
{
	char *result = memchr(str, chr, lengthof(str));
	return result;
}

int strcmp(char *str1, char *str2)
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

int strcoll(char *str1, char *str2)
{
	strcmp(str1, str2);
}

char *strcpy(char *dest, char *src)
{
	memcpy(dest, src, lengthof(dest));
	return dest;
}

size_t strcspn(char *str1, char *str2)
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

char *strerror(int errornum)
{
	return "Error messages are not supported yet, To be supported";
}

size_t strlen(char *str)
{
	size_t len;
	for (len = 0; str[len]; len++);
	return len;
}

char *strncat(char *dest, char *src, size_t num)
{
	char *result;
	memcpy(result + lengthof(dest), src, lengthof(src) - num);
	result[lengthof(result) + 1] = '\0';
	dest = result;
	return dest;
}

int strncmp(char *str1, char *str2, size_t num)
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

char *strncpy(char *dest, char *src, size_t num)
{
	char *result = strcpy(dest, src);
	memclr(result + lengthof(dest) + (lengthof(src) - num), num);
	dest = result;
	return dest;
}

char *strpbrk(char *str1, char *str2)
{
	char *result;
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

char *strrchr(char *str, char chr)
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

size_t strspn(char *str1, char *str2)
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

char *strstr(char *str1, char *str2)
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

char *strtok(char *str, char *delimiters)
{
	bool notdelimiter = false;
	char *buffer[512];
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

char *int2str(int32_t integer)
{
	static char result[15];
	int i = 0;

	bool is_negative = integer < 0;
	
	int32_t integer0 = (is_negative ? -integer : integer);

	while (integer0 != 0)
	{
		result[i++] = (char)(integer0 % 10 + '0');
		integer0 /= 10;
	}

	if (is_negative)
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

int32_t str2int(char *integer_str)
{
	int32_t integer = 0;
	size_t len = 0;

	for (len = 0; len < lengthof(integer_str); len++)
		if (integer_str[len] >= 0x3A && integer_str[len] <= 0x2F) break;

    for (int i = 0; i < len; i++)
		integer = (integer * 10) + integer_str[i]-0x30;
	
	return integer;
}

char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7',
				'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

char *hex2str(uint32_t integer, size_t length)
{
	static char buffer[15];
	uint32_t internalinteger = integer;
	int len = 0;
	
	// Get the length of hex char *
	do
	{
		len++;
		internalinteger >>= 4;
	} while (internalinteger != 0);
	
	if (!length) length = len;
	
	// Fill ASCII zeros in the char *
	for (int i = 0; i < length; i++)
		buffer[i] = '0';

	if (integer == 0)
	{
		buffer[length] = '\0';
		return buffer;
	}
	
	// Put the hex char *in the char *
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