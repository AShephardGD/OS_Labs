.PHONY: firstgcc secondgcc gcc debug firstdebug seconddebug clean

first = first
firstTarget = $(first).c

second = second
secondTarget = $(second).c

flags = -Wall -Wextra -Werror -lpthread
firstFlags = -o $(first) $(flags)
secondFlags = -o $(second) $(flags)

gcc: $(firstTarget) $(secondTarget)
	gcc $(firstTarget) $(firstFlags)
	gcc $(secondTarget) $(secondFlags)

firstgcc: $(firstTarget)
	gcc $(firstTarget) $(firstFlags)

secondgcc: $(secondTarget)
	gcc $(secondTarget) $(secondFlags)

debug: $(firstTarget) $(secondTarget)
	gcc $(firstTarget) -o $(first) -lpthread
	gcc $(secondTarget) -o $(second) -lpthread

firstdebug: $(firstTarget)
	gcc $(firstTarget) -o $(first) -lpthread

seconddebug: $(secondTarget)
	gcc $(secondTarget) -o $(second) -lpthread

clean:
	rm $(first)
	rm $(second)
