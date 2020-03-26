#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "Movimenti.h"
#include "Sensori.h"
#include "serial.h"
#include "NeoPixel.h"
#include "PWM-Motori.h"
#include "millis.h"
#include "Kit.h"
#include "BNO.h"
/*--------------FUNZIONI MOTORI----------*/
float GIniziali, GUltimo, GAdesso, GMancanti, m, velGyZ;
int k, contatoreGira;

int contatoreParallelo;
float DiAv, DiDi, differenza;

float tDx, tSx;

uint8_t FC, FDx, FSx;

void Gira(float Gradi){
	Serial_Send_String("Gira:");Serial_Send_Float(Gradi);SerialN();
	stopTimerEncoder();
	StopMotori();
	GIniziali=gradiGiroscopio(asseZ);
	k=0; contatoreGira=0;
	GUltimo=GIniziali;
	m=((float)pwmGiraInizio-pwmGiraFine)/(gradiretta);
	(Gradi>0)?(RuotaDX()):(RuotaSX());
	setTuttiPwm(pwmGiraInizio);
	do{
// 		Serial_Send_String("GA:");Serial_Send_Float(GAdesso);
// 		Serial_Send_String(" GM:");Serial_Send_Float(GMancanti);
// 		Serial_Send_String(" GI:");Serial_Send_Float(GIniziali);
// 		Serial_Send_String(" CO:");Serial_Send_Int(contatoreGira);
// 		SerialN();
		GAdesso=gradiGiroscopio(asseZ);
		if((GAdesso-GUltimo)>180) k--;
		else if((GAdesso-GUltimo)<-180) k++;
		GMancanti=Gradi-GAdesso-(k*360)+GIniziali;
		if(Gradi<0)GMancanti*=-1;
		velGyZ=accelerazioneAngolare(asseZ);
		 if(velGyZ<=1.5){
		 	contatoreGira++;
		//	Serial_Send_String("contatore Gira:");Serial_Send_Int(contatoreGira);SerialN();
 		 		if((contatoreGira<700)&&(contatoreGira>450)/*100*/){
			  Serial_Send_String("B1Gira");
 		 		setTuttiPwm(pwmBoostgy1);
 		 	}else if(contatoreGira>=700){
	 		 	Serial_Send_String("B2Gira");
 		  		setTuttiPwm(pwmBoostgy2);
 				(Gradi>0)?(RuotaSX()):(RuotaDX());
 				delay(tempoRInBoostgy2);
 		  		Avanti();         
 				delay(tempoAvBoostgy2);
 		  		(Gradi>0)?(RuotaDX()):(RuotaSX());
 		 		contatoreGira=10;
 		 	}
		}else if(ABS(Gradi)<15){
			setTuttiPwm(19);
		}else if((GMancanti<=gradiretta)&&(GMancanti>=-5))
			setTuttiPwm((GMancanti*m)+pwmGiraFine);
		else
			setTuttiPwm(pwmGiraInizio);
		GUltimo=GAdesso;
	}while(GMancanti>0);
SerialN();
	
	
	
	GAdesso=gradiGiroscopio(asseZ);
	if((GAdesso-GUltimo)>180){ k--;}
	else if((GAdesso-GUltimo)<-180) k++;
	GMancanti=Gradi-GAdesso-(k*360)+GIniziali;
	if(Gradi<0)GMancanti*=-1;
//	Serial_Send_Float(GMancanti);SerialN();
	if(ABS(GMancanti)>1){
// 		Serial_Send_String("correzione: ");
//  		Serial_Send_Float(GMancanti);
//  		SerialN();
		if(Gradi<0) GMancanti*=-1;
		Gira(GMancanti);
	}
 	(Gradi<0)?(RuotaDX()):(RuotaSX());
 	setTuttiPwm(99);
 	_delay_ms(50);
	StopMotori();
	if((90==ABS(Gradi)||(Gradi==180)))
		ultimoGira=Gradi;
	
}

