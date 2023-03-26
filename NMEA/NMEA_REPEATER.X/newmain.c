/*
 * File:   NMEA.c
 * Author: AJAY
 *
 */


#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 16000000

void uart_init()
{
	TXSTA=0x24;//0b0010,0100
	RCSTA=0x90;
	SPBRG=103;
}

char receive()
{
	while(!RCIF);
	char b=RCREG;
	return b;
}

void transmit(char b)
{	
	TXREG=b;
	while(!TRMT);
}

void str_transmit(char *p)
{
	while(*p!='\0')
		{
			transmit(*p);
			p++;
            __delay_ms(10);
	 	}    
}

int str_receive()
{
	char c,lat[10],lon[11],g_check[]="$GPGGA",data[32],full_indx=0;
	int i=0,count=0,loncount=0,latcount=0,flag=0,lat_check=0;
    while(i<=190)
    {
        c=receive();
 
           /*GPS SECTION*/
         if(count==6||flag==1)
        {   
         flag=1;
         
         /*COMPLETE DATA*/
         data[full_indx]=c;
         
         /*LATITUDE*/
         if(lat_check>11&&latcount<=8)
         {
           lat[latcount]=c;
           latcount++;
           if(c==',')
               latcount--;
         }
         
         /*LONGITUDE*/
         if(lat_check>22&&loncount<=9)
         {
           lon[loncount]=c;
           loncount++;
           if(c==',')
               loncount--;
         }
         
         full_indx++;
         lat_check++;
         
         }
     
        if(full_indx>=37)
        {
            flag=0;
            full_indx=0;
        }
        
        if(c==g_check[count])
        {
            count++;   
        }
        else
        {
            count=0;
        }
        i++;
    }
// str_transmit(data);
str_transmit("latitude:");
str_transmit(lat);
str_transmit("longitude:");
str_transmit(lon);
str_transmit("\n\r");
return;
}


void main()
{
	TRISC6=0;//TX
	TRISC7=1;//RX
    RC7=0;RC6=0;
    TRISB0=1;
    uart_init();
    if(RB0==1)
    {//TRANSMITTER
//        char nmea[]=;
        while(1)
        {
            str_transmit("$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\n$GPGGA,164514.535,1006.412,N,07621.114,E,1,12,1.0,0.0,M,0.0,M,,*68\n$GPRMC,164514.535,A,1006.412,N,07621.114,E,,,181219,000.0,W*7E");
            __delay_ms(5000);
        }
    }
    else if(RB0==0)
    {//RECIEVER
        while(1)
        {	
            str_receive();
            __delay_ms(2000);
        }
    }
	

}