#ifndef TIME_H
#define TIME_H

#include <types.h>

typedef struct
{
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t min;
    uint32_t sec;
    
} time_detailed_t;

time_detailed_t* time_retrieve_detailed();
void time_init();
void time_change();

#endif