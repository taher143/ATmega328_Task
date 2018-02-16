/**
AVR Round Robbin Scheduler
Hardware: ATMega8 running at 8MHz
*/
// prototypes of tasks

#include "task.h"

void Task1(void);
void Task2(void);

// Task definitions

void Task1(void)
{
	static uint8_t status = 0x01;
	if( status )
	PORTB |= (1<<PORTB1);
	else
	PORTB &= ~(1<<PORTB1);
	status = !status;
}


void Task2(void)
{
	static uint8_t status = 0x01;
	if( status )
	PORTB |= (1<<PORTB0);
	else
	PORTB &= ~(1<<PORTB0);
	status = !status;
}

int main(void)
{
	
	// set PORTD bit0 and bit1 as outputs
	DDRB = (1<<PORTB0);
	// set up the task list
	initScheduler();

	// add tasks, id is arbitrary
	// task1 runs every 1 second
	//addTask(1, Task1, 10);

	// task2 runs every 4 seconds
	addTask(2, Task2, 10);

	// enable all interrupts
	sei();
	for(;;)
	dispatchTasks();
	return 0; // will never reach here
}