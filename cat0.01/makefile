.PHONY: clean gcc

name = cat
targetName = $(name).c
flags = -o $(name) -Wall -Wextra -Werror -lm

gcc: $(targetName)
	gcc $(targetName) $(flags)

clean: 
	rm $(name)
