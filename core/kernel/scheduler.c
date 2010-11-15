#include <process.h>

typedef struct schedule_item
{
	uint8_t priority;
	thread_t* thread;
	struct schedule_item* next;
	uint8_t cycle;
	
} schedule_item_t;

schedule_item_t* schedule_stack;
schedule_item_t* schedule_current;

void scheduler_register_thread(thread_t *thread, uint8_t priority)
{
	schedule_item_t *item = (schedule_item_t *)kmalloc(sizeof(schedule_item_t));
	
	item->priority = priority;
	item->thread = thread;
	item->next = NULL;
	item->cycle = 0;
	
	schedule_item_t* temp = schedule_stack;
	while (temp)
		temp = temp->next;
		
	temp->next = item;
}

void scheduler_init(thread_t *kernel_thread)
{
	schedule_stack = kmalloc(sizeof(schedule_item_t));
	
	schedule_stack->priority = PRIORITY_HIGH;
	schedule_stack->thread = kernel_thread;
	schedule_stack->next = NULL;
	schedule_stack->cycle = 0;
	
	schedule_current = schedule_stack;
}

void scheduler_cycle()
{
	if (schedule_current->cycle == 0)
	{
		/* Switch to the next thread */
		schedule_item_t *next = schedule_current->next;
		if (!next)
			next = schedule_stack; // Loop back to beginning
			
		next->cycle = next->priority;
		
		thread_switch(next->thread);
	}
	else
	{
		/* This thread still has CPU time */
		schedule_current->cycle++;
	}
}
