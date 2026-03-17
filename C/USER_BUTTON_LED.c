#define BASE_ADDRESS 0x48000000UL
#define BASE_GPIO_C_ADDRESS 0x48000800UL
#define RCC 0x40021000UL

#define GPIO_A_MODER BASE_ADDRESS
#define GPIO_C_MODER BASE_GPIO_C_ADDRESS
#define GPIO_C_IDR (BASE_GPIO_C_ADDRESS + 0x10UL)
#define RCC_AHBENR (RCC + 0x14UL)
#define GPIO_ODR (BASE_ADDRESS + 0x14UL)
#define RCC_AHBRSTR (RCC + 0x28UL)
#define GPIO_BSRR (BASE_ADDRESS + 0x18UL)
#define GPIO_IDR (BASE_ADDRESS + 0x10UL)

/* RCC_AHBENR Values */
#define GPIO_A_CLOCK_ENABLE (0x1UL << 17)
#define GPIO_B_CLOCK_ENABLE (0x1UL << 18)
#define GPIO_C_CLOCK_ENABLE (0x1UL << 19)

/* GPIO_BSRR Values */
#define GPIO_A_RESET (0x1UL << 16)

/* GPIOC_IDR Values */
#define PIN_13 (0x1UL << 13)

int main(void) {
        volatile unsigned long *RCC_AHBENR_POINTER = (volatile unsigned long *) RCC_AHBENR;

        /* Enable the clock for GPIO A */
        *RCC_AHBENR_POINTER |= GPIO_A_CLOCK_ENABLE;
        *RCC_AHBENR_POINTER |= GPIO_C_CLOCK_ENABLE;

        volatile unsigned long *GPIO_A_MODER_POINTER = (volatile unsigned long *) GPIO_A_MODER;
        volatile unsigned long *GPIO_C_MODER_POINTER = (volatile unsigned long *) GPIO_C_MODER;

        /* Set GPIO A to output */
        *GPIO_A_MODER_POINTER |= 0x1UL;
        *GPIO_C_MODER_POINTER &= ~(0x3UL << 26);        /*Set Pin 13 to input */

        volatile unsigned long *GPIO_BSRR_POINTER = (volatile unsigned long *) GPIO_BSRR;
        volatile unsigned long *GPIO_C_IDR_POINTER = (volatile unsigned long *) GPIO_C_IDR;

        while (1) {
                if (*GPIO_C_IDR_POINTER & PIN_13) {
                        *GPIO_BSRR_POINTER |= GPIO_A_RESET;
                } else {
                        *GPIO_BSRR_POINTER |= 0x1UL;
                };

        };
};
