/*
 * 	startup.c
 *
 */
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

#ifdef SIMULATOR
#define DELAY_COUNT 10000
#else
#define DELAY_COUNT 10000000
#endif
static volatile int systick_flag;
static volatile int delay_count;
 
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}
__attribute__((naked)) unsigned int getPsrReg(void){
	__asm volatile ("MRS	R0,PSR\n");
	__asm volatile ("BX		LR\n");
}
__attribute__((naked)) void setPsrReg(unsigned int apsr){
	__asm volatile ("MSR	PSR_nvzcq,R0\n");
	__asm volatile ("BX		LR\n");

}

void delay_1mikro( void){
	*STK_CTRL = 0;
	*STK_LOAD =(168-1);
	*STK_VAL = 0;
	*STK_CTRL = 7;
} 

void delay( unsigned int count){ 
	if( count== 0) return;
	delay_count = count;
	systick_flag = 0;
	delay_1mikro();
}

void systick_irq_handler( void){
	*STK_CTRL = 0;
	delay_count--;
	if(delay_count > 0){
		delay_1mikro();
	}
	else{
		systick_flag = 1;
	}
} 

void app_init(void){
	*GPIO_MODER = 0x55555555;
	*SCB_VTOR = 0x2001C000;
	*((void (**)(void) ) 0x2001C03C ) = systick_irq_handler;
}



void main(void){

	app_init();
	*GPIO_ODR_LOW = 0;
	delay(DELAY_COUNT);
	*GPIO_ODR_LOW = 0xFF;
	while(1){
		if(systick_flag) break; // exception called through systick delay to handler function
	}
	
	*GPIO_ODR_LOW=0;
}