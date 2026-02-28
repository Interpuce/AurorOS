# AurorOS's code style

1. Never include `stdbool.h`, `stdlib.h`, `unistd.h`, `stdint.h` or other C standard library headers in kernel code or make alternative implementations of it. Instead use `types.h` or other kernel headers containing the missing declarations.
2. Avoid using `../../include` or any relative path to the `include` folder if it is not architecture-dependent (for example contains structures only present in x86).
3. Use [snake_case](https://en.wikipedia.org/wiki/Snake_case) for all variable, function, and file names.
4. Do not place any C source files inside `include`.
