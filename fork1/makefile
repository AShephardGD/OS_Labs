.PHONY: gcc debug clean

name = fork
targetName = $(name).c
flags = -o $(name) -Wall -Wextra -Werror

gcc: $(targetName)
	gcc $(targetName) $(flags)

debug: $(targetName)
	gcc $(targetName) -o $(name)

clean: 
	rm $(name)