void parallelototale(){
	Serial_Send_String("Parallelo \n");
	
	ColoreLED(WHITE);
	for (int i=-1;i<=1;i+=2)
	{
		contatoreParallelo=0;
		StopMotori();
		if(i==1){
			DiAv=DistanzaIR(DestraAvanti);
			DiDi=DistanzaIR(DestraDietro)-0.3;
		}else{
			DiAv=DistanzaIR(SinistraAvanti);
			DiDi=DistanzaIR(SinistraDietro)-0.3;
		}
		differenza = (DiAv-DiDi)*i;
		while(!((ABS(differenza)<tolleranzaParallelo)||(ABS(differenza)>tolleranzaMassimaParallelo)
		||(contatoreParallelo>2000)||(((DiAv+DiDi)/2)>distanzaMinimaParalleo)))
		{
			contatoreParallelo++;
			//Serial_Send_String("Paralleo:"); Serial_Send_Int(contatoreParallelo);SerialN();
			if(contatoreParallelo>1500)
				setTuttiPwm(pwmBoostParallelo);
			else
				setTuttiPwm(pwmParallelo);		
		    if(i==1){
			    DiAv=DistanzaIR(DestraAvanti);
			    DiDi=DistanzaIR(DestraDietro)-0.3;
		    }else{
			    DiAv=DistanzaIR(SinistraAvanti);
			    DiDi=DistanzaIR(SinistraDietro)-0.3;
		    }
		    differenza = (DiAv-DiDi)*i;
		    (differenza>0)?(RuotaDX()):(RuotaSX());
		}
		StopMotori();
		ColoreLED(SPENTO);
	}
}


uint8_t ControllaCasellaNera(short int azione, int statoazione, float vel){
	uint8_t valore=0;
	if(ColoreCasella()==Nero){
		
		Serial_Send_String("NERO ");
		valore=1;
		ColoreLED(PINKColor);
		setTuttiPwm(vel);
		Avanti();
		_delay_ms(250);//250
		setTuttiPwm(vel+10);
		Indietro();
		_delay_ms(500);//400
		int x=0;
		if(azione==1)
		{
			Serial_Send_String("DistEncod\n");
			counterInt2=0;
		counterInt3=0;
			while(!((x>100)||((counterInt2>=statoazione)&&(counterInt3>=statoazione)))){
				_delay_us(200);
				x++;
			}
		}
		else if(azione==2){
			
			Serial_Send_String("DistIR\n");
			float distanzaCorrente = 0;
			do
			{
				distanzaCorrente = DistanzaAvanti();
			}while (distanzaCorrente<statoazione-4);
		}
		parallelototale();
		if((DistanzaIR(SinistraAvanti)>20)||(DistanzaIR( SinistraDietro)>20)){
			
			Gira(-90);
			parallelototale();
			Avanti30cm(pwmAvanti);
		}
		else
			Gira(180);

		parallelototale();
		ColoreLED(SPENTO);
	}
	return valore;
}


uint8_t rilevaVittima(){
	uint8_t valore =0;
	if(statokit==1){
		int tempo=tempMovRilKit;
		tSx=TemperaturaSX();
		float diSx= (DistanzaIR(SinistraDietro)+DistanzaIR(SinistraAvanti))/2;
		if ((tSx>=temperaturaMinVittime)&&(diSx<20)){
			Serial_Send_String("vittSx\n");
			valore=1;
			statokit=0;
			rilasciokit(1);
			Gira(90);
			Indietro();
			setTuttiPwm(20);
			delay(tempo);
			StopMotori();
			while((statoGenRil==1)||(statoRilascio==0)){
				_delay_ms(5);
			}
			Avanti();
			setTuttiPwm(20);
			delay(tempo);
			Gira(-90);
			StopMotori();
		}
		tDx=TemperaturaDX();
		float diDx= (DistanzaIR(DestraDietro)+DistanzaIR(DestraAvanti))/2;
		if ((tDx>=temperaturaMinVittime)&&(diDx<20)){
			Serial_Send_String("vittDx\n");
			valore=1;
			statokit=0;
			rilasciokit(1);
			Gira(-90);
			Indietro();
			setTuttiPwm(20);
			delay(tempo);
			StopMotori();
			while((statoGenRil==1)||(statoRilascio==0)){
				_delay_ms(5);
			}
			Avanti();
			setTuttiPwm(20);
			delay(tempo);
			Gira(90);
			StopMotori();
		}
	}
	return valore;
}

