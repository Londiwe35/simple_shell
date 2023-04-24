#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 * Return: bytes read
 */

ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t c = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		c = getline(buf, &len_p, stdin);
#else
		c = _getline(info, buf, &len_p);
#endif
		if (c > 0)
		{
			if ((*buf)[c - 1] == '\n')
			{
				(*buf)[c - 1] = '\0'; /* remove trailing newline */
				c--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = c;
				info->cmd_buf = buf;
			}
		}
	}
	return (c);
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 * Return: bytes read
 */

ssize_t get_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t a, s, len;
	ssize_t c = 0;
	char **buf_p = &(info->arg), *o;

	_putchar(BUF_FLUSH);
	c = input_buf(info, &buf, &len);
	if (c == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		s = a; /* init new iterator to current buf position */
		o = buf + a; /* get pointer for return */

		check_chain(info, buf, &s, a, len);
		while (s < len) /* iterate to semicolon or end */
		{
			if (is_chain(info, buf, &s))
				break;
			s++;
		}

		a = s + 1; /* increment past nulled ';'' */
		if (a >= len) /* reached end of buffer? */
		{
			a = len = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = o; /* pass back pointer to current command position */
		return (_strlen(o)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (c); /* return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @a: size
 * Return: c
 */

ssize_t read_buf(info_t *info, char *buf, size_t *a)
{
	ssize_t c = 0;

	if (*a)
		return (0);
	c = read(info->readfd, buf, READ_BUF_SIZE);
	if (c >= 0)
		*a = c;
	return (c);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 * Return: i
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t a, len;
	size_t b;
	ssize_t c = 0, i = 0;
	char *o = NULL, *new_p = NULL, *d;

	o = *ptr;
	if (o && length)
		i = *length;
	if (a == len)
		a = len = 0;

	c = read_buf(info, buf, &len);
	if (c == -1 || (c == 0 && len == 0))
		return (-1);

	d = _strchr(buf + a, '\n');
	b = d ? 1 + (unsigned int)(d - buf) : len;
	new_p = _realloc(o, i, i ? i + b : b + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (o ? free(o), -1 : -1);

	if (i)
		_strncat(new_p, buf + a, b - a);
	else
		_strncpy(new_p, buf + a, b - a + 1);

	i += b - a;
	a = b;
	o = new_p;

	if (length)
		*length = i;
	*ptr = o;
	return (i);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 * Return: void
 */

void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
