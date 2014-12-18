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
The `TASK` tells you what you should try and accomplish for that level.   
Exit the program with `Ctrl+C`

Making your own problems
---
Create a blank text file with a number, preferrably starting with one greater than the largest number problem. (For example, adding an 11th question would result in creating a file called "problem11")

Use the following format:

`--prompt`    
This is where your prompt goes.    

`--line`    
Each line of these are the possible strings/complete lines/sentences/etc. to capture.

`--match`    
Indicates a string matching problem (as opposed to a replace or capture problem)    
Each line indicates whether or not a regex should match that corresponding string/sentence.    
A blank line indicates not matching, anything else indicates matching.

`--capture`
Indicates a string capture problem (as opposed to a replace or matching problem)    
Each line corresponds to the capture groups that are captured by the regex.    
These lines indicate what should be captured by `\0` or the entire regex.    
Following `--n` lines are what should be captured by the corresponding capture groups. i.e. `\1` `\2`

`--replace`    
Indicates a replace problem (as opposed to a find problem)    
Each line indicates what the capture and replace regex should return.

TODOs
---
Create a "main menu" or pass the program parameters to indicate levels/difficulty    
Create additional levels    
Improve the gui to be more clear and helpful    

Known bugs
---
In replace mode, if the global flag is not set, it will replace globally.
