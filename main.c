#include "ade7880.h"
#include "sim808.h"
 

void SpiSetup();
void Sim_Init();
char txt[17];
unsigned long checksum=0;
unsigned long maxReg=0x514791;
unsigned int angle=0;
unsigned int status=0;
float ff3;
float Sscale=0.00390,Pscale=0.0038816111;
const unsigned  char DATASIZE=22;
unsigned long pmu[DATASIZE];
 
//flash gnd clk data vcc
//Sim_debug rx tx gnd
//Debug mosi miso sck cs

//DATA UPDATE ISR
void PPS_ISR() iv IVT_INT_EXTI0 ics ICS_AUTO {
      
      
              EXTI_PR.B0=1; // clear flag
              
              pmu[0]=getVRMS(2);
              pmu[1]=getIRMS(2);
              pmu[2]=getVRMS(1);
              pmu[3]=getIRMS(1);
              pmu[4]=getVRMS(0);
              pmu[5]=getIRMS(0);
              pmu[6]=getVA(2);
              pmu[7]=getWatt(2);
              pmu[8]=getVA(1);
              pmu[9]=getWatt(1);
              pmu[10]=getVA(0);
              pmu[11]=getWatt(0); 
                        
              pmu[12]=getPhaseShift(2);
              pmu[13]=getPhaseShift(1);
              pmu[14]=getPhaseShift(0);
                            
              pmu[15]=getPeriode(2);
              pmu[16]=getPeriode(1);
              pmu[17]=getPeriode(0);
                     
}

 
void main() {

    
    //SpiSetup();
    
    //debug serial
    UART3_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART3_PB10_11);   
    UART3_Write_Text("Start");
   
   //Sim_Init();
   //ADE_SpiEnable();
   //ADE_Init();  
   //Sim808Power();    
   
    while(1){
     
      UART3_Write_Text("testing module");
     // AT_Write("AT");                                                //change the connection mode to 1
     //while(!response_success());
     UART3_Write_Text("Success"); 
     /*
     checksum= ADE_Read32(CHECKSUM);
     //checksum= ADE_Read16(COMPMODE);
     LongLongUnsignedToHex(checksum, txt);
     UART1_Write_Text("checksum : ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');
    
     checksum=getIRMS(0);
     LongLongUnsignedToHex(checksum, txt);
     UART1_Write_Text("irms: ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');
         
     ff3=(float)checksum/maxReg;
     ff3*=5.9;
     sprintf(txt, "%12f", ff3);             
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r'); 
        
     checksum=getVRMS(0);
     LongLongUnsignedToHex(checksum, txt);
     UART1_Write_Text("vrms: ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');
    
     ff3=(float)checksum/maxReg;
     ff3*=220;
     ff3*=2.489;
     sprintf(txt, "%12f", ff3);             
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r'); 
     
     angle=getPhaseShift(0);  
     ff3=angle*0.0703;
     FloatToStr(ff3, txt); 
     UART1_Write_Text("phase degree: ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');
     
     checksum=getPeriode(0);
     IntToHex(checksum, txt);
     UART1_Write_Text("periode reg : ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');
     
     ff3=(float)checksum*3.90625;
     ff3/=1000;
     
     sprintf(txt, "%12f", ff3);             
     UART1_Write_Text("periode in mili sec: ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');
        */ 
     Delay_ms(1000);
       
     }
      
}

void SpiSetup(){
   
    GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_3| _GPIO_PINMASK_5);
    GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_4);
    GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_15);// Set CS# pin as Output
    GPIOA_ODRbits.ODR15=1; 
    AFIOEN_bit = 1;
    SWJ_CFG1_bit = 1; 
    SPI1_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT |_SPI_CLK_IDLE_HIGH |
    _SPI_SECOND_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_ENABLE | _SPI_SSM_DISABLE |
    _SPI_SSI_0, &_GPIO_MODULE_SPI1_PB345);
    AFIO_MAPR.SPI1_REMAP=1;

}

void Sim_Init(){

   UART1_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PB67); 
   //enable receive interrupt
   RXNEIE_USART1_CR1_bit = 1;
   NVIC_IntEnable(IVT_INT_USART1);
      
   //sim power key
   GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_0);
   GPIOB_ODRbits.ODR0=1;
    
   //PPS external interrupt setup
    GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_0);  
    
    RCC_APB2ENR.AFIOEN = 1;              // Enable clock for alternate pin functions
    AFIO_EXTICR1 = 0x0001;               // PB0 as External interrupt
    EXTI_RTSR = 0x00000400;              // Set interrupt on Rising edge
    EXTI_IMR |= 0x00000400;              // Set mask
    NVIC_IntEnable(IVT_INT_EXTI0);       // Enable External interrupt(argument is ISR address vector)

    EnableInterrupts();                  // Enables the processor interrupt.


}

/*      
     ADE_Write16(MASK0,0x0200); 
     status=ADE_Read16(MASK0); 
     while((status && 0x0200)!=0x0200);
     ADE_Write16(STATUS0,0x0200);     
      
           
     checksum=getVA(0);      
     ff3=(float)checksum*Sscale;
     sprintf(txt, "%12f", ff3);             
     UART1_Write_Text("S=: ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r'); 
     
     checksum=getWatt(0);   
     ff3=(float)checksum*Pscale;
     sprintf(txt, "%12f", ff3);             
     UART1_Write_Text("P=: ");
     UART1_Write_Text(txt);
     UART1_Write('\n');
     UART1_Write('\r');    
         
   */
