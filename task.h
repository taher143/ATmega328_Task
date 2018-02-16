/*
 * task.h
 *
 * Created: 2/16/2018 6:30:23 PM
 *  Author: embedded
 */ 


#ifndef TASK_H_
#define TASK_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define MAX_TASKS (10)

// task states
#define RUNNABLE (0x00)
#define RUNNING  (0x01)
#define STOPPED  (0x02)
#define ERROR    (0x03)

// a task "type"
// pointer to a void function with no arguments
typedef void (*task_t)(void);


// basic task control block (TCB)
typedef struct __tcb_t
{
	uint8_t id; // task ID
	task_t task; // pointer to the task
	// delay before execution
	uint16_t delay, period;
	uint8_t status; // status of task
} tcb_t;

// the task list
tcb_t task_list[MAX_TASKS];

// scheduler function definitions
void initScheduler(void);
void addTask(uint8_t, task_t, uint16_t);
void deleteTask(uint8_t);
uint8_t getTaskStatus(uint8_t);
void dispatchTasks(void);

#endif /* TASK_H_ */