/**
 ******************************************************************************
 * @file      startup_coide.c
 * @author    Coocox
 * @version   V1.0
 * @date      20/07/2010
 * @brief     M0 M3 Devices Startup code.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the vector table entries with the exceptions ISR address
 *                - Initialize data and bss
 *                - Setup the microcontroller system. 			
 *                - Call the application's entry point.
 *            After Reset the Cortex-M0 M3 processor is in Thread mode,
 *            priority is Privileged, and the Stack is set to Main.
 *******************************************************************************
 */
 
 
/*----------Stack Configuration-----------------------------------------------*/  
#define STACK_SIZE       0x00000100      /*!< Stack size (in Words)           */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];      


/*----------Macro definition--------------------------------------------------*/  
#define WEAK __attribute__ ((weak))           


/*----------Declaration of the default fault handlers-------------------------*/  
/* System exception vector handler */
void WEAK  Reset_Handler(void);
void WEAK  NMI_Handler(void);
void WEAK  HardFault_Handler(void);
void WEAK  SVC_Handler(void);
void WEAK  DebugMon_Handler(void);
void WEAK  PendSV_Handler(void);
void WEAK  SysTick_Handler(void);

void WEAK WWDG_IRQHandler(void);            
void WEAK PVD_IRQHandler(void);              
void WEAK RTC_IRQHandler(void);        
void WEAK FLASH_IRQHandler(void);           
void WEAK RCC_CRS_IRQHandler(void);
void WEAK EXTI0_1_IRQHandler(void);                  
void WEAK EXTI2_3_IRQHandler(void);                  
void WEAK EXTI4_15_IRQHandler(void);    
void WEAK TSC_IRQHandler(void);    
void WEAK DMA1_Channel1_IRQHandler(void);   
void WEAK DMA1_Channel2_3_IRQHandler(void);   
void WEAK DMA1_Channel4_5_6_7_IRQHandler(void);   
void WEAK ADC1_COMP_IRQHandler(void);    
void WEAK LPTIM1_IRQHandler(void);    
void WEAK TIM2_IRQHandler(void);            
void WEAK TIM6_DAC_IRQHandler(void);           
void WEAK TIM21_IRQHandler(void);           
void WEAK TIM22_IRQHandler(void);             
void WEAK I2C1_IRQHandler(void);              
void WEAK I2C2_IRQHandler(void);            
void WEAK SPI1_IRQHandler(void);            
void WEAK SPI2_IRQHandler(void);            
void WEAK USART1_IRQHandler(void);          
void WEAK USART2_IRQHandler(void);             
void WEAK RNG_LPUART1_IRQHandler(void);    
void WEAK LCD_IRQHandler(void);    
void WEAK USB_IRQHandler(void);     
          



/*----------Symbols defined in linker script----------------------------------*/  
extern unsigned long _sidata;    /*!< Start address for the initialization 
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */    
extern unsigned long _edata;     /*!< End address for the .data section       */    
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */      
extern void _eram;               /*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/  
extern int main(void);           /*!< The entry point for the application.    */
extern void SystemInit(void);    /*!< Setup the microcontroller system(CMSIS) */
void Default_Reset_Handler(void);   /*!< Default reset handler                */
static void Default_Handler(void);  /*!< Default exception handler            */


/**
  *@brief The minimal vector table for a Cortex M3.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.  
  */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
  /*----------Core Exceptions------------------------------------------------ */
  (void *)&pulStack[STACK_SIZE],       /*!< The initial stack pointer         */
  Reset_Handler,                       /*!< The reset handler                 */
  NMI_Handler,                         /*!< The NMI handler                   */ 
  HardFault_Handler,                   /*!< The hard fault handler            */

  0,0,0,0,0,0,0,                       /*!< Reserved                          */
  SVC_Handler,                         /*!< SVCall handler                    */
  DebugMon_Handler,                    /*!< Debug monitor handler             */
  0,                                   /*!< Reserved                          */
  PendSV_Handler,                      /*!< The PendSV handler                */
  SysTick_Handler,                     /*!< The SysTick handler               */ 
  
  /*----------External Exceptions---------------------------------------------*/
	WWDG_IRQHandler,            
	PVD_IRQHandler,              
	RTC_IRQHandler,        
	FLASH_IRQHandler,           
	RCC_CRS_IRQHandler,
	EXTI0_1_IRQHandler,           
	EXTI2_3_IRQHandler,                 
	EXTI4_15_IRQHandler,   
	TSC_IRQHandler,    
	DMA1_Channel1_IRQHandler,   
	DMA1_Channel2_3_IRQHandler,   
	DMA1_Channel4_5_6_7_IRQHandler,   
	ADC1_COMP_IRQHandler,   
	LPTIM1_IRQHandler,   
	0, 
	TIM2_IRQHandler,   
	0,
	TIM6_DAC_IRQHandler,
	0,
	0,
	TIM21_IRQHandler,   
	0,
	TIM22_IRQHandler,                      
	I2C1_IRQHandler,            
	I2C2_IRQHandler,               
	SPI1_IRQHandler,            
	SPI2_IRQHandler,            
	USART1_IRQHandler,          
	USART2_IRQHandler,          
	RNG_LPUART1_IRQHandler,           
	LCD_IRQHandler,            
	USB_IRQHandler,
};


