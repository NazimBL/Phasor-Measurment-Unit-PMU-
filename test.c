#include "ade7880.h"
  
  
  void main() {    
    SpiSetup();
    //debug serial
    UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART3_PB10_11);   
    UART1_Write_Text("Start");
  
    ADE_SpiEnable();
    ADE_Init();  
     
   
    while(1){
     
     
     checksum= ADE_Read32(CHECKSUM);
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
        
     Delay_ms(1000);
       
     }
      
}
