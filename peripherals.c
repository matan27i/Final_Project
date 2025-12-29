/* File: peripherals.c */
#include <aduc841.h>
#include "header.h"

void Timer3_Init(void) // Configure Timer 3 for 9600 Baud rate
{
    T3CON &= 0xFE; 
    T3CON |= 0x86; 
    T3FD = 0x08;   
}

void UART_Init(void) // Uart ON
{
    SM0 = 0;
    SM1 = 1;
    REN = 1;
    RI = 0; 
    TI = 0;
    ES = 1;
}

void GlobalINT(void) // Global Interrupt enable
{    
    EA = 1;  
}

void UART_ISR(void) interrupt 4 // ISR
{
    uint8_t rx_char;
    uint8_t parsed_val = 0xFF; 
    uint8_t i;

    if (RI)
    {
        RI = 0;
        rx_char = SBUF;

        // Filter Convert ASCII char to hexa
        if (rx_char >= '0' && rx_char <= '9')      
        {
            parsed_val = rx_char - '0';
        }
        else
					if (rx_char >= 'A' && rx_char <= 'F') 
        {
            parsed_val = rx_char - 'A' + 10;
        }

        
        if (parsed_val != 0xFF)
        {
            Snew = parsed_val; 

            if (buffer_count < MAX_PACKETS)
            {
                                    
                for (i = 0; i < HAMMING_R; i++)
                {
                    S_stream_expanded[(buffer_count * HAMMING_R) + i] = 
                        (parsed_val >> (HAMMING_R - 1 - i)) & 0x01;// split the number in binary to 4 Bytes in buffer
                }
                
                buffer_count++;
                
                if (buffer_count == 1) // only one char make interrupt
                {
                    buffer_flag = 1;
                }
            }
        }
    }
    if (TI) TI = 0; 
}