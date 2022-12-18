.PHONY: gcc debug clean

name = cond
target = $(name).c
flags = -o $(name) -Wall -Wextra -Werror -lpthread

gcc: $(target)
	gcc $(target) $(flags)

debug: $(target)
	gcc $(target) -o $(name) -lpthread

clean:
	rm $(name)
