.PHONY: gcc debug clean

name = ls
targetName = $(name).c
flags = -o $(name) -Wall -Wextra -Werror -lm

gcc: $(targetName)
	gcc $(targetName) $(flags)

debug: $(targetName)
	gcc $(targetName) -o $(name) -lm

clean:
	rm $(name)
