/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define STK_CTRL ((volatile unsigned int *)(0xE000E010))  
#define STK_LOAD ((volatile unsigned int *)(0xE000E014))  
#define STK_VAL ((volatile unsigned int *)(0xE000E018)) 

#define GPIO_D 0x40020C00
#define GPIO_MODER  ((volatile unsigned int *) (GPIO_D))
#define GPIO_OTYPER  ((volatile unsigned short *) (GPIO_D+0x4))
#define GPIO_PUPDR ((volatile unsigned int *) (GPIO_D+0xC))
#define GPIO_IDR_LOW ((volatile unsigned char *) (GPIO_D+0x10))
#define GPIO_IDR_HIGH  ((volatile unsigned char *) (GPIO_D+0x11))
#define GPIO_ODR_LOW ((volatile unsigned char *) (GPIO_D+0x14))
#define GPIO_ODR_HIGH ((volatile unsigned char *) (GPIO_D+0x15))
#define SCB_VTOR ((volatile unsigned int *) (0xE000ED08))

#define SIMULATOR 1

#define SYSCFG_EXTICR1 ((short *) (0x40013880))
#define EXTI_IRM ((int *)(0x40013C00))
#define EXTI_RTSR ((int *)(0x40013C08))

void app_init(){
	*GPIO_MODER = 0x00005555; 
}

void main(void){
	*SYSCFG_EXTICR1 &= 0x0FFF;
	*SYSCFG_EXTICR1 |= 0x4000;
	*EXTI_IRM |= (1<<3);
	*EXTI_RTSR |= (1<<3);
	
	
}

