target = _npuzzle
src_dir = src/
src = $(wildcard $(src_dir)*.c)
cc = gcc


rel: flags := -Wall -Wextra -Ofast -flto
rel: compile 

dbg: flags := -Wall -Wextra -g3
dbg: compile

compile:
	$(cc) -o $(target) $(flags) $(src) -lraylib -lm 
