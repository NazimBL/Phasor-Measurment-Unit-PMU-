#include "sim808.h"

const char APN[] = "internet";
const char APN_user[] = "ooredoo";
const char UDP_port[] = "5012";
const char PDC_IP[]="5.189.189.207";

char rx_buff[64];
char data_ready;
char data_len,tmp;

void UART1_Interrupt() iv IVT_INT_USART1 ics ICS_AUTO {
   tmp = UART1_Read();
   switch(tmp) {
     case LF:
        if(data_len > 1)
        {
          data_ready = 1;
        }
        break;
     case CR:
        rx_buff[data_len] = 0; 
        break;
     default:
        rx_buff[data_len] = tmp;
        data_len++;
   }

}

void GPS_Setup(){

 memset(rx_buff,0,64);
 UART3_Write_Text("GPS Power On");
 UART3_Write_Text("Please wait...");
 AT_Write("AT+CGPSPWR=1");                                                //change the working mode to 1
 while(!response_success());
 
 UART3_Write_Text("GPS Cold Reset");
 UART3_Write_Text("Please wait...");
 AT_Write("AT+CGPSRST=0");                                                //change the working mode to 1
 while(!response_success());
 
 UART3_Write_Text("GPS On !");

}

void Get_NMEA(){



}
void GPRS_Config(){

   char ap_cnt = 0;
   memset(rx_buff,0,64);   
   UART3_Write_Text("Setting STA mode");
   UART3_Write_Text("Please wait...");
   AT_Write("AT+CWMODE=1");                                                //change the working mode to 1
   while(!response_success());                                                    //wait to respond success 
   UART3_Write_Text("Setting connection mode");
   UART3_Write_Text("Please wait...");
   AT_Write("AT+CIPMUX=0");                                                //change the connection mode to 1
   while(!response_success());  
   
   UART3_Write_Text("Setting GPRS connection");
   UART3_Write_Text("Please wait...");
   AT_Write("AT+CGATT=1");                                                //change the connection mode to 1
   while(!response_success());  
      
   while(1) 
   {                                                              
     UART3_Write_Text("Connect to ISP");
     UART3_Write_Text("Please wait...");
     UART1_Write_Text("AT+CSTT=");
     UART1_Write('"');
     UART1_Write_Text(APN);
     UART1_Write('"');
     UART1_Write(',');
     UART1_Write('"');
     UART1_Write_Text(APN_user);
     UART1_Write('"');
     UART1_Write(',');
     UART1_Write('"');
     UART1_Write_Text(APN_user);
     UART1_Write('"');
     UART1_Write(CR);
     UART1_Write(LF);
     if(response_success())
     {
       UART3_Write_Text("Success\n\r");
       break;
     }else
     {
       UART3_Write_Text("Try again...");
       Delay_ms(1000);
     }
   }
   
    UART3_Write_Text("Setting GPRS connection");
    UART3_Write_Text("Please wait...");
    AT_Write("AT+CIICR");                                               
    while(!response_success());
    
    UART3_Write_Text("Connected start sending");
}

void Send_UDP(){

    UART3_Write_Text("Configure UDP connection\n\r");
    UART3_Write_Text("Please wait...\n\r");
    UART1_Write_Text("AT+CIPSTART=");
    UART1_Write("UDP");  
    UART1_Write(',');
    UART1_Write('"');
    UART1_Write_Text(PDC_IP);
    UART1_Write('"');
    UART1_Write(',');
    UART1_Write('"');
    UART1_Write_Text(UDP_port);
    UART1_Write('"');
    UART1_Write(CR);
    UART1_Write(LF);
    while(!response_success());
}

void Get_IP()
{
     char response_cnt, conn_flg;
     UART3_Write_Text("Getting IP Address\n\r");
     UART3_Write_Text("Please wait...\n\r");
     AT_Write("AT+CIFSR");
     while(!data_ready);
     reset_buff();
     while(!data_ready);
     //IP addres is stored in rx_buff
     UART3_Write_Text(rx_buff);
     reset_buff();
}

void AT_Write(char *CMD) 
{
   UART1_Write_Text(CMD);
   UART1_Write(0x0D);
   UART1_Write(0x0A);
}

char response_success() 
{
    char result;
    while(!data_ready);
    if((strstr(rx_buff,"OK")) || (strstr(rx_buff,"CONNECT")) || (strstr(rx_buff,"ready"))) {
       result = 1;
    }else result = 0;
    reset_buff();
    return result;
}

void reset_buff() 
{
  memset(rx_buff,0,data_len);
  data_ready = 0;
  data_len=0;
}

void Sim808Power(){
 
 GPIOB_ODRbits.ODR0=0;
 GPIOB_ODRbits.ODR0=1;
 Delay_ms(1200);
 GPIOB_ODRbits.ODR0=0;
} 

/*
 GPIOB_ODRbits.ODR0=0;
 Delay_ms(1200);
 GPIOB_ODRbits.ODR0=1; 
*/ 
