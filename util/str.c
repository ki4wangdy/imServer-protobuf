
#include <util.h>


char *j_strdup(const char *str)
{
	if (str == NULL)
		return NULL;
	else
		return strdup(str);
}

char *j_strcat(char *dest, const char *txt)
{
	if (!txt) return(dest);

	while (*txt)
		*dest++ = *txt++;
	*dest = '\0';

	return(dest);
}

int j_strcmp(const char *a, const char *b)
{
	if (a == NULL || b == NULL)
		return -1;

	while (*a == *b && *a != '\0' && *b != '\0'){ a++; b++; }

	if (*a == *b) return 0;

	return -1;
}

int j_strcasecmp(const char *a, const char *b)
{
	if (a == NULL || b == NULL)
		return -1;
	else
		return strcasecmp(a, b);
}

int j_strncmp(const char *a, const char *b, int i)
{
	if (a == NULL || b == NULL)
		return -1;
	else
		return strncmp(a, b, i);
}

int j_strncasecmp(const char *a, const char *b, int i)
{
	if (a == NULL || b == NULL)
		return -1;
	else
		return strncasecmp(a, b, i);
}

int j_strlen(const char *a)
{
	if (a == NULL)
		return 0;
	else
		return strlen(a);
}

int j_atoi(const char *a, int def)
{
	if (a == NULL)
		return def;
	else
		return atoi(a);
}

char *j_attr(const char** atts, const char *attr)
{
	int i = 0;

	while (atts[i] != '\0')
	{
		if (j_strcmp(atts[i], attr) == 0) return (char*)atts[i + 1];
		i += 2;
	}

	return NULL;
}

/** like strchr, but only searches n chars */
char *j_strnchr(const char *s, int c, int n) {
	int count;

	for (count = 0; count < n; count++)
	if (s[count] == (char)c)
		return &((char *)s)[count];

	return NULL;
}