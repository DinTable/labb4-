/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void ){
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

/*
 * Systick  
 */
#define STK_CTRL ((volatile unsigned int *)(0xE000E010))  
#define STK_LOAD ((volatile unsigned int *)(0xE000E014))  
#define STK_VAL ((volatile unsigned int *)(0xE000E018)) 

#define GPIO_D 0x40020C00
#define GPIO_D_MODER  ((volatile unsigned int *) (GPIO_D))
#define GPIO_D_OTYPER  ((volatile unsigned short *) (GPIO_D+0x4))
#define GPIO_D_PUPDR ((volatile unsigned int *) (GPIO_D+0xC))
#define GPIO_D_IDR_LOW ((volatile unsigned char *) (GPIO_D+0x10))
#define GPIO_D_IDR_HIGH  ((volatile unsigned char *) (GPIO_D+0x11))
#define GPIO_D_ODR_LOW ((volatile unsigned char *) (GPIO_D+0x14))
#define GPIO_D_ODR_HIGH ((volatile unsigned char *) (GPIO_D+0x15))

#define GPIO_E 0x40021000
#define GPIO_E_MODER  ((volatile unsigned int *) (GPIO_E))
#define GPIO_E_OTYPER  ((volatile unsigned short *) (GPIO_E+0x4))
#define GPIO_E_PUPDR ((volatile unsigned int *) (GPIO_E+0xC))
#define GPIO_E_IDR_LOW ((volatile unsigned char *) (GPIO_E+0x10))
#define GPIO_E_IDR_HIGH  ((volatile unsigned char *) (GPIO_E+0x11))
#define GPIO_E_ODR_LOW ((volatile unsigned char *) (GPIO_E+0x14))
#define GPIO_E_ODR_HIGH ((volatile unsigned char *) (GPIO_E+0x15))


#define SCB_VTOR ((volatile unsigned int *) (0x2001C000))

#define SIMULATOR 1

#define SYSCFG_EXTICR1 ((short *) (0x40013808))
#define EXTI_IRM ((int *)(0x40013C00))
#define EXTI_RTSR ((int *)(0x40013C08))
#define EXTI_PR ((int *)(0x40013C14))
#define NVIC_ISER0 ((int *)(0xE000E100))
#define EXTI3_IRQ (0x2001C000 + 0x00000064)

static int count = 0; 

void irq_handler(void){
	*EXTI_PR |= (1<<3); //kvitterar avbrott
	if(*GPIO_E_IDR_LOW & 0x1){
		*GPIO_E_IDR_LOW |= (1<<4);
		count++;
	}
	else if(*GPIO_E_IDR_LOW & 0x2){
		*GPIO_E_IDR_LOW |= (1<<5);
		count = 0;
	}
	else if(*GPIO_E_IDR_LOW & 0x4){
		*GPIO_E_IDR_LOW |= (1<<6);
		
		*GPIO_D_MODER = 0x00000000;
		if(*GPIO_D_ODR_LOW){
			*GPIO_D_ODR_LOW = 0;
		}
		*GPIO_D_MODER = 0x00005555;
	}
	else{
		*GPIO_D_ODR_LOW = 0xFF;
	}
}

void app_init(){
	*GPIO_D_MODER = 0x00005555;	//initierar PD0-7
	*GPIO_E_MODER = 0x55555500;	
	
	
	*SYSCFG_EXTICR1 &= ~0xF000;  //nollställer bitfält för EXTI3 
	*SYSCFG_EXTICR1 |= 0x4000;	//port E till  EXTI3
	
	*EXTI_IRM |= (1<<3);		//aktiverar avbrottslina 3
	*EXTI_RTSR |= (1<<3);		//aktivera avbrott på positiv flank på avbrottslina 3
	
	*NVIC_ISER0 |= (1<<9);		//aktiverar avbrott från 
	*((void (**)(void)) EXTI3_IRQ) = irq_handler; //kopplar avbrottsrutin till EXTI3
}

void main(void){
	app_init();
	int bit;
	while(1){
		*GPIO_D_ODR_LOW = count; 
		
	}
}