CXXFLAGS = -std=c++11
LDFLAGS  = -lcurses -lboost_regex
LDCONFIG = $(shell which ldconfig 2>/dev/null || echo /sbin/ldconfig)
ifneq (,$(findstring libtinfo,$(shell $(LDCONFIG) -p)))
LDFLAGS += -ltinfo
endif

BINS = regex

.PHONY: all
all: $(BINS)

regex: regex.cpp

.PHONY: clean
clean:
	rm -f $(BINS)
