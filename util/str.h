
#ifndef _UTIL_STRING_H_
#define _UTIL_STRING_H_

char *j_strdup(const char *str); /* provides NULL safe strdup wrapper */
char *j_strcat(char *dest, const char *txt); /* strcpy() clone */
int j_strcmp(const char *a, const char *b); /* provides NULL safe strcmp wrapper */
int j_strcasecmp(const char *a, const char *b); /* provides NULL safe strcasecmp wrapper */
int j_strncmp(const char *a, const char *b, int i); /* provides NULL safe strncmp wrapper */
int j_strncasecmp(const char *a, const char *b, int i); /* provides NULL safe strncasecmp wrapper */
int j_strlen(const char *a); /* provides NULL safe strlen wrapper */
int j_atoi(const char *a, int def); /* checks for NULL and uses default instead, convienence */
char *j_attr(const char** atts, const char *attr); /* decode attr's (from expat) */
char *j_strnchr(const char *s, int c, int n); /* like strchr, but only searches n chars */

#define util_string_strdup		j_strdup
#define util_string_strcat		j_strcat
#define util_string_strcmp		j_strcmp
#define util_string_strcasecmp	j_strcasecmp

#define util_string_strncmp		j_strncmp
#define util_string_strncasecmp j_strncasecmp
#define util_string_strlen		j_strlen
#define util_string_atoi		j_atoi
#define util_string_attr		j_attr
#define util_string_strnchr		j_strnchr

#endif