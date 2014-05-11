SOURCE_FILES = main.c
all:
	gcc $(SOURCE_FILES) -o gsg

clean:
	rm *.fasta
	rm ./gsg