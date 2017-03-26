all:
	gcc lab.c -o lab3
	gcc lab.c -o lab3san -fsanitize=address
	cppcheck --enable=all --inconclusive --std=posix lab.c
	/usr/src/linux-source-4.4.0/scripts/checkpatch.pl -f lab.c
