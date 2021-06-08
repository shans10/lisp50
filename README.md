# Lisp50
A minimal Lisp built using C programming language based on the book [Build Your Own Lisp](http://buildyourownlisp.com/) by Daniel Holden. It is built using standard C libraries without using any external libraries. This project is made to be submitted as the Final Project for Harvard's CS50's Introduction to Computer Science.

#### Video Demo: https://youtu.be/JnzQRyZtroE

## Compilation and Execution

* Clone the repository  
`git clone https://github.com/shans10/lisp50`  
* Compile it with 'make' command  
`make`

**Note**: This project requires *editline* library to compile which can be installed in the following ways:
### Mac OS
On Mac the editline library comes with *Command Line Tools* which can be installed using:  
`xcode-select --install`

### Linux
**Ubuntu**: `sudo apt-get install libedit-dev`  
**Fedora**: `sudo dnf install libedit-devel`  

After the compilation is succesful it can be simply executed by invoking `./lisp50`  

### Prompt
```
Lisp50 Version 1.0  
Press Ctrl+c or type 'exit' to Exit  

lisp50>
```
This is the prompt the user will get after running the executable. Now the user can play with the new language by either using the builtin functions or creating functions of their own.

**Note:** To exit the prompt either press `Ctrl+c` or type `exit` and press Enter.

### Basic Language Syntax
This version of lisp uses  Polish Notation or pre-fix notation i.e. the operator comes before the operands.  
*Example:* `(* 5 10)` or `(+ 7 (* 2 9))`

**Note:** Expressions are evaluated recursively i.e. `(* (+ 5 6) (- 10 5))` = `(* 11 5)` = `55`

### Data Types
| Types           | Examples                         | Description(if any) |  
| --------------- | -------------------------------- | ------------------- |  
| Integers        |  `1`, `-4`, `55`, `0`  
| Boolean         |  `true(1)`, `false(0)`  
| Strings         |  `"Hello, CS50"`  
| S-Expressions   | `(5)`, `(+ 5 6)`                 | Any expression inside parantheses is an S-Expression. They are automatically evaluated and must begin with a function or operator or be a single value.                                       | 
| Q-Expressions   | `{1 2 3}`, `{hello world}`       | They are space seperated lists inside curly braces and are not evaluated automatically. |
| Functions       | `(tail {1 2 3})`, `list 1 2 3 4` | These are builtin functions. This lisp also provides the user ability to create their own functions. |  

### User Defined Functions
The users can define their own functions by following the below syntax:  
```
Lisp50 Version 1.0
Press Ctrl+c or type 'exit' to Exit

lisp50> def {add-mul} (\ {x y} {+ x (* x y)})
()
lisp50> add-mul 10 20
210
lisp50> add-mul 10
(\ {y} {+ x (* x y)})
lisp50> def {add-mul-ten} (add-mul 10)
()
lisp50> add-mul-ten 50
510
lisp50>
```
**Note:** A function called `fun` has been added to the Standard Library `stblib.lspy` which is loaded on execution. This function makes it even easier to create new functions as shown below:  
```
Lisp50 Version 1.0
Press Ctrl+c or type 'exit' to Exit

lisp50> fun {add-together x y} {+ x y}
()
lisp50> add-together 10 8
18
lisp50>
```

### Standard Library
The standard library file `stdlib.lspy` is automatically loaded on execution and contains some of the useful functions added using our lisp language instead of C language. One of those functions `fun` has been already mentioned above.  
Some other useful functions are:

| Function Name | Description |
| ------------- | ----------- |
| not           | Print the 'not' of the entered number.             |
| or            | Print the logical 'or' of the two numbers.         |
| and           | Print the logical 'and' of two numbers.            |
| len           | Find the legth of a list.                          |
| nth           | Find the nth element of the list.                  |
| last          | Find the last element in the list.                 |
| reverse       | reverse the given list.                            |
| fib           | Find the Fibonacci Sequence of the given number.   |
