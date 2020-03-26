#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "serial.h"
#include "Sensori.h"
#include "NeoPixel.h"
#include "PWM-Motori.h"
#include "BNO.h"
#include "ADC.h"
#include "TWI.h"


/*--------------SENSORI------------------*/

int ValoriTensioneSharpLungo[]={530,524,518,511,505,499,496,495,479,471,464,454,448,445,440,436,426,420,415,410,405,399,392,385,379,356,359,353,351,347,341,335,331,327,322,319,312,307,304,300,291,290,289,283,282,281,274,270,268,265,260,257,256,255,251,250,247,243,241,238,236,235,230,228,227,225,223,222,219,218,216,215,212,211,208,207,205,203,202,200,199,198,197,196,193,191,190,189,188,185,183,182,180,179,178,177,176,175,173,172,171,170,169,168,167,166,162,159,158,157,156,155,154,152,150,149,148,147,146,145,144,143,142,140,139,138,137,136,135,134,132,131,130,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,110,109,107,106,105,104,103,102,101,100,99,98,97,96,95,94,91,90,89,88,88,87,86,85,84,83,82,81,80,79,78,77,76,75,0};
int ValoriDistanzaSharpLungo[]={200,205,210,215,220,225,230,235,240,245,250,255,260,265,270,275,280,285,290,295,300,305,310,315,320,325,330,335,340,345,350,355,360,365,370,375,380,385,390,395,400,405,410,415,420,425,430,435,440,445,450,455,460,465,470,475,480,485,490,495,500,505,510,515,520,525,530,535,540,545,550,555,560,565,570,575,580,585,590,595,600,605,610,615,620,625,630,635,640,645,650,655,660,665,670,675,680,685,690,695,700,705,710,720,730,735,740,745,750,760,765,770,780,785,790,800,805,810,820,825,830,835,840,845,850,860,870,875,880,890,895,900,910,915,920,930,950,960,965,975,980,985,990,995,1000,1010,1020,1030,1040,1050,1060,1070,1080,1090,1100,1110,1120,1130,1140,1160,1170,1190,1200,1220,1240,1250,1260,1270,1290,1300,1310,1330,1350,1360,1370,1380,1390,1410,1420,1430,1450,1470,1490,1500,0};
short int LungValTenSharpLungo = 184;


float UltimaDistanzaRilevata=1000;
float corto, lungo, differenza, distanza;
float SFoto;

void delay(int tempo){
	for(int i=0;i<tempo;i++)
		_delay_ms(1);
}



//luce

float Fotoresistenza(){
	SFoto=0;
	for(int i=0;i<5;i++){
		SFoto+=StartADC(7);
	}
	return (SFoto/5);
}

float DistanzaIR(uint8_t POS){//funzione per sensori sharp corti e lunghi
	 	float Valore=0;
 	for(int i=0;i<3;i++)
 	Valore += (float)StartADC(POS);
 	Valore=Valore/3;
	 
	 
	 
	if(POS!=DavantiLungo){
		Valore=((13*1023)/(5.0*Valore));
		if (Valore>35)
		{
			Valore=1000;
		}
	}else{
//  		Valore=61.681 * pow(Valore*5/1024,-1.133);
//  		if((Valore>135)||(Valore<25)){
// 	 		Valore=1000;
//  		}
 		float Distanza=10000;int temp;
 		if((Valore<ValoriTensioneSharpLungo[0])&&(Valore>ValoriTensioneSharpLungo[LungValTenSharpLungo-1])){
	 		for(temp=0;temp<LungValTenSharpLungo;temp++){
		 		if(ValoriTensioneSharpLungo[temp]==Valore){
			 		Distanza=ValoriDistanzaSharpLungo[temp];
			 		break;
		 		}
		 		if((ValoriTensioneSharpLungo[temp]>Valore)&&(ValoriTensioneSharpLungo[temp+1]<Valore))
		 		break;
	 		}
	 		if(Distanza==10000){
		 		float Divisore=(ValoriTensioneSharpLungo[temp]-ValoriTensioneSharpLungo[temp+1]);
		 		float Minuendo=(Valore-ValoriTensioneSharpLungo[temp+1])/Divisore;
		 		Minuendo*=ValoriDistanzaSharpLungo[temp];
		 		float Sottraendo= (Valore-ValoriTensioneSharpLungo[temp])/Divisore;
		 		Sottraendo*=ValoriDistanzaSharpLungo[temp+1];
		 		Distanza=Minuendo-Sottraendo;
	 		}
 		}
 		
 		if(Distanza>1400) Distanza=10000;
 		Valore= Distanza/10;
	}
	return Valore;
}
float DistanzaAvanti(){//distanza circa 0-100 cm
	corto = DistanzaIR(DavantiCorto);
	lungo=  DistanzaIR(DavantiLungo);
	differenza = ABS(lungo-corto);
	if((corto==1000) && (lungo!=1000))//Se il corto è in errore e il lungo no
	distanza=lungo;
	else if((corto!=1000) && (lungo!=1000) && (differenza<4))//se il lungo non è in errore, il corto no e la differenza è minore di 5
	distanza = (corto+lungo)/2;
	else if(((corto!=1000) && ((lungo==1000) || (differenza>=4))))//se il corto NON è in errore E (il lungo è in errore o la differenza è maggiore di 5)
	distanza=corto;
	else if((corto==1000) && (lungo==1000))//entrambi in errore
	distanza=1111;
	else if(UltimaDistanzaRilevata>=50/*cm*/)
	distanza=1000;
	else
	distanza=-1;
	UltimaDistanzaRilevata=distanza;
	return distanza;
}
/*float TemperaturaDX(){   //temperaturaTpa81DX
	int devDX=0xDE;//6F;//sens dx
	float valoreDX=0, v;
	for (int i=2;i<9;i++)
	{
		v=TWI_read(devDX,i);
		if((i==4)||(i==5))
		valoreDX+=v;
	}
	valoreDX/=2;
	return valoreDX;
}

float TemperaturaSX(){   //temperaturaTpa81SX
	int devSX=0xD0;//68;
	float valoreSX=0;
	float v;
	for (int i=2;i<9;i++)
	{
		v=TWI_read(devSX,i);
		if((i==4)||(i==5))
		valoreSX+=v;
	}
	valoreSX/=2;
	return valoreSX;
}*/
float TemperaturaDX(){   //temperaturaTpa81DX
	int devDX=0xDE;//6F;//sens dx
	float valoreDX=0, v=0;
	for (int i=2;i<9;i++)
	{
		v=TWI_read(devDX,i);
		if(valoreDX<v)
			valoreDX=v;
	}
Serial_Send_String("tempDX:");Serial_Send_Float(valoreDX);SerialN();
	return valoreDX;
}

