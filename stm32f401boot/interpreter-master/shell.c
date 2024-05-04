/**
 * @file shell.c
 * File loader to test scripts in host environment
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int print(instance *it)
{
	variable *s = igetarg(it, 0);
	if (s->type == NUMBER) {
		if (s->value.f == (int)s->value.f)
			printf("%d\n", (int)s->value.f);
		else
			printf("%.3f\n", s->value.f);
	} else if (s->value.p != 0) {
		printf("%s\n", (char *)s->value.p);
	}
	return 0;
}

int gets(instance *it)
{
	char *line = 0;
	size_t size;
	getline(&line, &size, stdin);
	*strchr(line, '\n') = '\0';
	variable *v = make_vars(0, line);
	free(line);
	ipush(it, (size_t)v);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s file\n", argv[0]);
		return -1;
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == 0) {
		printf("Could not open file: %s\n", argv[1]);
		return -1;
	}

	instance *it = inewinstance();
	inew_cfunc(it, "print", print);
	inew_cfunc(it, "gets", gets);

	char *line = 0;
	size_t size;
	int result;
	while (getline(&line, &size, fp) != -1) {
		*strchr(line, '\n') = '\0';
		result = iaddline(it, line);
		if (result != 0)
			printf("Error: %d\n", result);
	}
	free(line);
	fclose(fp);

	irun(it);
	idelinstance(it);
	return 0;
}

