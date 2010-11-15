#ifndef LIB_MAP_H
#define LIB_MAP_H

#include <types.h>
#include <utils.h>

typedef struct array {
    int item_count;
    int max_item_count;
    int realloc_size;
    void** items;
} array_t;

int array_add_item( array_t* array, void* item );
int array_insert_item( array_t* array, int index, void* item );
int array_remove_item( array_t* array, void* item );
int array_remove_item_from( array_t* array, int index );
int array_get_size( array_t* array );
void* array_get_item( array_t* array, int index );
int array_index_of( array_t* array, void* item );
int array_make_empty( array_t* array );

int array_set_realloc_size( array_t* array, int realloc_size );

int array_init( array_t* array );
int array_destroy( array_t* array );

#endif 
