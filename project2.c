/*
 * project2.c
 *
 * Created: 4/18/2016 5:57:38 PM
 * Author : Carl Pacheco and Aisha Siddiq 
 */ 

#include "avr.h"
#include "lcd.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

struct Date {
	int month;
	int day;
	int year;
};

struct Time{
	int hour;
	int minute;
	int sec;
};

volatile unsigned char TimerFlag = 0; // ISR raises, main() lowers

struct Date date = {.day = 30, .month = 12, .year = 2016 };
struct Time time = {.sec = 59, .minute = 59, .hour = 24};

ISR(TIMER1_COMPA_vect)
{
	TimerFlag = 1;
}

unsigned char pressed(unsigned char r,unsigned char c){
	DDRC  = 0X0f;
	PORTC = 0xff;
	
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);

	wait_avr(1);
	
	if (GET_BIT(PINC, c+4))
	{
		return 0;
	}
	return 1;
}

unsigned char get_key(){
	unsigned char r, c;
	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			if(pressed(r, c))
			{
				return (r * 4) + c + 1; 
			}
		}
	}
	return 0;
}

int calculateDaysinMonth(){
	int daysInMonthArray[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int currentDaysInMonth = daysInMonthArray[date.month - 1];
	if (date.month == 2 && (date.year % 4) == 0) {
		currentDaysInMonth++;
	}
	return currentDaysInMonth;
}
void printTime(){
	char result[81];
	sprintf(result, "%02d/%02d/%04d                              %02d:%02d:%02d                                ", date.month, date.day, date.year, time.hour, time.minute, time.sec);
	puts_lcd2(result);
}

void calculateClock(){
		if(time.sec > 59){
			time.sec = 0;
			time.minute++;
		}
		if(time.minute > 59){
			time.minute = 0;
			time.hour++;
		}
		if(time.hour > 23){ //TODO make into 12 hour clock
			time.hour = 0;
			date.day++;
		}
		if(date.day > calculateDaysinMonth()){
			date.day = 1;
			date.month++;
		}
		if(date.month > 12){
			date.month = 1;
			date.year++;
		}
}

void enterMonth(struct Date *date){
	unsigned char keypadButton;
	int keypadPressed = 1;
	char printString[81];
	
	while(keypadButton != 16){
		sprintf(printString, "%-40s  %02d%-36s", "Month UP(A)", date->month, "");
		puts_lcd2(printString);
		
		keypadButton = get_key();
		if(!keypadPressed && keypadButton){
			keypadPressed = 1;
			if(keypadButton == 4){
				date->month++;	
			}
			else if(keypadButton == 8){
				date->month--;
			}
			if(date->month > 12){
				date->month = 1;
			}
			if(date->month < 1){
				date->month = 12;
			}
		}
		else if(!keypadButton){
			keypadPressed = 0;
		}
		
	}
}

void enterDay(){
	unsigned char keypadButton;
	int keypadPressed = 1;
	char printString[81];
	
	while(keypadButton != 16){
		sprintf(printString, "%-40s  %02d%-36s", "Day UP(A)", date.day, "");
		puts_lcd2(printString);
		
		keypadButton = get_key();
		if(!keypadPressed && keypadButton){
			keypadPressed = 1;
			if(keypadButton == 4){
				date.day++;
			}
			else if(keypadButton == 8){
				date.day--;
			}
			if(date.day > calculateDaysinMonth()){
				date.day = 1;
			}
			if(date.day < 1){
				date.day = calculateDaysinMonth();
			}
		}
		else if(!keypadButton){
			keypadPressed = 0;
		}
		
	}
}

void enterYear(struct Date *date){
	unsigned char keypadButton;
	int keypadPressed = 1;
	char printString[80];
	
	while(keypadButton != 16){
		sprintf(printString, "%-40s  %04d%-34s", "Year UP(A)", date->year, "");
		puts_lcd2(printString);
		
		keypadButton = get_key();
		if(!keypadPressed && keypadButton){
			keypadPressed = 1;
			if(keypadButton == 4){
				date->year++;
			}
			else if(keypadButton == 8){
				date->year--;
			}
			if(date->year > 9999){
				date->year = 1;
			}
			if(date->year < 1){
				date->year = 9999;
			}
		}
		else if(!keypadButton){
			keypadPressed = 0;
		}
		
	}
}

void enterHour(){
	unsigned char keypadButton;
	int keypadPressed = 1;
	char printString[80];
	
	while(keypadButton != 16){
		sprintf(printString, "%-40s  %02d%-36s", "Hour UP(A)", time.hour, "");
		puts_lcd2(printString);
		
		keypadButton = get_key();
		if(!keypadPressed && keypadButton){
			keypadPressed = 1;
			if(keypadButton == 4){
				time.hour++;
			}
			else if(keypadButton == 8){
				time.hour--;
			}
			if(time.hour > 23){
				time.hour = 0;
			}
			if(time.hour < 0){
				time.hour = 23;
			}
		}
		else if(!keypadButton){
			keypadPressed = 0;
		}
		
	}
}

void enterMinute(){
	unsigned char keypadButton;
	int keypadPressed = 1;
	char printString[80];
	
	while(keypadButton != 16){
		sprintf(printString, "%-40s  %02d%-36s", "Minute UP(A)", time.minute, "");
		puts_lcd2(printString);
		
		keypadButton = get_key();
		if(!keypadPressed && keypadButton){
			keypadPressed = 1;
			if(keypadButton == 4){
				time.minute++;
			}
			else if(keypadButton == 8){
				time.minute--;
			}
			if(time.minute > 59){
				time.minute = 0;
			}
			if(time.minute < 0){
				time.minute = 59;
			}
		}
		else if(!keypadButton){
			keypadPressed = 0;
		}
		
	}
}

void enterSecond(){
	unsigned char keypadButton;
	int keypadPressed = 1;
	char printString[80];
	
	while(keypadButton != 16){
		sprintf(printString, "%-40s  %02d%-36s", "Second UP(A)", time.sec, "");
		puts_lcd2(printString);
		
		keypadButton = get_key();
		if(!keypadPressed && keypadButton){
			keypadPressed = 1;
			if(keypadButton == 4){
				time.sec++;
			}
			else if(keypadButton == 8){
				time.sec--;
			}
			if(time.sec > 59){
				time.sec = 0;
			}
			if(time.sec < 0){
				time.sec = 59;
			}
		}
		else if(!keypadButton){
			keypadPressed = 0;
		}
		
	}
	time.sec--;
}
int main(void)
{
	ini_lcd();
	sei();
    TCCR1B |=(1<<CS12)|(1<<WGM12);
	TIMSK |= 1<<OCIE1A;

	OCR1A=31250;
	
	

	unsigned char keypadButton;

    while (1) 
    {
		keypadButton = get_key();
		
		if(keypadButton){
			if (keypadButton == 1){
				enterMonth(&date);
			}
			else if (keypadButton == 2){
				enterDay();
			}
			else if (keypadButton == 3){
				enterYear(&date);
			}
			else if (keypadButton == 5){
				enterHour();
			}
			else if (keypadButton == 6){
				enterMinute();
			}
			else if (keypadButton == 7){
				enterSecond();
			}
		}
		
		if(TimerFlag){
			TimerFlag = 0;
			time.sec++;
			calculateClock();
			printTime();
		}
   }	
}