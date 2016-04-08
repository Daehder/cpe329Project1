all: StringLibTest
	./StringLibTest

StringLibTest: StringLib.o StringLibTest.o
	gcc StringLib.o StringLibTest.o -o StringLibTest

%.c: %.h 
	touch %@

%.o: %.c 
	@gcc -c $< -o $@
