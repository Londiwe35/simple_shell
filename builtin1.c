#include "shell.h"

/**
 * _myhistory - displays the history list
 *  with line numbers, starting at 0.
 * @info: Structure containing arguments. Used to maintain
 *  constant function prototype different.
 *  constant
 *  Return: Always 0
 */

int _myhistory(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias 
 * Return: Always 0 on success, 1 on error
 */

int unset_alias(info_t *info, char *str)
{
	char *f, o;
	int ret;

	f = _strchr(str, '=');
	if (!f)
		return (1);
	o = *f;
	*f = 0;
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*f = o;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 * Return: Always 0 on success, 1 on error
 */

int set_alias(info_t *info, char *str)
{
	char *f;

	f = _strchr(str, '=');
	if (!f)
		return (1);
	if (!*++f)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 * Return: Always 0 on success, 1 on error
 */

int print_alias(list_t *node)
{
	char *f = NULL, *i = NULL;

	if (node)
	{
		f = _strchr(node->str, '=');
		for (i = node->str; i <= f; i++)
		_putchar(*i);
		_putchar('\'');
		_puts(f + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */

int _myalias(info_t *info)
{
	int a = 0;
	char *f = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (a = 1; info->argv[a]; a++)
	{
		f = _strchr(info->argv[a], '=');
		if (f)
			set_alias(info, info->argv[a]);
		else
			print_alias(node_starts_with(info->alias, info->argv[a], '='));
	}

	return (0);
}
