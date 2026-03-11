#define BASE_ADDRESS 0x48000000UL
#define RCC 0x40021000UL

#define GPIO_MODER BASE_ADDRESS
#define RCC_AHBENR (RCC + 0x14UL)
#define GPIO_BSRR (BASE_ADDRESS + 0x18UL)

/* RCC_AHBENR Values */
#define GPIO_A_CLOCK_ENABLE (0x1UL << 17)

/* GPIO_BSRR Values */
#define GPIO_A_RESET (0x1UL << 16)

int main() {

	volatile unsigned long *RCC_AHBENR_POINTER = (volatile unsigned long *) RCC_AHBENR;

	*RCC_AHBENR_POINTER |= GPIO_A_CLOCK_ENABLE;

	volatile unsigned long *GPIO_MODER_POINTER = (volatile unsigned long *) GPIO_MODER;

	*GPIO_MODER_POINTER |= 0x1UL;

	volatile unsignned long *GPIO_BSRR_POINTER = (volatile unsigned long *) GPIO_BSRR;

	while (1) {
		*GPIO_BSRR_POINTER |= 0x1UL;
		for (volatile int i = 0; i < 150000; i++) {
		};
		*GPIO_BSRR_POINTER |= GPIO_A_RESET;
		for (volatile int i = 0; i < 150000; i++) {
		};
	};
};
