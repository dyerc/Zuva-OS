#ifndef FILESYSTEM_H
#define FILESYSTEM_H

typedef struct
{
	uint32_t (*probe) (const char *device);
	uint32_t (*mount) (const char *device);
	
} fs_handles_t;


#endif
