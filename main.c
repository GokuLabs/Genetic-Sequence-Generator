#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <getopt.h>

typedef enum { false, true } bool;
#ifndef DEBUG
#define DEBUG 0
#endif
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
    if(strftime(cdt, 20, "%d_%m_%y_%H_%M", timeinfo) == 0)
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
void startBuildingFile(int customNumberToGen, bool customNumber){
    srand (time(NULL));
        const char *geneticTags[2] = {
            "AT","GC"
        };
        const char *geneticArray[1024];
        int count = 0;
        int numberToGen = 100000;
        int charCount = 0;

        char *filename = getname("generation_", ".fasta");
        FILE *file; 
        file = fopen(filename,"w+");
        
        fprintf(file,">AB000263\n");
        printf("Starting generation...\n");

        if(DEBUG)printf("customNumber:%i customBoolean:%i\n",customNumberToGen,customNumber );
        if (customNumber == true)
        {
            numberToGen = customNumberToGen;
        }
        printf("Generating %i combinations\n", numberToGen);
        while (count < numberToGen){
            count++;
            while (charCount < 25){
                charCount++;
                int n = rand()%2;
                fprintf(file,"%s", geneticTags[n]);
            }
            fprintf(file, "\n");
            charCount = 0;
            loadBar(count,numberToGen,100,100);
        }
        fprintf(file, "1");
        printf("Done. File written as %s\n", filename);
        fclose(file);
}
int main(int argc, char const *argv[])
{

    int opt;
    clock_t t;
    while((opt = getopt(argc, argv, "s:l:")) != -1) {
        switch (opt){
          case 's':
            startBuildingFile(0,false);
            break;

          case 'l':
            t = clock();
            startBuildingFile(atoi(optarg),true);
            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC;
            double minutes = ((double)t)/CLOCKS_PER_SEC / 60.0;

            if (time_taken > 60.0)
            {
                if (minutes > 60.0)
                {
                    printf("Wrote file in: %.2f hours\n", (time_taken / 60.0));
                }else {
                    printf("Wrote file in: %.2f minutes\n", (time_taken / 60.0));
                }
            }else if (time_taken < 60){
                printf("Wrote file in: %.2f seconds\n", time_taken);
            }
            break;

          default:
            printf("Genetic Sequence Generator\n");   
            printf("Prints out 100,000 random combinations of the four main base pairs (A,T,C,G)\n");
            printf("Writes to file in working directory\n");
            printf("Arguments;\n");
            printf("-s  Start generation with default number of combinations; 100,000\n");
            printf("-l  Start generation with a custom number of generations\n");
            printf("Example: ./gsg -s or ./gsg -l 18000\n");
        }
    }
    return 0;
}