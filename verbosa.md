# Verbosa Language

**Version**: 1.0.0  
**Author**: ItsZariep  
**Date**: July 06 2024

## Introduction

Verbosa is a low-level programming language designed to enforce explicit and unambiguous code. It emphasizes verbosity, ensuring that every operation and instruction is clearly defined and easy to follow.

## Design Philosophy

- **Function-Centric**: In Verbosa, every operation is a function. Internal functions are invoked using the "@" symbol.
- **Separation of Concerns**: External functions are distinctly called via `@call`, creating a clear boundary between language constructs and external resources. For instance, instead of directly invoking a function named "read," you must use `@call (source.function) [function argument]`. Example: `@call (h.vc.readfile) [/path/to/file]`.
- **Contextual Naming**: Internal function calls can, and sometimes must, include a name for context, particularly in control structures.

## Syntax Overview

### Function Syntax

As highlighted in the design, every operation in Verbosa is performed through functions. There are two main categories of function calls:

1. **Internal Functions**: These are core language functions prefixed with "@".
2. **External Functions**: These are called using the `@call (type.header.function) [argv] {codeblock}` syntax, for example "@call (h.vc.print) {"hello"}", if the type is not specified, it will default to `h(eader)`. If the resource is not available as a header, it defaults to `f(ile)`.

#### Examples of Internal Functions:

- **@include**: Incorporates external files or headers into the source code. For files, it references paths relative to the main file. For headers, it uses the `$HEADERPATH` environment variable (default: `/usr/lib/verbosa`).

  ```verbosa
  @include (file) [text.vh, input.vh];
  @include (header) [stdio];
  ```

- **@declare**: Declares a data type, specifying whether it’s constant or variable.

  ```verbosa
  @declare (var.char) [character = 'h'];
  @declare (const.str) [string = 'hello'];
  @declare (var.int) [integer = 32];
  ```

- **@modify**: Modifies a value without invoking a function.

  ```verbosa
  @modify (character) [character = 'l'];
  ```

#### Control Structures:

The `@control` function handles structures such as if/else, switches, and cases. Due to the need for contextual clarity, `@control` requires an attribute used as a name.

```verbosa
@control checkcondition (if) [condition]:
{ 
    // code block
}
@control checkcondition (elseif) [condition]:
{
    // another code block
}
@control checkcondition (else) [checkcondition]:
{
    // else code block
}
```

#### Creating External Functions:

The `@function` keyword is used to define external functions. Functions require a name attribute and follow the syntax `@function funcname (returntype) [argv]`.

For example, to create a `main` function:

```verbosa
@function main (int) [h.vc.argv]
{
    @call (file.print) ["hello world"];
    @call (file.return) [0];
}
```

### External Functions

External functions are invoked through `@call`. For instance, after defining a function using `@function`, it can be executed with `@call`:

```verbosa
@function sum (int) [int.num1, int.num2]:
{
    @call (file.print) ["Answer: %d\n" <- @call (file.math) [num1 + num2]];
}
```

### Special Values

Verbosa provides a set of special values to simplify coding. These are useful for passing arguments to functions without defining extra values:

- `null`
- `void`
- `false`
- `true`
- `int`
- `char`
- `str`
- `float`
- `-`
- `+`
- `/`
- `*`

### Reserved Names
Headers cannot use the following names for syntax reasons, and the compiler will raise an error indicating that the file is already in use:

- **vc** as a header: reserved for external language functions.
- **self** as a header: used by the compiler to identify the self-compiled file.

### Verbosa Standar functions

Verbosa standar library and verbosa standard functions are available by default without need to include, since are low level declarations, is not practical to  have this functions on including, so are archtitecture specific functions.

in every program is possible to invoke @call (h.vc.FUNCTIONNAME) [ARGUMENTS].

- print: print formated text to stdout.
- put: print unformated text to stdout.
- getinput: get input from stdin.
- argv: system arguments (type char)

### File Types
Verbosa uses several file types for compiling:

- .vc: **V**erbosa **C**ode
- .vh: **V**erbosa **H**eader
- .vo: **V**erbosa **O**bject (only used when compiling)
- .vt: **V**erbosa **Token** (only used when compiling)