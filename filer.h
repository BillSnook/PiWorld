#ifndef FILER_H
#define FILER_H

#include "common.h"

class filer {
public:
    explicit filer();

    void saveData( speed_array *spd );
    void readData( speed_array *spd );
};

#endif // FILER_H
