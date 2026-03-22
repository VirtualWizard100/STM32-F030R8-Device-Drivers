#include <stdint.h>

#define GPIOA_BASE_ADDRESS 0x48000000UL
#define RCC_BASE_ADDRESS 0x40021000UL
#define USART2_BASE_ADDRESS 0x40004400UL


#define RCC_AHBENR (RCC_BASE_ADDRESS + 0x14UL)
#define RCC_APB1ENR (RCC_BASE_ADDRESS + 0x1CUL)

#define GPIOA_MODER GPIOA_BASE_ADDRESS
#define GPIOA_AFRL (GPIOA_BASE_ADDRESS + 0x20UL)

#define USART2_CR1 USART2_BASE_ADDRESS
#define USART2_BRR (USART2_BASE_ADDRESS + 0xCUL)
#define USART2_ISR (USART2_BASE_ADDRESS + 0x1C)
#define USART2_TDR (USART2_BASE_ADDRESS + 0x28UL)


#define USART2_CR1_TE (0x1U << 3)
#define USART2_CR1_UE 0x1U

#define USART2_ISR_TXE (0x1U << 7)

#define SYS_FREQ 8000000

#define BUADRATE 115200

void uart_init() {

	*((volatile uint32_t *) RCC_AHBENR) |= (0x1UL << 17);	// Enable clock for GPIO A
	*((volatile uint32_t *) RCC_APB1ENR) |= (0x1U << 17);	// Enable clock for USART2

	*((volatile uint32_t *) GPIOA_MODER) &= ~((0x3UL << 4) | (0x3UL << 6));
	*((volatile uint32_t *) GPIOA_MODER) |= (0x1UL << 5);	// Set PA2 to Alternate Function mode
	*((volatile uint32_t *) GPIOA_MODER) |= (0x1UL << 7);	// Set PA3 to Alternate Function mode

	*((volatile uint32_t *) GPIOA_AFRL) &= ~((0xFUL << 8) | (0xFUL << 12));
	*((volatile uint32_t *) GPIOA_AFRL) |= ((0x1UL << 8) | (0x1UL << 12));	// Set PA2, and PA3 to Alternate Function 1 for USART2 TX, and RX

	*((volatile uint32_t *) USART2_CR1) |= USART2_CR1_TE;					// Enable Transmitter for USART2
	*((volatile uint32_t *) USART2_CR1) |= USART2_CR1_UE;					// Enable UART on USART2

};

void calculate_buadrate_divisor(uint32_t periph_clk, uint32_t baudrate) {
	*((volatile uint32_t *) USART2_BRR) = ((periph_clk + (baudrate/2U)) / baudrate);
};

void uart_write(int ch) {
	while (!((*(volatile uint32_t *) USART2_ISR) & USART2_ISR_TXE)) {
	};
	*((volatile uint32_t *) USART2_TDR) = (ch & 0xff);
};

int __io_putchar(int ch) {
	uart_write(ch);

	return ch;
};

void uart_str(char *str) {
	while (*str) {
		__io_putchar((int) *str++);

	};
};

int main() {

	uart_init();
	calculate_baudrate_divisor(SYS_FREQ, BAUDRATE);		// Calculate divisor for 115200
	while (1) {
		uart_str("Hello from STM32\r\n");
	};
};
