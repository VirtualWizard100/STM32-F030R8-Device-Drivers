#define BASE_ADDRESS 0x48000000UL
#define RCC 0x40021000UL

#define GPIO_A_MODER BASE_ADDRESS
#define RCC_AHBENR (RCC + 0x14UL)
#define GPIO_ODR_REGISTER (BASE_ADDRESS + 0x14UL)

int main(void) {

	volatile unsigned long *RCC_AHBENR_POINTER = (volatile unsigned long *) RCC_AHBENR;

	volatile unsigned long *GPIO_A_MODER_POINTER = (volatile unsigned long *) GPIO_A_MODER;

	volatile unsigned long *GPIO_ODR_REGISTER_POINTER = (volatile unsigned long *) GPIO_ODR_REGISTER;
	/**RCC_AHBENR_POINTER |= (0x1UL << 17);*/
	*GPIO_ODR_REGISTER_POINTER &= GPIO_ODR_REGISTER; //Clear data to GPIO_A
	*GPIO_A_MODER_POINTER &= GPIO_A_MODER; //Reset GPIO_A to modes
	for (volatile int i = 0; i < 100; i++) {
	};
	*RCC_AHBENR_POINTER &= RCC_AHBENR; //Disable clock for GPIO_A

	while (1) {
	};
};
