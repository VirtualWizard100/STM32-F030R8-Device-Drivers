#define BASE_ADDRESS 0x48000000UL
#define RCC 0x40021000UL

#define GPIO_A_MODER BASE_ADDRESS
#define RCC_AHBENR (RCC + 0x14UL)
#define GPIO_ODR_REGISTER (BASE_ADDRESS + 0x14UL)
#define RCC_AHBRSTR (RCC + 0x28UL)

int main(void) {

	volatile unsigned long *RCC_AHBENR_POINTER = (volatile unsigned long *) RCC_AHBENR;

	*RCC_AHBENR_POINTER |= (0x1UL << 17);

	volatile unsigned long *GPIO_A_MODER_POINTER = (volatile unsigned long *) GPIO_A_MODER;

	*GPIO_A_MODER_POINTER |= 0x1UL;

	volatile unsigned long *GPIO_ODR_POINTER = (volatile unsigned long *) GPIO_ODR_REGISTER;

	*GPIO_ODR_POINTER |= 0x1UL;

	while (1) {
	};
};
