#ifndef COMMON_H
#define COMMON_H


#define PWM_RESOLUTION          4096.0
#define PWM_COUNT               4096
#define PWM_MAX                 4095

#define SLIDER_RESOLUTION       64

#define SPEED_ADJUSTMENT        32      // Half for now to solve crash if too high

#define SPEED_ARRAY             16      // Number of distinct speeds we can select

struct speed_array {
//    speed_array() {};
    int left;
    int right;
};
#endif // COMMON_H
