Practice Regular Expressions
============

This is a simple C++ application which allows you to practice creating [regular expression](http://en.wikipedia.org/wiki/Regular_expression) patterns. We have provided 10 prompts which cover a variety of regular expression pattern usages. With this program, you can modify the prompts we have provided and also create your own prompts to solve. This program will highlight which characters match the regular expression pattern that you type in and will not allow continuing to the next problem until the current problem is solved correct.

![alt text](http://i.imgur.com/qZTMalh.gif "gif demo")

Building and running
---
Required libraries: **C++11, [Curses Library](http://en.wikipedia.org/wiki/Curses_%28programming_library%29), [Boost Library](http://www.boost.org/)**

After downloading/cloning this repository, perform the following commands:
```
$ make      # builds the program
$ ./regex   # runs the program
```

Interface
---
| Input | Description |
|:-----|:-----|
| Arrow keys/mouse | Move between `Regex pattern:` and `Replace with:`|
| Mouse | Toggles `Extended regex` and `Global Tag` checkboxes. |
| `Ctrl+E` | Toggles `Extended regex` checkbox. |
| `Ctrl+G` | Toggles `Global Tag` checkbox. |
| `Ctrl+C` | Exits the program. |

| Onscreen | Description |
|:------|:-----|
| `TASK` | Tells you what you should try and accomplish for that level. This will change to indicate that the problem is solved correctly.|
| `HINT` | Provides a hint. |

Making your own problems
---
Create a blank text file with a number, preferrably starting with one greater than the largest number problem. (For example, adding an 11th question would result in creating a file called "problem11")

Use the following format:

| Flag | Description |
|:--------------|:---------------|
|`--prompt` | This is where your prompt goes.|
|`--line`| Each line of these are the possible strings/complete lines/sentences/etc. to capture.|
|`--match`| Indicates a string matching problem (as opposed to a replace or capture problem). Each line indicates whether or not a regex should match that corresponding string/sentence. A blank line indicates not matching, anything else indicates matching.|
|`--capture`| Indicates a string capture problem (as opposed to a replace or matching problem). Each line corresponds to the capture groups that are captured by the regex. These lines indicate what should be captured by `\0` or the entire regex. Following `--n` lines are what should be captured by the corresponding capture groups (backreferencing) i.e. `\1` `\2`|
|`--replace`| Indicates a replace problem (as opposed to a find problem). Each line indicates what the capture and replace regex should return.|

Refer to the provided prompts if you need assistance.

TODOs
---
Create a "main menu" or pass the program parameters to indicate levels/difficulty    
Create additional levels    
Improve the gui to be more clear and helpful    

Known bugs
---
In replace mode, if the global flag is not set, it will replace globally.
