/*
 * Codice Spocioni 3.0.c
 *
 * Created: 15/05/2019
 * Author : Pogetta Matteo
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "Movimenti.h"
#include "Sensori.h"
#include "NeoPixel.h"
#include "millis.h"
#include "PWM-Motori.h"
#include "Kit.h"
#include "BNO.h"
#include "TWI.h"
#include "ADC.h"

ISR(INT2_vect){
	counterInt2++;
	_delay_us(100);
//  	Serial_Send_String("2--");
//  	Serial_Send_Int(counterInt2);SerialN();
}

ISR(INT3_vect){
	counterInt3++;
	_delay_us(100);
//  	Serial_Send_String("3--");
//  	Serial_Send_Int(counterInt3);SerialN();
}

int main(void)
{	
	ColoreLED(RED);
	ColoreLED1(BLUE);
	Serial_Init();
	initPulsanti();
	initRilKit();
	initPWM();
	TWI_init();
	BNO_init();
	InitADC();
	init_millis(F_CPU);
	sei();
	//verificaBatteria();
	valoreMinimoBatteria=14.5; //in gara mettere 5
	LedGara=0;
	pwmAvanti=25;//15
	ultimoGira=180;
	statokit=1;
	posizioneIncerta=0;
	presenzaMuroSinistra=0;
	countFinecorsa=0;
	float DiAV,diAvDx, diAvSx,diDiDx,  diDiSx;	
	int contadestre=0;
	int ndestre=5;
Nsalite=0;
NAvEnc=0;
//  while(1){
// 	 RuotaDX();
// 	 setTuttiPwm(20);
//  }

	while (1)
	{
 		ColoreLED(GREEN);
  		diAvDx=DistanzaIR(DestraAvanti);
  		diDiDx=DistanzaIR(DestraDietro);
 		diAvSx=DistanzaIR(SinistraAvanti);
		diDiSx=DistanzaIR(SinistraDietro);
  		DiAV=DistanzaAvanti();
		setTuttiPwm(pwmAvanti);
		Serial_Send_String("MAIN nd:");Serial_Send_Int(contadestre);SerialN();
		if ((contadestre<ndestre)&&((diAvDx>20)||(diDiDx>20))){
			NAvEnc=0;
			ColoreLED(BLUE);
			if((diAvDx>=20)&&(diDiDx<20)){
				Serial_Send_String("MoDxAv\n");
				Avanti();
				while(diDiDx<20)
					diDiDx=DistanzaIR(DestraDietro);
				_delay_ms(250);
			}else if((diAvDx<20)&&(diDiDx>=20)){
			Serial_Send_String("MoDxIn\n");
				Indietro();
				while(diAvDx<20)
					diAvDx=DistanzaIR(DestraAvanti);
				_delay_ms(250);
			}
			if (((diAvSx+diDiSx)/2)<=13.0)
				presenzaMuroSinistra=1;
			Serial_Send_String("mainiDx\n");
			Gira(90);
			contadestre++;
			diAvSx=DistanzaIR(SinistraAvanti);
			if (diAvSx>=18.0)
				presenzaMuroSinistra++;
			if (presenzaMuroSinistra==2){
				Serial_Send_String("PosIncertaDx\n");
				Indietro();
				setTuttiPwm(30/*23*/);
				_delay_ms(600/*800*/);
				StopMotori();
				Avanti();
				setTuttiPwm(/*23*/30);
				_delay_ms(/*150*//*250*/300);
			}
			ColoreLED(SPENTO);
			parallelototale();
			presenzaMuroSinistra=0;
		}else if(DiAV<=10){
				NAvEnc=0;
				contadestre=0;
				if(diAvSx>18.0){		//gira sx
					Serial_Send_String("Sx\n");
					Gira(-90);
					Indietro();
					setTuttiPwm(20);
					_delay_ms(100);
					parallelototale();
				}else if(diAvDx>=diAvSx){	//verifica da che parte è meglio girare
					Serial_Send_String("180-Or\n");
					Gira(90);
					rilevaVittima();
					Gira(90);
					parallelototale();
				}else{
					Serial_Send_String("180-AntOr\n");
					Gira(-90);
					rilevaVittima();
					Gira(-90);
					parallelototale();
				}	
		}
		if (contadestre>=ndestre){
			contadestre=0;
			Serial_Send_String("Av30xDx>nd\n");
			Avanti30cm(pwmAvanti);
		}
		ColoreLED(SPENTO);
		rilevaVittima();
		Avanti30cm(pwmAvanti);
		rilevaVittima();
	}
}