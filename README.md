regexProgram
============

A simple C++ application for solving regular expression problems.

![alt text](http://i.imgur.com/qZTMalh.gif "gif demo")

Make the program
---
Prerequsites: **C++11, Curses Library, Boost Library**

In a terminal perform the following commands:
```
$ make
$ ./regex
```

Brief interface explaination
---
Use the arrow keys or mouse to move between "Regex pattern:" and "Replace with:"    
Use mouse to toggle Extended regex and Global Tag.

Making your own problems
---
Create a blank text file with a number, preferrably starting with one greater than the largest number problem. (For example, adding an 11th question would result in creating a file called "problem11")

Use the following format:

`--prompt`    
This is where your prompt goes.    

`--line`    
Each line of these are the possible strings/complete lines/sentences/etc. to capture.

`--match`    
Indicates a string matching problem (as opposed to a replace problem)    
Each line indicates a [TODO FIX THIS EXPLAINATION]

`--replace`    
Indicates a replace problem (as opposed to a find problem)
