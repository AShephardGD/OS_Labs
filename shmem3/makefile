.PHONY: firstgcc secondgcc gcc debug firstdebug seconddebug clean

first = first
firstTarget = $(first).c

second = second
secondTarget = $(second).c

flags = -Wall -Wextra -Werror
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
	gcc $(firstTarget) -o $(first)
	gcc $(secondTarget) -o $(second)

firstdebug: $(firstTarget)
	gcc $(firstTarget) -o $(first)

seconddebug: $(secondTarget)
	gcc $(secondTarget) -o $(second)

clean:
	rm $(first)
	rm $(second)
