#define CR  0x0D
#define LF  0x0A

void Sim808Power();

void GPS_Setup();
void Get_NMEA();

void Get_IP();
void GPRS_Config();
void Send_UDP();

void AT_Write(char *CMD);
char response_success();
void reset_buff();
