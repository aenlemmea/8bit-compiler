----
# Note

**The codegen is not there yet. It requires some work. For now all the functionality is in place, just need the register allocation and scheduling in place.**

The sample folder shows a sample ran with the compiler.

----

## 8bit-compiler

8bit-compiler for [8bit-computer](https://github.com/lightcode/8bit-computer) architecture.

It does some minor semantic checking and the codegen works directly on the AST.

## Usage:

```bash
Usage: ./etbit <filename> --show-parsed
```

## Building:

```bash
# In 8bit-compiler directory
cmake . -Bbuild && make
```

If you see deprecated warnings, it is for the test due to some extraneous constructs that were put in place.

## Sample outputs

```cobol
parser_test

stmnt_kind::VARDEC_STMNT 
TYPE: 
expr_kind::TYPE Kind: TYPE_INT_DECL Value: int
IDENT: 
expr_kind::IDENTIFIER Kind: IDENT Value: a
stmnt_kind::VARDEC_STMNT 
TYPE: 
expr_kind::TYPE Kind: TYPE_INT_DECL Value: int
IDENT: 
expr_kind::IDENTIFIER Kind: IDENT Value: e
 IF COND: expr_kind::CONDN 
expr_kind::INFIX 
Left: expr_kind::IDENTIFIER Kind: IDENT Value: a
Oper: ==
Right: expr_kind::NUM Kind: NUMERAL Value: 3 BODY: 
stmnt_kind::EXPR_STMNT 
expr_kind::INFIX 
Left: expr_kind::IDENTIFIER Kind: IDENT Value: e
Oper: =
Right: expr_kind::INFIX 
Left: expr_kind::IDENTIFIER Kind: IDENT Value: e
Oper: +
Right: expr_kind::NUM Kind: NUMERAL Value: 4
```

----

```cobol

lexer test

1 Kind: IDENT Value: a
2 Kind: ASSIGNMENT Value: =
3 Kind: NUMERAL Value: 10
4 Kind: SEMICOLON Value: ;
5 Kind: IDENT Value: b
6 Kind: ASSIGNMENT Value: =
7 Kind: NUMERAL Value: 20
8 Kind: SEMICOLON Value: ;
9 Kind: IDENT Value: c
10 Kind: ASSIGNMENT Value: =
11 Kind: IDENT Value: a
12 Kind: PLUS Value: +
13 Kind: IDENT Value: b
14 Kind: SEMICOLON Value: ;
15 Kind: TYPE_INT_DECL Value: int
16 Kind: IDENT Value: a
17 Kind: SEMICOLON Value: ;
18 Kind: IF Value: if
19 Kind: LPAREN Value: (
20 Kind: IDENT Value: c
21 Kind: EQUALS Value: ==
22 Kind: NUMERAL Value: 30
23 Kind: RPAREN Value: )
24 Kind: LCURLY Value: {
25 Kind: IDENT Value: c
26 Kind: ASSIGNMENT Value: =
27 Kind: IDENT Value: c
28 Kind: PLUS Value: +
29 Kind: NUMERAL Value: 1
```
