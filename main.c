/* File: main.c 

Final project Stage 1 Input + Transmitter - "Decoder" implemition

DESCRIPTION: ASCII input via UART then converting characters into bit streams
send to x from s (multi) functiom stored in extended memory waiting to send to bus (pin 1?)
*/

#include <aduc841.h>
#include <string.h>
#include "header.h"

volatile uint8_t Snew = 0;
volatile uint8_t buffer_count = 0;
volatile bit buffer_flag = 0;

// Must be xdata to fit in memory , volatile make the compiler cheack if there changes
volatile uint8_t xdata S_stream_expanded[MAX_PACKETS * HAMMING_R]; 

// Output Buffer xdata - 15 * buffer size byte
uint8_t xdata X_stream_output[MAX_PACKETS * HAMMING_N];

uint8_t R_config_list[MAX_PACKETS];

void main(void)
{
    uint8_t i;
    
    GlobalINT();
    Timer3_Init();
    UART_Init();
    
    while(1)
    {
        if (buffer_flag)
        {
            ES = 0; 
            buffer_flag = 0; 
            
            get_multi_X_from_S((uint8_t*)S_stream_expanded, R_config_list, buffer_count, X_stream_output);         
            buffer_count = 0;// Reset buffer
            ES = 1; // Re-enable Interrupts
        }
    }
}