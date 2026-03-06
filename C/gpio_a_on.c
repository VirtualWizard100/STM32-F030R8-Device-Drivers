#include <stdio.h>
#include <stdlib.h>

#define BASE_ADDRESS 0x40000000UL
#define PERIPHERAL_OFFSET  0x08000000UL
#define PERIPHERAL_BASE_ADDRESS (BASE_ADDRESS + PERIPHERAL_OFFSET)
#define GPIO_A PERIPHERAL_BASE_ADDRESS
#define GPIO_B (PERIPHERAL_BASE_ADDRESS + 0x0000400UL)
#define GPIO_C (PERIPHERAL_BASE_ADDRESS + 0x0000800UL)
#define GPIO_D (PERIPHERAL_BASE_ADDRESS + 0x0000C00UL)
#define RCC (BASE_ADDRESS + 0x0021000UL)
#define RCC_ENABLE_ADDRESS (RCC + 0x14UL)
#define GPIO_MODER PERIPHERAL_BASE_ADDRESS
#define GPIO_ODR_ADDRESS (PERIPHERAL_BASE_ADDRESS + 0x14UL)
#define BIT_SET_RESET_REGISTER (PERIPHERAL_BASE_ADDRESS + 0x18UL)

int main() {
	
	volatile unsigned long *RCC_ENABLE_POINTER = (volatile unsigned long *) RCC_ENABLE_ADDRESS;
	
	*RCC_ENABLE_POINTER |= (0x1UL << 17);	//Enable GPIO A
	
	volatile unsigned long *GPIO_A_POINTER = (volatile unsigned long *) GPIO_A;
	*GPIO_A_POINTER &= ~0x3UL; //Clear bits 0, and 1 (~3 = 0b00) 0, 1 & 00, = 00)
	
	*GPIO_A_POINTER |= 0x1UL;	//set GPIO A to output
	
	volatile unsigned long *GPIO_ODR_ADDRESS_POINTER = (volatile unsigned long *) GPIO_ODR_ADDRESS;

	*(volatile unsigned long *) BIT_SET_RESET_REGISTER = (0x1UL << 16);

	*GPIO_ODR_ADDRESS_POINTER |= 0x1UL;
	
	while (1) {};	//Keep on forever
	
};
