//
#define F_CPU 16000000ul
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "serial.h"

long BAUD = (F_CPU/16/USART_BAUDRATE)-1;//103


float ABS(float x){
	if(x<0) x*=-1;
	return x;
}

float limita(float x, float I, float F){
	if(F>I){
		if(x<I)x=I;
		if(x>F)x=F;
		}else{
		if(x>I)x=I;
		if(x<F)x=F;
	}
	return x;
}



void Serial_Init(){
	/* Set baud rate */
 	UBRR0H = (unsigned char)(BAUD>>8);  // 9600 bit/s
 	UBRR0L = (unsigned char)BAUD;
	/* Enable receiver and transmitter */

	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit, none parity  "9600,N,8,1" */
	UCSR0C = 0b00000110;
} // USART_Init

void Serial1_Init()
{
UBRR1H = (unsigned char)(BAUD>>8);  
UBRR1L = (unsigned char)BAUD;
UCSR1B = (1<<RXEN1)|(1<<TXEN1);
UCSR1C = 0b00000110;
}
void Serial1_Tx(unsigned char data)
{
	while ( !( UCSR1A & (1<<UDRE1)) );
	UDR1=data;
}

unsigned char Serial1_Rx( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) );
	/* Get and return received data from buffer */
	return UDR1;
}
void Serial1_Send_String(char *string1)
{
	
	int i=0;
	for(i=0;i<strlen(string1);i++)
	{
		Serial1_Tx(string1[i]);
	}
}

void Serial1_Send_Float(float data)
{
	if(data<0)
	{
		Serial1_Send_String("-");
		data=-data;
	}
	int size=3;
	int var=data;
	float data1=data*pow(10,size);
	Serial1_Send_Int(var);
	Serial1_Send_String(",");
	var=var*pow(10,size);
	data1=data1-var;
	var=data1;
	Serial1_Send_Int(var);
}
void Serial1N()
{
Serial1_Send_String("\n");
}

void Serial1_Send_Int(int32_t num)
{
	if(num<0)
	{
		Serial1_Send_String("-");
		num=-num;
	}
	if(num==0){Serial1_Tx('0');}
	else
	{
		char str[32];				// definisce una stringa sulla quale convertire il numero da trasmettere (max 10 cifre)
		char i;						// contatore ciclo
		for(i=0;i<32;i++) str[i]=0; // cancella la stringa
		i=31;
		while (num)
		{
			str[i]=num%10+'0';		// converte il numero da trasmettere in una stringa (dalla cifra meno significativa)
			num/=10;
			i--;
		}
		for (i=0;i<32;i++)			// invia la stringa un carattere alla volta
		if (str[i]) Serial1_Tx(str[i]);
	}
}

int Serial1_Acquire()  //legge pi? dati RAW dalla porta RX0 e li converte in numero INT
{
	char buffer[16];
	uint16_t data=0;
	short k=0;
	
	while(!(UCSR1A&(1<<RXC1)));
	_delay_ms(1);
	
	do
	{
		buffer[k]=UDR1;
		k++;
		_delay_ms(0.5);
	}
	while(UCSR1A&(1<<RXC1));
	
	for(int s=0;s<k;s++)
	{
		data += round(pow(10,k-s-1)) * (buffer[s]-48);
	}
	return(data);
}

//Trasmissione seriale (un carattere)
void Serial_Tx(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0=data;//0b00001111 ;
}

//ricezione seriale (un carattere)
unsigned char Serial_Rx( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

//Trasmissione Seriale un numero intero
void Serial_Send_Int(int32_t num)
{
	if(num<0)
	{
		Serial_Send_String("-");
		num=-num;
	}
if(num==0){Serial_Tx('0');}
else
{
	char str[32];				// definisce una stringa sulla quale convertire il numero da trasmettere (max 10 cifre)
	char i;						// contatore ciclo
	for(i=0;i<32;i++) str[i]=0; // cancella la stringa
	i=31;
	while (num)
	{
		str[i]=num%10+'0';		// converte il numero da trasmettere in una stringa (dalla cifra meno significativa)
		num/=10;
		i--;
	}
	for (i=0;i<32;i++)			// invia la stringa un carattere alla volta
	if (str[i]) Serial_Tx(str[i]);
	}
}


void Serial_Send_String(char *string1)
{
	int i=0;
	for(i=0;i<strlen(string1);i++)
	{
			Serial_Tx(string1[i]);
	}
}
void SerialN()
{
Serial_Tx(13);
Serial_Tx(10);
}
void Serial_Send_Float(float data)
{
	if(data<0)
	{	
		Serial_Send_String("-");
		data=-data;
	}
	int size=3;
	int var=data;
	float data1=data*pow(10,size);
	Serial_Send_Int(var);
	Serial_Send_String(".");
	var=var*pow(10,size);
	data1=data1-var;
	var=data1;
	Serial_Send_Int(var);
}