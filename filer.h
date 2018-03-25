#ifndef FILER_H
#define FILER_H

#include "common.h"

class filer {
public:
    explicit filer();

    void saveData( speed_array *spd );
    bool readData( speed_array *spd );
};

#endif // FILER_H
