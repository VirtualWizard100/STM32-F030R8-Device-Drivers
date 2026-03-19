#define CORTEX_M0_INTERNAL_PERIPHERALS_BASE_ADDRESS 0xE0000000
#define PRIVATE_PERIPHERALS_OFFSET 0xD000000
#define SYSTICK 0x0000003C
#define SYSTICK_TIMER (CORTEX_M0_INTERNAL_PERIPHERALS_BASE_ADDRESS + 0xE010UL)
#define STK_CSR SYSTICK_TIMER
#define STK_RVR (SYSTICK_TIMER + 0x4UL)
#define STK_CVR (SYSTICK_TIMER + 0x8UL)
#define STK_CALIB (SYSTICK_TIMER + 0xCUL)

#define CLOCK_ENABLE 0x1UL
#define CLOCK_SOURCE (0x1UL << 2)
#define COUNT_FLAG (0x1UL << 16)
#define TICK_INT (0x1UL << 1)
#define ONE_MS_LOAD 8000

#define DELAY 1000

#define GPIO_A_BASE_ADDRESS 0x48000000
#define RCC 0x40021000

#define RCC_AHBENR (RCC + 0x14UL)
#define GPIOA_MODER GPIO_A_BASE_ADDRESS
#define GPIOA_BSRR (GPIO_A_BASE_ADDRESS + 0x18UL)


int main(void) {

	volatile unsigned long *STK_RVR_POINTER = (volatile unsigned long *) STK_RVR;
	*STK_RVR_POINTER = (ONE_MS_LOAD - 1);

	volatile unsigned long *STK_CVR_POINTER = (volatile unsigned long *) STK_CVR;

	*STK_CVR_POINTER = 0;	//Initializing the CVR Register so that way the RVR loads it with it's value, and also clear the COUNTFLAG if it's set to 1

	volatile unsigned long *STK_CSR_POINTER = (volatile unsigned long *) STK_CSR;

	void systick_ms_delay(unsigned int delay) {
		*STK_CSR_POINTER = (CLOCK_ENABLE|CLOCK_SOURCE);	//Enable the SysTick timer counter and tells it that the timer is the processor

		for (int i = 0; i < DELAY; i++) {
			while ((*STK_CSR_POINTER & COUNT_FLAG) == 0) {	//When the CVR hits 0, the COUNTFLAG is set to 1, then the CVR starts at the max value again, and starts counting down again
			};
		};

		*STK_CSR_POINTER = 0;
	};


	volatile unsigned long *RCC_AHBENR_POINTER = (volatile unsigned long *) RCC_AHBENR;

	*RCC_AHBENR_POINTER |= (0x1UL << 17);	//Enable clock for GPIO A

	volatile unsigned long *GPIOA_MODER_POINTER = (volatile unsigned long *) GPIOA_MODER;

	*GPIOA_MODER_POINTER &= ~0x3UL;
	*GPIOA_MODER_POINTER |= 0x1UL;	//Set GPIO A to output

	volatile unsigned long *GPIOA_BSRR_POINTER = (volatile unsigned long *) GPIOA_BSRR;

	systick_ms_delay(DELAY);

	*GPIOA_BSRR_POINTER = (0x1UL << 16);

	*GPIOA_BSRR_POINTER = 0x1UL;

	systick_ms_delay(DELAY);

	*GPIOA_BSRR_POINTER = (0x1UL << 16);

	while (1){
	};
};

