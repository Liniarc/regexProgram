all:
	g++ regex.cpp -lcurses -lboost_regex -ltinfo -std=c++11 -o regex