/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called. 
  * @param  None
  * @retval None
  */
void Default_Reset_Handler(void)
{
  /* Initialize data and bss */
  unsigned long *pulSrc, *pulDest;

  /* Copy the data segment initializers from flash to SRAM */
  pulSrc = &_sidata;

  for(pulDest = &_sdata; pulDest < &_edata; )
  {
    *(pulDest++) = *(pulSrc++);
  }

  /* Zero fill the bss segment. */
  for(pulDest = &_sbss; pulDest < &_ebss; )
  {
    *(pulDest++) = 0;
  }

  /* Setup the microcontroller system. */
  SystemInit();
	
  /* Call the application's entry point.*/
  main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler. 
  *       As they are weak aliases, any function with the same name will override 
  *       this definition.
  */
  
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler

#pragma weak WWDG_IRQHandler = Default_Reset_Handler            
#pragma weak PVD_IRQHandler = Default_Reset_Handler             
#pragma weak RTC_IRQHandler = Default_Reset_Handler        
#pragma weak FLASH_IRQHandler = Default_Reset_Handler           
#pragma weak RCC_CRS_IRQHandler = Default_Reset_Handler             
#pragma weak EXTI0_1_IRQHandler = Default_Reset_Handler                  
#pragma weak EXTI2_3_IRQHandler = Default_Reset_Handler                   
#pragma weak EXTI4_15_IRQHandler = Default_Reset_Handler   
#pragma weak TSC_IRQHandler = Default_Reset_Handler  
#pragma weak DMA1_Channel1_IRQHandler = Default_Reset_Handler   
#pragma weak DMA1_Channel2_3_IRQHandler = Default_Reset_Handler      
#pragma weak DMA1_Channel4_5_6_7_IRQHandler = Default_Reset_Handler    
#pragma weak ADC1_COMP_IRQHandler = Default_Reset_Handler            
#pragma weak LPTIM1_IRQHandler = Default_Reset_Handler                     
#pragma weak TIM2_IRQHandler = Default_Reset_Handler            
#pragma weak TIM6_IRQHandler = Default_Reset_Handler           
#pragma weak TIM21_IRQHandler = Default_Reset_Handler           
#pragma weak TIM22_IRQHandler = Default_Reset_Handler                       
#pragma weak I2C1_IRQHandler = Default_Reset_Handler              
#pragma weak I2C2_IRQHandler = Default_Reset_Handler               
#pragma weak SPI1_IRQHandler = Default_Reset_Handler            
#pragma weak SPI2_IRQHandler = Default_Reset_Handler            
#pragma weak USART1_IRQHandler = Default_Reset_Handler          
#pragma weak USART2_IRQHandler = Default_Reset_Handler          
#pragma weak RNG_LPUART1_IRQHandler = Default_Reset_Handler             
#pragma weak LCD_IRQHandler = Default_Reset_Handler            
#pragma weak USB_IRQHandler = Default_Reset_Handler

/**
  * @brief  This is the code that gets called when the processor receives an 
  *         unexpected interrupt.  This simply enters an infinite loop, 
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None  
  */
static void Default_Handler(void) 
{
  /* Go into an infinite loop. */
  while (1) 
  {
  }
}

/*********************** (C) COPYRIGHT 2009 Coocox ************END OF FILE*****/
