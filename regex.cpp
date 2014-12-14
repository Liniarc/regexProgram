#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <vector>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

int x = 0, y = 0;
string regexPattern;
string replacePattern;
string hint;
string prompt;

vector<string> lines;
vector<vector<string>> coloredLines;
vector<string> replaced;
vector<vector<string>> captures;
vector<int> caplen;

vector<vector<string>> captureSolutions;
vector<string> replaceSolutions;

bool extended = false;
bool global = false;
bool replaceMode = false;
bool match = false;
bool finished = false;

void mouseAction(int mx, int my)
{
    if (my == 2)
    {
        if (mx < 18)
            extended = !extended;
        else if (mx > 22 && mx < 36)
            global = !global;
        //else if (mx > 38 && mx < 57)
        //    replaceMode = !replaceMode;
    }
    else if (my == 0)
    {
        y = 0;
        x = mx-16;
        if (mx-16 > regexPattern.length())
            x = regexPattern.length();
    }
    else if (my == 1)
    {
        y = 1;
        x = mx-16;
        if (mx-16 > replacePattern.length())
            x = replacePattern.length();
    }
}

void keyAction( int ch )
{
    switch (ch)
    {
        case KEY_MOUSE:
            MEVENT event;
            getmouse(&event);
            mouseAction(event.x,event.y);
            break;
        case KEY_BACKSPACE:
        case 127:
            if (y == 0 && x > 0 && regexPattern.length() >= x)
            {
                regexPattern.erase(regexPattern.begin()+x-1);
                x--;
            }
            if (y == 1 && x > 0 && replacePattern.length() >= x)
            {
                replacePattern.erase(replacePattern.begin()+x-1);
                x--;
            } 
            break;
            case KEY_LEFT:
            if (x>0)
                x--;
            break;
        case KEY_RIGHT:
            if (y == 0 && x < regexPattern.length())
                x++;
            if (y == 1 && x < replacePattern.length())
                x++;
            break;
        case KEY_UP:
            y = 0;
            if (x > regexPattern.length())
                x = regexPattern.length();
            break;
        case KEY_DOWN:
            y = 1;
            if (x > replacePattern.length())
                x = replacePattern.length();
            break;
        default:
            if (ch >= 32 && ch < 127)
            {
                if (y == 0)
                    regexPattern.insert(regexPattern.begin()+x, ch);
                else
                    replacePattern.insert(replacePattern.begin()+x, ch);
                x++;
            }
    }
}

