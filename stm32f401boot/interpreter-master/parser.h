/**
 * @file parser.h
 * Main library component; parses, manages and runs script
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

#ifndef PARSER_H_
#define PARSER_H_

#include "variable.h"

#include <stdint.h>

/**
 * Contains data for an 'instance' of script.
 * Multiple instances can be created to run different scripts at once.
 */
typedef struct {
	variable *vars;    /**< An array of defined variables */
	char **names;      /**< An array of names for the variables */
	size_t *stack;   /**< The instance's stack */
	size_t stidx;    /**< The instance's position in the stack */
	variable ***lines; /**< Compiled data for each line of script */
	size_t lnidx;    /**< Position in script/line being run */
	variable *ret;     /**< Pointer to the last returned variable */
	uint8_t indent;    /**< Current indent/scope of the instance */
	uint8_t sindent;   /**< Indent of scope to skip, for false conditionals */
} instance;

/**
 * A flag to set in instance.sindent to determine if code should be skipped
 * over.
 */
#define SKIP (1 << 7)

typedef int (*func_t)(instance *);

/**
 * Creates a new parser instance, loading the built-in library into it.
 * @return a new, malloc'd instance
 */
instance *inewinstance(void);

/**
 * Destroys an instance, freeing all used memory including for the instance
 * itself.
 * @param it the instance to destroy
 */
void idelinstance(instance *it);

/**
 * Parses the given line of script, and adds it to the end of the instance's
 * stored script.
 * @param it the instance to use
 * @param s the line to load
 * @return zero for success, otherwise error
 */
int iaddline(instance *it, const char *s);

/**
 * Runs the instance with its loaded script until there's nothing more to run.
 * @param the instance to use
 * @return an error code, or zero if success
 */
int irun(instance *it);

/**
 * Makes a C function visible to the script.
 * @param it the current instance
 * @param name the name of the function when called from the script
 * @param func the C function
 */
void inew_cfunc(instance *it, const char *name, func_t func);

/**
 * Creates a script-accessible float number.
 * @param it the current instance
 * @param name the name of the new variable
 * @param f the variable's initial value
 */
void inew_number(instance *it, const char *name, float f);

/**
 * Pops a word from the instance's stack.
 * @param it the current instance
 * @return the popped value
 */
size_t ipop(instance *it);

/**
 * Pushes a word to the instance's stack.
 * @param it the current instance
 * @param v the word to push
 */
void ipush(instance *it, size_t v);

/**
 * Gets the nth argument passed to the current C function.
 * This call has no protections.
 * @param it the current instance
 * @param n the index of the argument, zero-based
 * @return the argument's variable
 */
variable *igetarg(instance *it, size_t n);

/**
 * Parses the given line, returning compiled data to run.
 * For internal use only.
 * @param it the current instance
 * @param s the string to parse
 * @return the 'compiled code'
 */
variable **iparse(instance *it, const char *s);

/**
 * 'Solves' (runs) the given compiled data, returning what the line returns.
 * For internal use only.
 * @param it the current instance
 * @param ops the compiled data to run
 * @param count the size of the ops array, zero if unknown
 * @return the variable returned by the line, null if none
 */
variable *isolve(instance *it, variable **ops, size_t count);

#endif // PARSER_H_
