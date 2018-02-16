/*
 * task.c
 *
 * Created: 2/16/2018 6:30:38 PM
 *  Author: embedded
 */ 

#include "task.h"

// keeps track of number of timer interrupts
uint16_t count = 0;
// initializes the task list
void initScheduler(void)
{
	// use 1/8 of system clock frequency
	TCCR0B |= (1<<CS01);
	// initialize timer value = 0
	TCNT0 = 0;
	// enable Timer0 Overflow interrupt
	TIMSK0 |= (1<<TOIE0);
	
	for(uint8_t i=0; i<MAX_TASKS; i++)
	{
		task_list[i].id = 0;
		task_list[i].task = (task_t)0x00;
		task_list[i].delay = 0;
		task_list[i].period = 0;
		task_list[i].status = STOPPED;
	}
}

// adds a new task to the task list
// scans through the list and
// places the new task data where
// it finds free space
void addTask(uint8_t id, task_t task, uint16_t period)
{
	uint8_t idx = 0, done = 0x00;
	while( idx < MAX_TASKS )
	{
		if( task_list[idx].status == STOPPED )
		{
			task_list[idx].id = id;
			task_list[idx].task = task;
			task_list[idx].delay = period;
			task_list[idx].period = period;
			task_list[idx].status = RUNNABLE;
			done = 0x01;
		}
		if( done ) break;
		idx++;
	}
}

// remove task from task list
// note STOPPED is equivalent
// to removing a task
void deleteTask(uint8_t id)
{
	for(uint8_t i=0;i<MAX_TASKS;i++)
	{
		if( task_list[i].id == id )
		{
			task_list[i].status = STOPPED;
			break;
		}
	}
}

// gets the task status
// returns ERROR if id is invalid
uint8_t getTaskStatus(uint8_t id)
{
	for(uint8_t i=0;i<MAX_TASKS;i++)
	{
		if( task_list[i].id == id )
		return task_list[i].status;
	}
	return ERROR;
}

// dispatches tasks when they are ready to run
void dispatchTasks(void)
{
	for(uint8_t i=0;i<MAX_TASKS;i++)
	{
		// check for a valid task ready to run
		if( !task_list[i].delay &&
		task_list[i].status == RUNNABLE )
		{
			// task is now running
			task_list[i].status = RUNNING;
			// call the task
			(*task_list[i].task)();

			// reset the delay
			task_list[i].delay =
			task_list[i].period;
			// task is runnable again
			task_list[i].status = RUNNABLE;
		}
	}
}

// generates a "tick"
// each tick 100ms apart
ISR(TIMER0_OVF_vect)
{
	count ++;
	if( count == 392 )
	{
		count = 0;
		// cycle through available tasks
		for(uint8_t i=0;i<MAX_TASKS;i++)
		{
			if( task_list[i].status == RUNNABLE )
			task_list[i].delay--;
		}
	}
}
