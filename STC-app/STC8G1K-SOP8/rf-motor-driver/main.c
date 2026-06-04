#include "stc8g.h"
#include "intrins.h"

/*
 * STC8G1K08A-SOP8 Pinout for RF Motor Driver application:
 * Pin 1: P5.4 -> LED
 * Pin 2: VCC
 * Pin 3: P5.5 -> D2 (Input)
 * Pin 4: GND
 * Pin 5: P3.0 -> D1 (Input)
 * Pin 6: P3.1 -> D0 (Input)
 * Pin 7: P3.2 -> MOTOR_B (Output)
 * Pin 8: P3.3 -> MOTOR_A (Output)
 */

sbit LED     = P5^4;
sbit D2      = P5^5;
sbit D1      = P3^0;
sbit D0      = P3^1;
sbit MOTOR_B = P3^2;
sbit MOTOR_A = P3^3;

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void Delay(unsigned int ms)
{
    while(ms--)
    {
        Delay1ms();
    }
}

void GPIO_Init()
{
    // STC8G GPIO Mode Configuration:
    // PnM1 PnM0 | Mode
    // 0    0    | Standard 8051 (Weak Pull-up)
    // 0    1    | Push-Pull Output (Strong Pull-up)
    // 1    0    | Pure Input (High-Impedance)
    // 1    1    | Open-Drain Output

    // P3 configuration:
    // P3.0(D1), P3.1(D0) - Input (High-Z)
    // P3.2(MOTOR_B), P3.3(MOTOR_A) - Output (Push-Pull)
    P3M1 = 0x03; // 0000 0011 -> P3.1, P3.0 Input
    P3M0 = 0x0C; // 0000 1100 -> P3.3, P3.2 Push-Pull

    // P5 configuration:
    // P5.4(LED) - Output (Push-Pull)
    // P5.5(D2) - Input (High-Z)
    P5M1 = 0x20; // 0010 0000 -> P5.5 Input
    P5M0 = 0x10; // 0001 0000 -> P5.4 Push-Pull
}

void main()
{
    unsigned char blink_timer = 0;
    GPIO_Init();
    
    // Initial states
    LED = 1;     // Active low LED off
    MOTOR_A = 0; // Motor off
    MOTOR_B = 0; // Motor off

    while (1)
    {
        // RF Control Logic:
        // D2 high -> Motor Forward (Both A & B high), LED Constant ON (Low)
        // D1 high -> Turn Right (MOTOR_A high, MOTOR_B low), LED Blink
        // D0 high -> Turn Left (MOTOR_A low, MOTOR_B high), LED Blink
        
        if (D2) 
        {
            MOTOR_A = 1;
            MOTOR_B = 1;
            LED = 0; // Constant ON
        }
        else if (D1 || D0)
        {
            if (D1)
            {
                MOTOR_A = 1;
                MOTOR_B = 0;
            }
            else // D0
            {
                MOTOR_A = 0;
                MOTOR_B = 1;
            }
            
            // Blink logic
            blink_timer++;
            if (blink_timer > 5) // Adjust for speed
            {
                LED = !LED;
                blink_timer = 0;
            }
        }
        else
        {
            MOTOR_A = 0;
            MOTOR_B = 0;
            LED = 1; // OFF
            blink_timer = 0;
        }

        Delay(20); // Loop timing
    }
}
