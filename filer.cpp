//#include <iostream>
//#include <fstream>
#include <stdio.h>

#include "filer.h"
#include "mainwindow.h"
#include "hardware.h"

using namespace std;

filer::filer() {

}

void filer::saveData( speed_array *spd ) {

    FILE *fp;

    fp = fopen( SPEED_FILE_NAME, "wb" );
    if ( NULL != fp ) {
        fwrite( spd, sizeof( speed_array), SPEED_ARRAY, fp );
        fclose(fp);
    } else {
        fprintf(stderr,"saveData failed opening file\n");
    }
}

bool filer::readData( speed_array *spd ) {

    FILE *fp;

    fp = fopen( SPEED_FILE_NAME, "rb" );
    if ( NULL != fp ) {
        fread( spd, sizeof( speed_array), SPEED_ARRAY, fp );
        fclose(fp);
        return true;
    }
    fprintf(stderr,"readData failed opening file\n");
    return false;
}
