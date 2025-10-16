#include <avr/io.h>
#include <stdio.h>

#include "console.h"

// Calculate the USART baud rate register value
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

// Print a character to the USART
static int usart_putchar(char c, FILE *stream)
{
    // Wait for the transmit buffer to be empty
    while (!(USART0.STATUS & USART_DREIF_bm))
        ;

    // Put the character in the transmit buffer
    USART0.TXDATAL = c;

    return 0;
}

// Initialize a stdio stream for the USART
static FILE USART_stream = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

void console_init(uint32_t baud_rate)
{
    // Set the baud rate
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(baud_rate);

    USART0.CTRLC = (USART_CMODE_SYNCHRONOUS_gc + USART_PMODE_DISABLED_gc + USART_SBMODE_1BIT_gc + USART_CHSIZE_8BIT_gc);

    USART0.CTRLA |= USART_RXCIE_bm;

    // Set TX pin as output
    PORTA.DIR |= PIN1_bm;

    // Set RX pin as input
    PORTA.DIR &= ~PIN2_bm;

    // Enable the USART transmitter
    USART0.CTRLB |= (USART_TXEN_bm + USART_RXEN_bm);

    // Attach stdout to the USART stream
    stdout = &USART_stream;
}