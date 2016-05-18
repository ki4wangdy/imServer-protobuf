
#include <util.h>


spool spool_new(pool_t p)
{
	spool s;

	s = pmalloc(p, sizeof(struct spool_struct));
	s->p = p;
	s->len = 0;
	s->last = NULL;
	s->first = NULL;
	return s;
}

static void _spool_add(spool s, const char *goodstr)
{
	struct spool_node *sn;

	sn = pmalloc(s->p, sizeof(struct spool_node));
	sn->c = goodstr;
	sn->next = NULL;

	s->len += strlen(goodstr);
	if (s->last != NULL)
		s->last->next = sn;
	s->last = sn;
	if (s->first == NULL)
		s->first = sn;
}

void spool_add(spool s, const char *str)
{
	if (str == NULL || strlen(str) == 0)
		return;

	_spool_add(s, pstrdup(s->p, str));
}

void spool_escape(spool s, const char *raw, int len)
{
	if (raw == NULL || len <= 0)
		return;

	_spool_add(s, strescape(s->p, raw, len));
}

void spooler(spool s, ...)
{
	va_list ap;
	char *arg = NULL;

	if (s == NULL)
		return;

	va_start(ap, s);

	/* loop till we hit our end flag, the first arg */
	while (1)
	{
		arg = va_arg(ap, char *);
		if ((spool)arg == s)
			break;
		else
			spool_add(s, arg);
	}

	va_end(ap);
}

const char *spool_print(spool s)
{
	char *ret, *tmp;
	struct spool_node *next;

	if (s == NULL || s->len == 0 || s->first == NULL)
		return NULL;

	ret = pmalloc(s->p, s->len + 1);
	*ret = '\0';

	next = s->first;
	tmp = ret;
	while (next != NULL)
	{
		tmp = j_strcat(tmp, next->c);
		next = next->next;
	}

	return ret;
}

/** convenience :) */
const char *spools(pool_t p, ...)
{
	va_list ap;
	spool s;
	char *arg = NULL;

	if (p == NULL)
		return NULL;

	s = spool_new(p);

	va_start(ap, p);

	/* loop till we hit our end flag, the first arg */
	while (1)
	{
		arg = va_arg(ap, char *);
		if ((pool_t)arg == p)
			break;
		else
			spool_add(s, arg);
	}

	va_end(ap);

	return spool_print(s);
}


char *strunescape(pool_t p, char *buf)
{
	int i, j = 0;
	char *temp;

	if (buf == NULL) return(NULL);

	if (strchr(buf, '&') == NULL) return(buf);

	if (p != NULL)
		temp = pmalloc(p, strlen(buf) + 1);
	else
		temp = malloc(strlen(buf) + 1);

	if (temp == NULL) return(NULL);

	for (i = 0; i < strlen(buf); i++)
	{
		if (buf[i] == '&')
		{
			if (strncmp(&buf[i], "&amp;", 5) == 0)
			{
				temp[j] = '&';
				i += 4;
			}
			else if (strncmp(&buf[i], "&quot;", 6) == 0) {
				temp[j] = '\"';
				i += 5;
			}
			else if (strncmp(&buf[i], "&apos;", 6) == 0) {
				temp[j] = '\'';
				i += 5;
			}
			else if (strncmp(&buf[i], "&lt;", 4) == 0) {
				temp[j] = '<';
				i += 3;
			}
			else if (strncmp(&buf[i], "&gt;", 4) == 0) {
				temp[j] = '>';
				i += 3;
			}
		}
		else {
			temp[j] = buf[i];
		}
		j++;
	}
	temp[j] = '\0';
	return(temp);
}


char *strescape(pool_t p, const char *buf, int len)
{
	int i, j, newlen = len;
	char *temp;

	if (buf == NULL || len < 0) return NULL;

	for (i = 0; i < len; i++)
	{
		switch (buf[i])
		{
		case '&':
			newlen += 5;
			break;
		case '\'':
			newlen += 6;
			break;
		case '\"':
			newlen += 6;
			break;
		case '<':
			newlen += 4;
			break;
		case '>':
			newlen += 4;
			break;
		}
	}

	if (p != NULL)
		temp = pmalloc(p, newlen + 1);
	else
		temp = malloc(newlen + 1);
	if (newlen == len)
	{
		memcpy(temp, buf, len);
		temp[len] = '\0';
		return temp;
	}

	for (i = j = 0; i < len; i++)
	{
		switch (buf[i])
		{
		case '&':
			memcpy(&temp[j], "&amp;", 5);
			j += 5;
			break;
		case '\'':
			memcpy(&temp[j], "&apos;", 6);
			j += 6;
			break;
		case '\"':
			memcpy(&temp[j], "&quot;", 6);
			j += 6;
			break;
		case '<':
			memcpy(&temp[j], "&lt;", 4);
			j += 4;
			break;
		case '>':
			memcpy(&temp[j], "&gt;", 4);
			j += 4;
			break;
		default:
			temp[j++] = buf[i];
		}
	}
	temp[j] = '\0';
	return temp;
}