uint8_t rilevaVittimaRampa(){
	uint8_t valore =0;
		tSx=TemperaturaSX();
		tDx=TemperaturaDX();
		if ((tSx>=temperaturaMinVittime)||(tDx>=temperaturaMinVittime)){
			Serial_Send_String("vittRamp\n");
			valore=1;
			StopMotori();
			rilasciokit(1);
			_delay_ms(200);
			setTuttiPwm(-gradiGiroscopio(asseY)*0.6);
			Indietro();
			while((statoGenRil==1)||(statoRilascio==0)){
				_delay_ms(5);
			}
		}
	return valore;
}

uint8_t verifica_salita_discesa(){
	uint8_t valore=0, c=0;
	if(inclinazione()==salita){
		Nsalite++;
		if(Nsalite>10){
Nsalite=0;
			Serial_Send_String("Salita\n");
			ColoreLED(PINKColor);
			Indietro();
			setTuttiPwm(pwmIndietroPrimaSalita);
			_delay_ms(tempoIndietroPrimaSalita);
			Gira(180);
			parallelototale();
			float AvDx=DistanzaIR(DestraAvanti);
			float AVSx=DistanzaIR(SinistraAvanti);
			Indietro();
 			if((AVSx<18)&&(AvDx>=18)){
 				setSingoliPwm(pwmAvantiSalita,pwmAvantiSalita-pwmVarSalita,pwmDietroSalita, pwmDietroSalita-pwmVarSalita);
 			
			 }
 			else if((AvDx<18)&&(AVSx>=18)){
 				setSingoliPwm(pwmAvantiSalita-pwmVarSalita,pwmAvantiSalita, pwmDietroSalita-pwmVarSalita ,pwmDietroSalita);
 			}
 			else{
				setSingoliPwm(pwmAvantiSalita,pwmAvantiSalita, pwmDietroSalita,pwmDietroSalita);
			}
			_delay_ms(1500);
		
			while(inclinazione()==discesa){
				if(c==0) if(rilevaVittimaRampa()==1){
					 c=1;
					 setSingoliPwm(pwmAvantiSalita,pwmAvantiSalita, pwmDietroSalita,pwmDietroSalita);
					 Indietro();
				}
				_delay_us(100);
			
			}
			setTuttiPwm(pwmIndietroDopoSalita);
			Indietro();
			delay(tempoIndietroDopoSalita);
		//	parallelototale();
			if((DistanzaIR(SinistraAvanti)>20)||(DistanzaIR( SinistraDietro)>20)){
			
				Gira(-90);
				parallelototale();
				Avanti30cm(pwmAvanti);
			}
			else
				Gira(180);
			parallelototale();
			valore=1;
			ColoreLED(SPENTO);
		}
	}
	else if(inclinazione()==discesa){
		Serial_Send_String("Discesa\n");
		ColoreLED(RED);
		setTuttiPwm(16);
		Avanti();
		_delay_ms(150);
		//StopMotori();
		while(inclinazione()==discesa){
			if(c==0) if(rilevaVittimaRampa()==1){
				c=1;
				setSingoliPwm(pwmAvantiSalita,pwmAvantiSalita, pwmDietroSalita,pwmDietroSalita);
				Avanti();
			}
			_delay_us(100);
			
		}
		ColoreLED(SPENTO);
		valore=1;
	}

	return valore;
}

