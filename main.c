#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>

char* currdatetime()
{
    const int size = 20;
    char *cdt = (char*)malloc(sizeof(char)*size);

    if(cdt == NULL)
    {
        return NULL;
    }

    memset (cdt, 0, size);

    time_t currDateTime;
    currDateTime = time(NULL);

    if(currDateTime == -1)
    {
        return NULL;
    }

    struct tm  *timeinfo = localtime (&currDateTime);
    if(strftime(cdt, 20, "%d.%m.%y", timeinfo) == 0)
    {
        return NULL;
    }

    return cdt;
}

char *getname(const char *pathtofile, const char *ext)
{
    char *timestamp = currdatetime();

    int size = (strlen(pathtofile) + strlen(ext) + strlen(timestamp) + 1);

    char *filename = (char*)malloc(sizeof(char)*size);

    if(filename == NULL)
    {
        return NULL;
    }

    memset (filename, 0, size);
    strcpy(filename, pathtofile);
    strcpy(filename+strlen(pathtofile), timestamp);
    strcpy(filename+strlen(pathtofile)+strlen(timestamp), ext);

    free(timestamp);
    timestamp = NULL;

    return filename;
}

static inline void loadBar(int x, int n, int r, int w)
{
    if ( x % (n/r +1) != 0 ) return;
    float ratio = x/(float)n;
    int   c     = ratio * w;
     printf("%3d%% [", (int)(ratio*100) );
     for (int x=0; x<c; x++)
       printf("=");
 
    for (int x=c; x<w; x++)
       printf(" ");
    printf("]\n\033[F\033[J");
}

int main(int argc, char const *argv[])
{
	if (argc > 1 && !strcmp(argv[1],"-s")) {
		srand (time(NULL));
		const char *geneticTags[4] = {
			"G",
			"T",
			"C",
			"A"
		};
		const char *geneticArray[1024];
		int count = 0;
    	char *filename = getname("generation_", ".txt");
		FILE *file; 
		file = fopen(filename,"a+");
		printf("Starting generation...\n");
		while (count < 170000000){
			count++;
			int n = rand()%4;
			loadBar(count,170000000,100,100);
			fprintf(file,"%s", geneticTags[n]);
		}
		printf("Done. File written as %s\n", filename);
		fclose(file);
		return 0;
	}else {
		printf("Genetic Sequence Generator\n");	
		printf("Prints out 170,000,000 random combinations of the four main base pairs (A,T,C,G)\n");
		printf("Arguments;\n");
		//printf("-o	Output directory (Defaults to working directory)\n"); TODO
		printf("-s	Start generation (Writes to file in working directory\n");
		printf("Example: ./gsg -s\n");
		return 0;
	}	
}