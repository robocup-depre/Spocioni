
#include <avr/io.h>
		//PARALLELO
#define pwmParallelo 35//32
#define pwmBoostParallelo 80//100
#define tolleranzaParallelo 0.4//.7
#define tolleranzaMassimaParallelo 12
#define distanzaMinimaParalleo 20
void parallelototale();

float pwmAvanti;

		//GIRA
#define pwmBoostgy1 80
#define pwmBoostgy2 100
#define tempoAvBoostgy2 150 //tempo Avanti
#define tempoRInBoostgy2 220 //tempo ruota Indietro
#define pwmGiraInizio 35//34 //32 //33
#define pwmGiraFine 15//14//19 //23
#define gradiretta 30
void Gira(float);
int ultimoGira;

		//VITTIME
#define tempMovRilKit 120 //tempo indieto nel movimento per rilasciare il kit
uint8_t statokit;
#define temperaturaMinVittime 36 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
uint8_t rilevaVittima();
uint8_t rilevaVittimaRampa();

		//AVANTI 30cm
int posizioneIncerta;//1 se ha fatto il movimento per liberare il sensore
int presenzaMuroSinistra;//1 se è presente muro a sx
void Avanti30cm(int vel);
			//con millis
//#define tempoAvanti 1050  //16
#define tempoAvanti 1500  //10
			//con sensori
#define pwmBoost1 60
#define pwmBoost2 100
#define tempoBoost 10
#define pwmBoost3 95
#define tempoAvBoost3 900 //tempo Avanti
#define tempoInBoost3 750 //tempo Indietro

	
		//FINECORSA
#define gradiFinecorsa 28
#define tempoFinecorsa 400
#define tempoFinecorsaSoloIndietro 150
#define tempoFinecorsaCentrale 600
#define pwmFinecorsa 20
char ControllaFinecorsa(int vel);

		//SALITA-DISCESA
#define pwmAvantiSalita 73
#define pwmDietroSalita 90
#define pwmVarSalita 30
#define pwmDiscesa 7
#define tempoIndietroPrimaSalita 800
#define pwmIndietroPrimaSalita 25
#define tempoIndietroDopoSalita 300
#define pwmIndietroDopoSalita 25

int Nsalite;

//encoder
unsigned int counterInt2;
unsigned int counterInt3;

int NAvEnc;