SOURCE_FILES = main.c
all:
	gcc $(SOURCE_FILES) -o gsg

clean:
	rm *.txt
	rm ./gsg