
EXE?= test
STATIC_EXE ?= $(EXE)

INCL := -I./include/
LIB_NAME := 

all: app

DEBUG = -g -ggdb
CFLAGS = -lstdc++ -lm -W -Wall

app: 
	$(CC) $(CFLAGS) $(DEBUG) main.c common.c parenthesis.c exp_parser.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE)

clean:
	rm -rf $(STATIC_EXE)
