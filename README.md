# SLR(1) table and LR(0) generator

## Grammar syntax
After line `#TERM` goes line of terminals.  
After line `#NONTERM` goes line of non-terminals.  
Terminals and non-terminals can be any strings except `S` and `$` - which are reserved tokens.

Other lines starting with `#` are considered comments and ignored.  
Any other line is considered to be a rule.  

Rule syntax: `LHS RHS_1 RHS_2 ... RHS_N` , which is considered to be `<LHS> -> <RHS_1><RHS_2>...<RHS_N>` , 
where `LHS` needs to be a defined non-terminal and RHS_i to be defined.  
Rule's righthand side can be empty: `LHS` , which is considered to be `<LHS> -> ^`  
The first rule should be always with `S` lefthand side, meaning the full sentence, as it is later used to compute the first closure.  
Example is provided in [example.txt](./example.txt)

## Building
```bash
make
```

## Usage
Supposing you have your grammar listed in file `grammar.txt`:
```bash
./bin/slr grammar.txt
```
You can also give grammar to input of the programm through `stdin`, whether typing everything in terminal or redirecting output, but **this method might contain a lot of bugs**
```bash
cat grammar.txt | ./bin/slr
```

## Output
Directory `generated/` is created with files:
* `grammar.txt` - copied grammar after being lexed (to be sure that grammar is correctly parsed)
* `follow.txt` - follow-set of every non-terminal in grammar
* `LR_Collection.txt` - collection of canonical states of LR(0)
* `SLR_Table.txt` - collection of states and corresponding actions of SLR(1) table

All the generated output is also dublicated to stdout.

In case of error happening (due to programm's fail or uncertainty of given grammar), the reason of fail is written to stderr and process is terminated through uncaught exception.

Order of steps:
1. Lexer
2. Grammar loading
3. Generation of LR collection based on grammar & it's first rule
4. Generation of SLR table based on LR collection
