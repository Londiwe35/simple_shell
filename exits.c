#include "shell.h"

/**
 **_strncpy - copies a string
 *@dest: the destination string to be copied to
 *@src: the source string
 *@r: the amount of characters to be copied
 *Return: the concatenated string
 */
char *_strncpy(char *dest, char *src, int r)
{
	int a, z;
	char *c = dest;

	a = 0;
	while (src[a] != '\0' && a < r - 1)
	{
		dest[a] = src[a];
		a++;
	}
	if (a < r)
	{
		z = a;
		while (z < r)
		{
			dest[z] = '\0';
			z++;
		}
	}
	return (c);
}

/**
 **_strncat - concatenates two strings
 *@dest: the first string
 *@src: the second string
 *@r: the amount of bytes to be maximally used
 *Return: the concatenated string
 */

char *_strncat(char *dest, char *src, int r)
{
	int a, z;
	char *c = dest;

	a = 0;
	z = 0;
	while (dest[a] != '\0')
		a++;
	while (src[z] != '\0' && z < r)
	{
		dest[a] = src[z];
		a++;
		z++;
	}
	if (z < r)
		dest[a] = '\0';
	return (c);
}

/**
 **_strchr - locates a character in a string
 *@c: the string to be parsed
 *@e: the character to look for
 *Return: (c) a pointer to the memory area s
 */
char *_strchr(char *c, char e)
{
	do {
		if (*c == e)
			return (c);
	} while (*c++ != '\0');

	return (NULL);
}
