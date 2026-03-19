#define TIM3_ARR_VAL	(8000 - 1)
#define TIM3_PSC_VAL	(1000 - 1)
#define TIM3_CNT_VAL	0

#define TIM3_BASE_ADDRESS 0x40000400UL
#define RCC_BASE_ADDRESS 0x40021000UL

#define RCC_APB1ENR (RCC_BASE_ADDRESS + 0x1CUL)				/* Advanced Peripheral Bus Clock Enable Register */

#define TIM3_CR1 TIM3_BASE_ADDRESS				/* TIM3 Control Register 1 */
#define TIM3_CR2 (TIM3_BASE_ADDRESS + 0x4UL)	/* TIM3 Control Register 2 */
#define TIM3_DIER (TIM3_BASE_ADDRESS + 0xCUL)	/* TIM3 DMA Interrupt Enable Register */
#define TIM3_SR (TIM3_BASE_ADDRESS + 0x10UL)	/* TIM3 Status Register */
#define TIM3_EGR (TIM3_BASE_ADDRESS + 0x14UL)	/* TIM3 Event Generation Register */
#define TIM3_CNT (TIM3_BASE_ADDRESS + 0x24UL)	/* TIM3 Counter Register */
#define TIM3_PSC (TIM3_BASE_ADDRESS + 0x28UL)	/* TIM3 Pre-Scalar Register */
#define TIM3_ARR (TIM3_BASE_ADDRESS + 0x2CUL)	/* TIM3 Auto Reload Register */

#define ARM-Cortex-M0-MHZ 8000000
#define MS 8000

/* RCC_APB1ENR Values */
#define TIM3_CLOCK_ENABLE (0x1UL << 1)

/* TIM3_CR1 Values */
#define TIM3_CR1_CEN 0x1UL	/* TIM3 Control Register 1 Counter Enable */

/* TIM3_SR Values */
#define TIM3_SR_UIF 0x1UL	/* TIM3 Status Register Update Interrupt Flag */

#define UEVENT_FREQUENCY (ARM-Cortex-M0-MHZ / ((TIM3_PSC_VAL + 1) * (TIM3_ARR_VAL + 1)))	// If the TIM3_PSC_VAL is 100, and the TIM3_ARR_VAL is 8000, then the UEVENT_FREQUENCY will be 1

#define GPIOA_BASE_ADDRESS 0x48000000

#define RCC_AHBENR (RCC_BASE_ADDRESS + 0x14UL)

#define GPIOA_MODER GPIOA_BASE_ADDRESS
#define GPIOA_BSRR (GPIOA_BASE_ADDRESS + 0x18UL)



void tim3_1hz_init(void) {
	*((volatile unsigned long *) RCC_APB1ENR) |= TIM3_CLOCK_ENABLE;	// Enable clock for TIM3

	/*
	 *  The reason for the Pre-Scalar is to make it to where you can slow the frequency down to a smaller number so you can actually
	 *	control the frequency of the UEVENT through the ARR because of it's limited size, since the register is only 16 bits, loading 8000
	 *	into it is possible, but not 8000000, so if you want the UEVENT Flag to set once every second you need to divide 8000000 by 1000 in the
	 *	Pre-Scalar to slow the frequency down to 8KHZ, then load 8000 into the ARR to make it to where the UEVENT Flag is set once every second,
	 *	or 1HZ
	 */

	*((volatile unsigned long *) TIM3_PSC) = TIM3_PSC_VAL;			// Set Pre-Scalar Register to 1000 for 8KHZ Clock Cycle

	*((volatile unsigned long *) ARR) = TIM3_ARR_VAL;				// Set Auto Reload Register to 8000 for 8KHZ for a 1HZ Event Cycle

	*((volatile unsigned long *) TIM3_CNT) = 0;						// Clear Counter Register

	*((volatile unsigned long *) TIM3_CR1) |= TIM3_CR1_CEN;			// Enable the Counter for TIM3
};



int main() {

	*((volatile unsigned long *) RCC_AHBENR) |= (0x1UL << 17);

	*((unsigned long *) GPIOA_MODER) |= 0x1UL;

	tim3_1hz_init();

	volatile unsigned long *GPIOA_BSRR_POINTER = (volatile unsigned long *) GPIOA_BSRR;

	volatile unsigned long *TIM3_SR_POINTER = (volatile unsigned long *) TIM3_SR;

	while (1) {
		while (!(*TIM3_SR_POINTER & TIM3_SR_UIF)) {
		};

		*TIM3_SR_POINTER &= ~TIM3_SR_UIF;

		*GPIOA_BSRR_POINTER = 0x1UL;

		while (!(*TIM3_SR_POINTER & TIM3_SR_UIF)) {
		};

		*TIM3_SR_POINTER &= ~TIM3_SR_UIF;

		*GPIOA_BSRR_POINTER = (0x1UL << 16);
	};
	return 0;

};
