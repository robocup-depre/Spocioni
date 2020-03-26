#define F_CPU 16000000UL
#include <avr/io.h>
#include "PWM-Motori.h"

void initPWM(void)
{
	DDRE|=(1<<PE4)|(1<<PE5)|(1<<PE3);
	DDRH|=(1<<PH3)|(1<<PH4)|(1<<PH5);
	DDRA=0xFF;
	PORTA=0xFF;
 	TCCR3A|= (1<<COM3A1)|(0<<COM3A0)|(1<<COM3B1)|(0<<COM3B0)|(1<<COM3C1)|(0<<COM3C0)|(1<<WGM31)|(1<<WGM30);
 	TCCR3B|= (1<<WGM32)|(0<<WGM33);
 	TCCR4A|= (1<<COM4A1)|(0<<COM4A0)|(1<<COM4B1)|(0<<COM4B0)|(1<<COM4C1)|(0<<COM4C0)|(1<<WGM41)|(1<<WGM40);     // attiva il PWM a 10 bit TIMER 4
 	TCCR4B|= (1<<WGM42)|(0<<WGM43);
	startPWM();
}

void startPWM(void)
{
	
 	TCCR4B|= (1<<CS42)|(0<<CS41)|(0<<CS40);
 	TCCR3B|= (1<<CS32)|(0<<CS31)|(0<<CS30);
}

void stopPWM(void)
{
 	TCCR3B&=(0<<ICNC3)|(0<<ICES3)|(0<<WGM33)|(1<<WGM32)|(0<<CS32)|(0<<CS31)|(0<<CS30);
 	TCCR4B&=(0<<ICNC4)|(0<<ICES4)|(0<<WGM43)|(1<<WGM42)|(0<<CS42)|(0<<CS41)|(0<<CS40);
}

void StopMotori(){
	PORTA=0xFF;
}

void setSingoliPwm(float AvSX, float AvDX, float DiSX,float DiDX){
	if(AvDX!=-1) (int)(RegPwmMotAvDx=AvDX*topPwmMotAvDx);//1000	
	if(AvSX!=-1) (int)(RegPwmMotAvSx=AvSX*topPwmMotAvSx);//830
	if(DiDX!=-1) (int)(RegPwmMotDiDx=DiDX*topPwmMotDiDx);//1000
	if(DiSX!=-1) (int)(RegPwmMotDiSx=DiSX*topPwmMotDiSx);//830
}

void setTuttiPwm(float PWM){
	RegPwmMotAvDx=(int)(PWM*topPwmMotAvDx);//1000		//av dx
	RegPwmMotDiDx=(int)(PWM*topPwmMotDiDx);//830		//di dx
	RegPwmMotDiSx=(int)(PWM*topPwmMotDiSx);//1000		//di sx
	RegPwmMotAvSx=(int)(PWM*topPwmMotAvSx);//830			//av sx
}

void setCoppiaPwm(float DX, float SX){
	if(DX!=-1){
		RegPwmMotAvDx=(int)(DX*topPwmMotAvDx);//1000
		RegPwmMotDiDx=(int)(DX*topPwmMotDiSx);//1000
	}
	if(SX!=-1){
		RegPwmMotAvSx=(int)(SX*topPwmMotAvSx);//830	
		RegPwmMotDiSx=(int)(SX*topPwmMotDiSx);//830	
	}
}

void Avanti(){
	PORTA=PortaAvanti;

}

void Indietro(){
	PORTA= ~(PortaAvanti);
}

void RuotaDX(){
	PORTA=PortaRuotaDestra;
}

void RuotaSX(){
	PORTA=~PortaRuotaDestra;
}


// funzioni per gli encoder

void setTimerEncoder(){
	DDRD|=(0<<PD3)|(0<<PD2);
	EIMSK=(1<<INT3)|(1<<INT2);
	EICRA=(1<<ISC31)|(0<<ISC30)|(1<<ISC21)|(0<<ISC20);
	
}

void stopTimerEncoder(){
	EIMSK=(0<<INT3)|(0<<INT2);
}