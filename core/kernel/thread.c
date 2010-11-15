#include <process.h>
#include <mm.h>

thread_t current_thread;
uint32_t thread_id = 0; 

static void thread_exit();

thread_t* thread_kernel_create()
{
}

thread_t* thread_create(int (*fn)(void*), void *arg, uint32_t *stack)
{
	thread_t *thread = (thread_t *)kmalloc(sizeof(thread_t));
	memset(thread, 0, sizeof(thread_t));
	thread->id = thread_id++;
	
	*--stack = (uint32_t)arg;
	*--stack = (uint32_t)&thread_exit;
	*--stack = (uint32_t)fn;
	*--stack = 0;
	thread->ebp = (uint32_t)stack;
	thread->esp = (uint32_t)stack;
	
	return thread;
}

void thread_switch(thread_t* thread)
{
	
}

static void thread_exit()
{
}
