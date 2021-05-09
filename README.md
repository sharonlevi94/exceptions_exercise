1. OOP 2 ex. 3:
//----------------------------------------------------------------------------
2. Creators:
a- Sharon Levi  
b- Bar Ifrah      
//----------------------------------------------------------------------------
3. General ideas and goals of the project
This program is a 'function calculator' as an exercise for Exceptions. 
Every Error that can be happen in this program, handle with Exceptions tool of C++.
You start with 2 built-in functions, and make your way up from there. 
Built in functions are: 1.ln(x), 2.sin(x).
First, the user insert the size of the list functions, the number must be 2 to 100. 
You can use the calculator with the following commands:
-the commands are the 3/4 letter words before the brackets in each line(e.g poly(nomial)..)-
  1. eval(uate) funcNum, xValue: evaluates func. num (funcNum) in the menu with inserted X.
  2. poly(nom) n c0 c1 ...cn-1: inserts a new polynomial when 'n' is the highest degree and c's are the coefficients.
  3. mul(tiply) funcNum1 funcNum2: multiplies the 2 wanted functions and adds the new func to the list.
  4. add funcNum1 funcNum2: adds between the 2 wanted functions and adds the new func to the list.
  5. comp(osite) funcNum1 funcNum2: composites 2 wanted functions and adds the new func to the list.
  (let f(), g() functions. comp f(), g() will output f(g())).
  6. log N funcNum: adds log N (funcNum) to the functions list.
  7. del(ete) funcNum: deletes the function 'funcNum' from the list.
  8. help: shows 'help' screen.
  9. exit: prints 'Goodbye' and exits the program.
  10. read <path_to_file>: for execute commands from a file, not support in command "read" in the file.
      after the program finish the commands from the file, it get back read commands from the user.
  11. resize <new_size>: the user can change the capacity of the functions list, the size must be 2 to 100.

The input varies between every command. Input is specified on every line of the commands.
The output will always be the function name with the result.
//----------------------------------------------------------------------------
4. Files:
General:
CMakeLists.txt- cmake of the project.
Source files: 
-CMakeLists.txt - Cmake file to all src files.
-ex1_sharon_levi_bar_ifrah.cpp //main
-Menu.cpp - class that redirects all commands.
-Function.cpp- abstract class, for inheritance.
-Add.cpp
-Composite.cpp
-Ln.cpp
-Log.cpp
-Multiply.cpp
-Poly.cpp
-Sin.cpp

Headers:
-CMakeLists.txt - To all headers
-Add.h
-Composite.h
-Function.h
-Ln.h
-Log.h
-rMacros.h
-Menu.h
-Multiply.h
-Poly.h
-Sin.h
-Macros.h
Resources‫:‬
-NONE
//----------------------------------------------------------------------------
5. Data structures :
   Vectors - shared pointer vactors to hold functions (sin/ln etc..)
//----------------------------------------------------------------------------
6. Algorithms that are worth mentioning:
‫---‬
//----------------------------------------------------------------------------
7. Known bugs:
//----------------------------------------------------------------------------
8. Other notes:
   Input from user- read on by one.
   Input from file - read with 'getline' command.
   If the input is uncorrect, the program clean the buffer.