char ControllaFinecorsa(int vel){
	char val=0;
	if(countFinecorsa<=4){
			if(DistanzaAvanti()>=6.5){
			FC=finecorsaCentrale();
			FDx=finecorsaDX();
			FSx=finecorsaSX();
			if((FC==1)||((FDx==1)&&(FSx==1))){
				
				Serial_Send_String("FinecAv\n");
				ColoreLED(BLUE);
				setTuttiPwm(pwmFinecorsa);
				Indietro();
				_delay_ms(tempoFinecorsaCentrale);
				Gira(-ultimoGira);
				parallelototale();
				Avanti30cm(vel);
				val=1;
			}
			FDx=finecorsaDX();
			if(FDx==1){
				
				Serial_Send_String("FinecDx\n");
				ColoreLED(ORANGE);
					setTuttiPwm(pwmFinecorsa);
					Indietro();
					delay(tempoFinecorsa);
					Gira(-gradiFinecorsa);
					setTuttiPwm(pwmFinecorsa);
					Avanti();
					delay(tempoFinecorsa*2);
					Gira(gradiFinecorsa);
					parallelototale();
					setTuttiPwm(pwmFinecorsa);
					Indietro();
					_delay_ms(800);
					StopMotori();
	
					Avanti30cm(vel);
					val=1;
			}
			FSx=finecorsaSX();
			if(FSx==1){
				
				Serial_Send_String("FinecSx\n");
				ColoreLED(PINKColor);
					setTuttiPwm(pwmFinecorsa);
					Indietro();
					delay(tempoFinecorsa);
					Gira(gradiFinecorsa);
					setTuttiPwm(pwmFinecorsa);
					Avanti();
					delay(tempoFinecorsa*2);
					Gira(-(gradiFinecorsa));
					parallelototale();
					setTuttiPwm(pwmFinecorsa);
					Indietro();
					_delay_ms(800);
					StopMotori();
				
					Avanti30cm(vel);
					val=1;
			}
			SpegniLED();
		}
	}else
	{
		val=0;
		countFinecorsa=0;
	}
	if(val==1)countFinecorsa++;
	return val;
}



