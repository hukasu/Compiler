﻿# Experimental Regular Expressions

Here is a quick explanation of the experimental regular expression language used by this project.

This language differs from POSIX and other well established regular expression languages as it prefers prefixed operands over infix or postfix.

## Start of Text

The start of text is represented by `{b}`.  

## End of Text

The end of text is represented by `{e}`.  

## Literals

All characters except `{` and `}` are used as-is.  

E.g. `a`, `^`, `♪`, `山`  

You can write reserved characters, and any other character, by wrapping them in `{l:<c>}`

E.g. `{l:{}`, `{l:}}`, `{l:a}`, `{l::}`, `{l:♪}`, `{l:山}`

## Alternation

The alternation between 2 or more paths is done by `{a:<expr1>:<expr2>[:<exprN>]}`.

E.g. `{a:{abc}{def}{}}`

Note: the empty path is to represent an empty string (`ε`).

## Concatenation

Concatenation does not have to be wrapped on curly brackets, any symbol followed by another will be treated as concatenation.

E.g. `abc{a:{def}{ghi}}`

## Repetition

Repetition is achieved by using `{r:<expr>}`.

E.g. `{r:abc}`

You can also achieve a fixed amount of repetition, or a range, with `{r;<n>:<expr>}` and `{r;<b>;<e>:<expr>}`, respectively.

E.g. `{r;5:abc}`, `{r;1;3:abc}`

Note: `<n>`, `<b>`, and `<e>` must be positive non zero integers, and `<e>` must be greater than `<b>`.

## Option

The POSIX '?' is used as `{o:<expr>}`

E.g. `{o:abc}`

## Grouping

Grouping is already dealt by the curly brackets, so parenthesis will be read as literals.

# Future

As it is at the moment, it would be cumbersome to use this regular expression, as it does not allow wildcards or character classes,
if this gets off the ground, character classes will be the first to be added.

Maybe even earlier than that if the author get annoyed by leaving it out.
