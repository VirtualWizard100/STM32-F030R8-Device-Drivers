#include <stdint.h>
#include <stdio.h>

/* Base Addresses */
#define GPIOA_BASE_ADDRESS 0x48000000UL
#define RCC_BASE_ADDRESS 0x40021000UL

#define DMA_BASE_ADDRESS 0x40020000UL
#define USART1_BASE_ADDRESS 0x40013800UL



/* Reset and Clock Controller Registers */
#define RCC_AHBENR (RCC_BASE_ADDRESS + 0x14UL)
#define RCC_APB2ENR (RCC_BASE_ADDRESS + 0x18UL)

/* GPIO A Registers */
#define GPIOA_MODER GPIOA_BASE_ADDRESS				/* GPIO A Mode Register */
#define GPIOA_ODR (GPIOA_BASE_ADDRESS + 0x14UL)		/* GPIO A Output Data Register */
#define GPIOA_BSRR (GPIOA_BASE_ADDRESS + 0x18UL)	/* GPIO A Bit Set Reset Register */
#define GPIOA_AFRL (GPIOA_BASE_ADDRESS + 0x20UL)	/* GPIO A ALternate Function Register */

/* USART1 Registers */
#define USART1_CR1 USART1_BASE_ADDRESS				/* USART1 Control Register 1 */
#define USART1_BRR (USART1_BASE_ADDRESS + 0x0CUL)	/* USART1 Baud Rate Register */
#define USART1_ISR (USART1_BASE_ADDRESS + 0x1CUL)	/* USART1 Interrupt and Status Register */
#define USART1_TDR (USART1_BASE_ADDRESS + 0x28UL)	/* USART1 Transmit Data Register */


/* USART1 Control Register 1 Values */
#define USART1_CR1_UE 0x1UL				/* USART1 Control Register 1 USART Enable */
#define USART1_CR1_PCE (0x1UL << 10)	/* USART1 Control Register 1 Parity Control Enable */
#define USART1_CR1_PS (0x1UL << 9)		/* USART1 Control Register 1 Parity Selection */
#define USART1_CR1_TE (0x1UL << 3)		/* USART1 Control Register 1 Transmitter Enable */
#define USART1_CR1_RE (0x1UL << 2)		/* USART1 Control Register 1 Receiver Enable */
#define USART1_CR1_M1 (0x1UL << 28)		/* USART1 Control Register 1 Word Length */

/* USART1 Interrupt and Status Register */
#define USART1_ISR_TC (0x1U << 6)		/* USART1 Interrupt and Status Register Transmission Complete Flag */
#define USART1_ISR_TXE (0x1U << 7)		/* USART1 Interrupt and Status Register Transmit Data Register Empty Flag */
/* RCC Advanced High Performance Bus Enable Register Values */
#define GPIOA_CLOCK_ENABLE (0x1UL << 17)

/* RCC Advanced Peripheral Bus 2 Enable Register Values */
#define USART1_CLOCK_ENABLE (0x1UL << 14)

#define SYS_FREQ 8000000


#define APB2_CLK SYS_FREQ

#define DBG_SET_BAUDRATE 115200

void uart_init(void);
static void uart_write(int ch);

static void uart_set_baudrate(unsigned int periph_clk, unsigned int baudrate) {		// Calculates the baudrate divisor, then stores it in the Baudrate Register


	*((volatile uint32_t *) USART1_BRR) = ((periph_clk + (baudrate/2U)) / baudrate);;
};

static void uart_write(int ch) {		// Checks if the TDR (Transmission Data Register) is empty, once it is, it stores the next byte into the TDR
	while (!(*((volatile uint32_t *) USART1_ISR) & USART1_ISR_TXE)) {
	};

	*((volatile uint32_t *) USART1_TDR) = (ch & 0xff);

};


int __io_putchar(int ch) {				// Wrapper for uart_write that also returns the integer
	uart_write(ch);

	return ch;
};

void uart_str(char *str) {				// Infinitely calls __io_putchar with the next byte until str is empty
	while (*str) {
		__io_putchar(*str++);
	};
};


void uart_init(void) {
	*((volatile uint32_t *) RCC_AHBENR) |= GPIOA_CLOCK_ENABLE;					// Enable the clock for GPIO A

	*((volatile uint32_t *) RCC_APB2ENR) |= USART1_CLOCK_ENABLE;				// Enable the clock for USART1

	*((volatile uint32_t *) GPIOA_MODER) &= ~((0x3UL << 4) | (0x3UL << 6));		// Clear bits for PA2 and PA3

	*((volatile uint32_t *) GPIOA_MODER) |= ((0x1UL << 5)|(0x1UL << 7));		// Set PA2, and PA3 to Alternate Function Mode

//	*((volatile uint32_t *) GPIOA_MODER) |= (0x1UL << 7);// Set PA3 to Alternate Function Mode

	*((volatile uint32_t *) GPIOA_AFRL) &= ~((0xFU << 8)|(0xFU << 12));

	*((volatile uint32_t *) GPIOA_AFRL) |= (0x1UL << 8);						// Set PA2 to AF1 for USART1_TX

	*((volatile uint32_t *) GPIOA_AFRL) |= (0x1UL << 12);						// Set PA3 to AF1 for USART1_RX

	uart_set_baudrate(APB2_CLK, DBG_SET_BAUDRATE);								// Set the baudrate for USART1

	*((volatile uint32_t *) USART1_CR1) = USART1_CR1_TE;						// Clear USART1_CR1, and set the Transmitter Enable bit

	*((volatile uint32_t *) USART1_CR1) |= USART1_CR1_UE;						// Set the Uart Module Enable Flag
};

int main(void) {


	uart_init();

	(*(volatile uint32_t *) GPIOA_MODER) |= 0x1UL;
	(*(volatile uint32_t *) GPIOA_BSRR) = 0x1UL;

	while (1) {
		uart_str("Hello from STM32\r\n");
	};
};
