#include <stdlib.h>
#include <stdio.h>


void assertPuntatoreNonNull(void* puntatore, char* messaggioErrore) {
    if(puntatore == NULL) {
        printf("%s", messaggioErrore);
        exit(-1);
    }
}