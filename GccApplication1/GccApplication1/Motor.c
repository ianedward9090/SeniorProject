#include <avr/interrupt.h>
#include <util/delay.h>


//#define RED1   PORTB0
//#define BLUE1  PORTB1
//#define BLACK1 PORTB2
//#define WHITE1 PORTB3

//#define RED2   _BV(PB4)
//#define BLUE2  _BV(PB5)
//#define BLACK2 _BV(PB6)
//#define WHITE2 _BV(PB7)

int clockwise(void);
int counterclockwise(void);

int main(void)
{
	int i=0;
	DDRB = 0xff;

	PORTB = 0;

	while(1){
		for(i=0; i<100;i++){
			clockwise(void);

		}
		_delay_ms(500);

		for(i=0; i<100;i++){
			counterclockwise(void);

		}


	}

	return 0;

}


int clockwise(void)
{
	PORTB = 0b00001010;
	_delay_ms(500);
	PORTB = 0b00001001;
	_delay_ms(500);
	PORTB = 0b00000101;
	_delay_ms(500);
	PORTB = 0b00000110;
	_delay_ms(500);
	return 0;
}

int counterclockwise(void)
{
	PORTB = 0b00000110;
	_delay_ms(500);
	PORTB = 0b00000101;
	_delay_ms(500);
	PORTB = 0b00001001;
	_delay_ms(500);
	PORTB = 0b00001010;
	_delay_ms(500);
	return 0;



}

