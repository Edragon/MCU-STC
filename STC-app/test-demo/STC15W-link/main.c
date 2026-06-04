#include "STC15W.h"
#include "intrins.h"

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



void delay(int i)
	{
	while(i) {
		Delay100ms(); i--;
	}
}
	
void main (void)
{		
		
		P13 = 1; //A
		P55 = 1; //B
		P33 = 1; //C
		P37 = 1; //D
		P11 = 1; //E
		P14 = 1; //F
		P32 = 1; //G
		P36 = 1; //.DP
	
		while(1){
			P12 = 0;
			delay(10);
			P12 = 1;
			delay(10);
			
			P15 = 0;
			delay(10);
			P15 = 1;
			delay(10);
			
			P54 = 0;
			delay(10);
			P54 = 1;
			delay(10);
			
		}

}