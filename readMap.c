#include <stdio.h>
#include <stdlib.h>

void main(void){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( "map.txt" , "rb" );
    if( !fp ) perror("map.txt"),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
    fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    /* do your work here, buffer is a string contains the whole text */

    char tab[31][29];
    int j=0, k=0;

    for(int i=0; i<lSize; i++){
        if(buffer[i] == '\n'){
            tab[j][k] = ' ';
            j++;
            k=0;
            continue;
        }
        tab[j][k] = buffer[i];
        k++;
        

    }

    for(int i=0; i<31; i++){
        for(int j=0; j<29; j++){
            printf("%c", tab[i][j]);
        }

        printf("\n\r");
    }

    fclose(fp);
    free(buffer);
}