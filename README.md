Prog
====
Prog is a simple-yet-complex programming language designed to be easy to learn, but difficult to master.

It blends modern programming concepts with traditional syntax, encouraging both beginners and low-level hackers to explore its power.

Lines must end in a semicolon (`;`).

---

Datatypes
=========
Prog includes core types for numeric and spatial computing.

- `Int(n)` — A positive integer. `n` specifies byte size (1 to 32). Example: `Int4 x = 42;`
- `Float` — A 32-bit decimal number. Example: `Float pi = 3.14;`
- `Bool` — Boolean value: `true` or `false`
- `Vec(n)` — A float-based vector with 2 to 5 dimensions.
  - `Vec3 pos = Vec3(1.0, 2.0, 0.5);`

---

Variable Types
==============
- `Const` — Immutable value; must be assigned once.
- `Var` — Mutable variable; can be reassigned.

```prog
Const Int1 MAX = 10;
Var Float speed = 4.2;
speed = speed * 1.5;
```

---

Functions
=========
Functions are defined by:
1. Return type (e.g., `Int4`, `Float`, or `void`)
2. Function name
3. Parameters inside `()`
4. Function body in `{}`

```prog
Int4 square(Int4 n) {
    return n * n;
}

void log(String msg) {
    print("LOG: " + msg);
}
```

---

Control Flow
============
### If / Else
```prog
if (x > 5) {
    print("Big");
} else {
    print("Small");
}
```

### While Loop
```prog
while (alive) {
    tick();
}
```

### For Loop
```prog
for (Int1 i = 0; i < 10; i = i + 1;) {
    print(i);
}
```

---

Operators
=========
### Arithmetic
- `+`, `-`, `*`, `/`, `%`

### Comparison
- `==`, `!=`, `<`, `<=`, `>`, `>=`

### Logical
- `&&` — Logical AND
- `||` — Logical OR
- `!` — Logical NOT

---

Vectors
=======
Vectors store multiple float components and support math:

```prog
Vec3 a = Vec3(1, 2, 3);
Vec3 b = Vec3(3, 1, 2);
Vec3 c = a + b; // Adds component-wise
```

---

Entry Point
===========
Every program begins execution in a `void main()` function:

```prog
void main() {
    print("Hello, Prog!");
}
```

---

Comments
========
Single-line comments use `//`

```prog
// This is a comment
```

---

Grammar
=======

```program     -> statement+
statement   -> declaration | assignment | expression_statement | if_statement | block
declaration -> type identifier = expression ;
assignment  -> identifier = expression ;
expression_statement -> expression ;
if_statement -> if ( expression ) { statement+ }
block       -> { statement+ }
expression  -> identifier | number | expression operator expression | ( expression )
type        -> int | float | void
operator    -> + | - | * | /
identifier  -> [a-zA-Z_][a-zA-Z0-9_]*```
```

---

Future Features
===============
- Structs and Enums
- Pattern matching
- Imports and modules
- Bytecode or native compilation
- IDE syntax highlighting

---

License
=======
MIT

---
