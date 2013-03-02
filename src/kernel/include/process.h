#ifndef PROCESS_H
#define PROCESS_H

#define PRIORITY_HIGH 	4
#define	PRIORITY_NORMAL	2
#define	PRIORITY_LOW	1

#include <types.h>

typedef struct
{
	uint32_t id;
	uint32_t esp, ebp, ebx, esi, edi;
	
} thread_t;

#endif