float TemperaturaSX(){   //temperaturaTpa81SX
	int devSX=0xD0;//68;
	float valoreSX=0, v=0;
	for (int i=2;i<9;i++)
	{
		v=TWI_read(devSX,i);
		if(valoreSX<v)
			valoreSX=v;
}
Serial_Send_String("tempSX:");Serial_Send_Float(valoreSX);SerialN();
	return valoreSX;
}

float voltmetro() {
	int value=0;
	for(int i=0;i<20;i++){
		value+=StartADC(6);
	}
	value/=20;
	float R1=100000.0;//99300.0;
	float R2=10000.0;
	float vout = (value * 5.0) / 1024.0;
	float vin =  vout / (R2/(R1+R2));
	if (vin<0.09)
	vin=0.0;
	return vin;
}

uint8_t ColoreCasella(void){
	float valore1=Fotoresistenza();
	//rial_Send_String("val:");Serial_Send_Float(valore1);Serial_Send_String("     ");
	uint8_t Colore=Bianco;
	if(valore1<ArgentoMAX)
		Colore=Argento;
	else if(valore1<BiancoMAX)
		Colore=Bianco;
	else if(valore1<NeroMAX)
		Colore=Nero;
	return Colore;
}

/*-------------------------------------*/


/*--------------Pulsanti/finecorsa------------*/
uint8_t i, valore;

uint8_t pulsante(){
	valore=0;
	for(i=0;i<6;i++){
		if((PING & (1<<PG1))==0){
			valore=1;
			break;
		}
	}
	return valore;
}

uint8_t finecorsaDX(){
	 valore=0;
	for(i=0;i<6;i++){
		if((PINB & (1<<PB2))==0){
			valore=1;
			break;
		}
	}
	return valore;
}		
uint8_t finecorsaSX(){
	valore=0;
	for(i=0;i<6;i++){
		if((PINB & (1<<PB0))==0){
			valore=1;
			break;
		}
	}
	return valore;
}
uint8_t finecorsaCentrale(){
	valore=0;
	for(i=0;i<6;i++){
		if((PINB & (1<<PB1))==0){
			valore=1;
			break;
		}
	}
	return valore;
}

void initPulsanti(){
	DDRB&=~((1<<PB1)|(1<<PB2)|(1<<PB0));
	PORTB|=(1<<PB1)|(1<<PB2)|(1<<PB0);
	DDRG&=~((1<<PG1));
	PORTG|=(1<<PG1);
}
void stampaPulsFinecorsa(){
	if(pulsante())Serial_Send_String("Pulsante \n");
	if(finecorsaCentrale())Serial_Send_String("Pala Davanti \n");
	if(finecorsaDX())Serial_Send_String("Baffo Dx \n");
	if(finecorsaSX())Serial_Send_String("Baffo Sx \n");

}

uint8_t ultimainclinazione;

uint8_t inclinazione(){
	uint8_t casella=piano;
	float gradi=0;
	gradi=gradiGiroscopio(asseY);
//	_delay_ms(10);
	if((gradi>gradiSalita)||((gradi>gradiMinSalita)&&(ultimainclinazione==salita))){
		casella=salita;
	}
	else if((gradi<gradiDiscea)||((gradi<gradiMinDiscesa)&&(ultimainclinazione==discesa))){
		casella=discesa;
	}
	else{
		casella=piano;
	}
	ultimainclinazione=casella;
	return casella;
}

void verificaBatteria(){
	if(voltmetro()<valoreMinimoBatteria){
		StopMotori();
		Serial_Send_String("\n\n/////////////////////////////////////////BATTERIA SCARICA <");
		Serial_Send_Float(valoreMinimoBatteria);
		Serial_Send_String("///////////////////////////////\n\n");
		while(1){
			ColoreLED(RED);
			_delay_ms(200);
			ColoreLED(SPENTO);
			_delay_ms(300);
		}
	}
}

/*----------------------*/