void Avanti30cm(int vel){
Nsalite=0;
	stopTimerEncoder();
	//parallelototale();
	setTuttiPwm(vel);
	Avanti();
	float distanzaInizialeOld= DistanzaAvanti(), distanzaCorrente = 0, differenzaMisure=0;
	_delay_ms(5);
	float distanzaIniziale = DistanzaAvanti();
	differenzaMisure=distanzaIniziale-distanzaInizialeOld;
	if (ABS(differenzaMisure)>=10)
	{
		Avanti30cm(vel);
	}
	int numeroCasella=distanzaIniziale/32;
	
	if ((distanzaIniziale==1111)||(numeroCasella>1))
	//if(1==1)
	{
		NAvEnc++;
		Serial_Send_String("30Encoder\n");
		setTimerEncoder();
		
		counterInt2=0;
		counterInt3=0;
		//con encoder
		float mc=0, oldmc=0;
		long csb=0, cn=0;
		int pwm, x=0;
		while(!(((counterInt3>=1520)&&(counterInt2>=1520))||(x>3))){

			Serial_Send_String("Int3:");Serial_Send_Int(counterInt3);Serial_Send_String("Int2:");Serial_Send_Int(counterInt2);SerialN();			
cn++;
			mc=(counterInt2+counterInt3)/2;
			if(ABS(mc-oldmc)<6) csb++;
			oldmc=mc;
			if((csb>35) || (cn>450) || (NAvEnc>11)){
				if (NAvEnc>11)
					NAvEnc=5;
				//Serial_Send_String("Boost3AvEnco NAvEnc:");Serial_Send_Int(NAvEnc);Serial_Send_String("cn:");Serial_Send_Int(cn);Serial_Send_String("csb:");Serial_Send_Int(csb);SerialN();
				Avanti();
				setTuttiPwm(pwmBoost3);
				Indietro();
				delay(tempoInBoost3);
				setTuttiPwm(pwmBoost3);
				Avanti();
				delay(tempoAvBoost3);
				csb=-30;
				cn=0;
				x++;
				pwm=pwmBoost3;
			}
			else if((csb>20) || (cn>200) /*200*/){
				Serial_Send_String("Boost2AvEnco\n");
				pwm=pwmBoost2;
			}
			else if((csb>10) || (cn>100)/*150*/){
				Serial_Send_String("Boost1AvEnco\n");
				
				pwm=pwmBoost1;
			}else
			pwm=vel;
//Serial_Send_String("cbs:"); Serial_Send_Int(csb);Serial_Send_String("  cn:");Serial_Send_Int(cn);SerialN();
// 			Serial_Send_Int(csb);

			//rilevaVittima();
			//ColoreLED(ORANGE);
			Avanti();
			rilevaVittima();
			setTuttiPwm(pwm);
			if(ControllaFinecorsa(vel)==1) break;
			if(ControllaCasellaNera(1,(counterInt2+counterInt3)/2,vel)==1) break;
			if(verifica_salita_discesa()==1) break;
			//Serial_Send_Int(counterInt3);SerialN();
			//aggiungere nero, finecorsa, salita
		}
		//Serial_Send_String("uscito encoder:");Serial_Send_Int(counterInt2);SerialN();
		StopMotori();
		
		
		//Serial_Send_String("1300 \n");
		//ColoreLED(WHITE);
		//unsigned long prevMillis = millis();
		//while(!((millis()-prevMillis)>=tempoAvanti)){		
//// 			rilevaVittima();
			//Avanti();
			//setTuttiPwm(vel); 			
////			if0(0verifica_salita_discesa()==1) break;
 			//if(ControllaFinecorsa(vel)==1) break;
 			//if(ControllaCasellaNera(1,millis()-prevMillis,vel+5)==1) break;
		//
		//}
		ColoreLED(SPENTO);
	}
	else{
		NAvEnc=0;
		ColoreLED(BLUE);
		Serial_Send_String("av 30	\n");
		long contatore=0;

		distanzaCorrente=distanzaIniziale;
float oldval=distanzaCorrente;
		int resto=(int)distanzaCorrente%32, casellainiziale=distanzaCorrente/32, casella=0, errore1111=0;
		if((casellainiziale==0) || (resto>=19)){
			casellainiziale++;
		}
			
		float pwm=vel;
		int contVR=0;
		Serial_Send_String("30Sens\n");
		do
		{
			ColoreLED(RED);	
 			if(ControllaFinecorsa(vel)==1) break;
			 countFinecorsa=0;
 			rilevaVittima();
 			if(verifica_salita_discesa()==1) break;
 			if(ControllaCasellaNera(2,distanzaIniziale,vel)==1) break;
 			contatore++;
 			if(contatore>400/*300*/){
				 Serial_Send_String("Boost3Avsens\n");
 				Avanti();
 				setTuttiPwm(pwmBoost3);
 				Indietro();
 				delay(tempoInBoost3);
 				setTuttiPwm(pwmBoost3);
 				Avanti();
 				delay(tempoAvBoost3);
 				contatore=150;
 				pwm=pwmBoost3;
 			}
 			else if(contatore>200/*200*/){
				 Serial_Send_String("Boost2Avsens\n");
 				pwm=pwmBoost2;
 			}
 			else if(contatore>150/*150*/){
				 Serial_Send_String("Boost1Avsens\n");
 				pwm=pwmBoost1;
 			}else 
				pwm=vel;
			Avanti();
			setTuttiPwm(pwm);
			distanzaCorrente=DistanzaAvanti();
  			//Serial_Send_String("\n");
			  
 		//	Serial_Send_Float(distanzaCorrente);
	//		 Serial_Send_String("\n");
//   			Serial_Send_String("	distIniz:");Serial_Send_Float(distanzaIniziale);
//   			Serial_Send_String("   resto:");Serial_Send_Int(resto);
//   			Serial_Send_String("   casella:");Serial_Send_Int(casella);
//   			
//   			Serial_Send_String("   casellaIniz:");Serial_Send_Int(casellainiziale);
//  			Serial_Send_String("   contatore:");Serial_Send_Int(contatore);
			//_delay_ms(1);

			if(distanzaCorrente==1111){
				errore1111++;
				if(errore1111>20){
					Serial_Send_String("Uscito30sensxerrore1111\n");
					break;
				}
			}
			else errore1111=0;
			if(ABS(distanzaCorrente-oldval)>20){
		//		contVR++;
	//			if(contVR>20) break;
//				else
 continue;
 			}
//else{
// 				 contVR=0;
			oldval=distanzaCorrente;
// 			}
			casella=distanzaCorrente/32;
			resto=(int)distanzaCorrente%32;
		} while(!((((resto<=8)&&(distanzaCorrente<=20))||((resto<=7)&&(distanzaCorrente>=25)))&&((casellainiziale-1)>=casella)&&(contatore>10)&&(distanzaCorrente!=1111)));
		//Serial_Send_String("fine 30Sen:");Serial_Send_Float(distanzaCorrente);Serial_Send_String(" I:");Serial_Send_Float(distanzaIniziale);SerialN();
}
	//Serial_Send_String("fine 30\n");
	Indietro();
	setTuttiPwm(70);
	_delay_ms(20);
	StopMotori();
	statokit=1;
	parallelototale();
	
	
}

/*-------------------------------------*/