bool checkSolution()
{
    if (replaceMode)
    {
        if (replaced.size() != replaceSolutions.size())
            return false;
        for (int i = 0; i < replaced.size(); i++)
        {
            if (replaced[i] != replaceSolutions[i])
            {
                hint = "Check number ";
                hint += to_string(i+1);
                hint += "  Expected \"" + replaceSolutions[i] + "\"";
                hint += " got \"" + replaced[i] + "\"";
                return false;
            }
        }
        return true;
    }
    else if (!match)
    {
        if (captureSolutions.size() != captures.size())
            return false;
        for (int i = 0; i < captureSolutions.size(); i++)
        {
            while (captureSolutions[i].size() > captures[i].size())
            {
                captures[i].push_back("");
            }
            for (int j = 1; j < captureSolutions[i].size(); j++)
            {
                if (captureSolutions[i][j] != captures[i][j])
                {
                    hint = "Check number ";
                    hint += to_string(i+1);
                    hint += ", Capture \\";
                    hint += to_string(j);
                    hint += "  ";
                    hint += "Expected \"" +captureSolutions[i][j]+"\" ";
                    hint += "got \"" + captures[i][j] +"\"";
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        if (captureSolutions.size() != captures.size())
            return false;
        for (int i = 0; i < captureSolutions.size(); i++)
        {
            if (captureSolutions[i][0] == "")
            {
                if (captures[i].size() != 0)
                {
                    hint = "Check number ";
                    hint += to_string(i+1);
                    hint += ". Be aware of expressions that capture blank strings";
                    return false;
                }
            }
            else
            {
                if (captures[i].size() == 0)
                {
                    hint = "Check number ";
                    hint += to_string(i+1);
                    hint += ". This should be captured";
                    return false;
                }
            }
        }
        return true;
    }

        
}

void drawScreen()
{
    clear();
    move(0,0);
    addstr("Regex Pattern: ");
    move(0,16);
    addstr(regexPattern.c_str());
    move(1,0);
    addstr("Replace With:  ");
    move(1,16);
    addstr(replacePattern.c_str());
    move(2,0);
    addstr("Extended Regex [ ]    Global Tag [ ]    Replace Mode [ ]");
    
    if (extended)
        mvaddch(2,16,'X');
    if (global)
        mvaddch(2,34,'X');
    if (replaceMode)
        mvaddch(2,54,'X');
    

    move(4,0);
    addstr("HINT:  ");
    if (!checkSolution())
        addstr(hint.c_str());
    else
        addstr("CORRECT! Press any key to continue");
    move(5,0);
    addstr("TASK:  ");
    addstr(prompt.c_str());
    for (int i = 0; i < lines.size(); i++)
    {
        int color = 1;
        
        init_pair(1, COLOR_BLACK, COLOR_GREEN);
        init_pair(2, COLOR_BLACK, COLOR_BLUE);
        move(7,0);
        addstr("Text:");
        move(8+i,0);    
        for (int j = 0; j < coloredLines[i].size(); j++)
        {
            if (j%2!=0)
            {
                color = 1 - color;
                attron(COLOR_PAIR(color+1));
            }
            addstr(coloredLines[i][j].c_str()); 
            if (j%2!=0)
                attroff(COLOR_PAIR(color+1));
        }
        if (replaceMode)
        {
            move(7,50);
            addstr("Replaced String:");
            move(8+i, 50); 
            addstr(replaced[i].c_str());
        }
        else
        {
            int loc = 50;
            move(6,loc);
            addstr("Captured String:");
            for (int j = 0; j < caplen.size() && j < 9; j++)
            {
                move (7, loc);
                addstr("\\");
                addch(j+'0');
                loc += caplen[j];
            }
            
            loc = 50;
            for (int j = 0; j < captures[i].size(); j++)
            {
                move(8+i, loc); 
                addstr(captures[i][j].c_str());
                loc += caplen[j];
            }
        }
    }
    move(y, x+16);
}

void performRegex()
{
    replaced.clear();
    caplen.clear();
    caplen.push_back(5);
    hint = "";
    bool valid = true;
    regex r;
    try
    {  
        if (extended)
        {
            r.assign(regexPattern.c_str(), regex::extended);
        }
        else
        {
            if (regexPattern.back() != '\\')
                r.assign(regexPattern.c_str(), regex::basic);
            else
                valid = false;
        }
    }
    catch (regex_error &e)
    {
        valid = false;
    }
    if (!valid)
        hint = "Invalid regex!";

    for (int i = 0; i < lines.size(); i++)
    {
        coloredLines[i].clear();
        captures[i].clear();
        if (!valid)
        {
            replaced.push_back("");
            coloredLines[i].push_back(lines[i]);
            continue;
        }
        smatch m;
        string s= lines[i];
        auto words_begin = sregex_iterator(s.begin(), s.end(), r);
        auto words_end = sregex_iterator();

        for (sregex_iterator j = words_begin; j != words_end; j++)
        {
            m = *j;
            coloredLines[i].push_back(m.prefix().str());
            coloredLines[i].push_back(m.str());
            if (!global)
                break;
        }
        if (words_begin == words_end)
            coloredLines[i].push_back(s);
        else
            coloredLines[i].push_back(m.suffix().str());

        if (!global && regex_search (s,m,r)) {
            int numcap = 0;
            for (auto x:m) 
            {
                captures[i].push_back(x.str());
                if (numcap >= caplen.size())
                    caplen.push_back(5);
                if (x.str().length() > caplen[numcap])
                    caplen[numcap] = x.str().length()+3;
                numcap++;
            }
        }

        replaced.push_back(regex_replace(s,r,replacePattern));
    }
}

void getFile(string s)
{
    ifstream infile(s);
    string line;
    getline(infile,line);
    
    lines.clear();
    coloredLines.clear();
    captures.clear();
    replaced.clear();
    caplen.clear();

    captureSolutions.clear();
    replaceSolutions.clear();
    if (line == "--prompt")
    {
        string p;
        getline(infile, p);
        prompt = p;
        getline(infile,line);
    }
    if (line == "--line")
    {
        for (string s; getline(infile,s);)
        {
            if (s.length() >= 2 && s[0] == '-' && s[1] == '-')
            {
                line = s;
                break;
            }
            lines.push_back(s);
            coloredLines.push_back(vector<string>());
            captures.push_back(vector<string>());
            captureSolutions.push_back(vector<string>());
        }
    }
    if (line == "--capture" || line == "--match")
    {
        match = line == "--match";
        replaceMode = false;
        string s;
        int i = 0;
        while (getline(infile,s))
        {
            if (i == lines.size())
                i = 0;
            else
            {
                captureSolutions[i].push_back(s);
                i++;
            }
        }
    }
    else if (line == "--replace")
    {
        replaceMode = true;
        for (string s; getline(infile,s);)
        {
            replaceSolutions.push_back(s);
        }
    }
}


void initCurses()
{
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    mousemask(BUTTON1_PRESSED, NULL);
    keypad(stdscr, TRUE);
}


int main ()
{
    int level = 1;
    int ch = 0;
    getFile("problems/problem" + to_string(level));
    initCurses();
    while(true)
    {
        if (checkSolution())
        {
            regexPattern = "";
            replacePattern = "";
            x = 0;
            y = 0;
            if (level == 10)
                break;
            level++;
            getFile("problems/problem" + to_string(level));
        }
        keyAction(ch);
        performRegex();
        drawScreen();
        ch = getch();
    }
    endwin();
    printf("end\n");
    return 0;
}
