#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( "map.txt" , "rb" );
    if( !fp ) perror("map.txt"),exit(1);

    fseek( fp , 0L , SEEK_END);
    rewind( fp );

    char tab[31][28];
    char ch;
    int i = 0, j = 0;
    while (fgets(tab[i], 30, fp)){
        i++;
    }
    printf("\n");

    for(int i=0; i<31; i++){
        for(int j=0; j<28; j++){
            printf("%c", tab[i][j]);
        }
        printf("\n\r");
    }

    fclose(fp);
}