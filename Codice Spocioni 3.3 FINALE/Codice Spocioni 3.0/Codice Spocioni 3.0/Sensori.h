

//SENSORI

void delay(int tempo);
//IR0

#define ArgentoMAX 10
#define BiancoMAX 680 //600   //NeroMIN
#define NeroMAX    1500 //900
#define Bianco 0
#define Nero 1
#define Argento 2	

#define piano 0
#define salita 1
#define discesa 2
#define gradiSalita 15
#define gradiDiscea -15
#define gradiMinDiscesa -10
#define gradiMinSalita 10

void verificaBatteria();
float valoreMinimoBatteria;
uint8_t  inclinazione();

float DistanzaIR(uint8_t POS);

float Fotoresistenza();

uint8_t ColoreCasella();

uint8_t pulsante();
uint8_t finecorsaDX();
uint8_t finecorsaSX();
uint8_t finecorsaCentrale();
void initPulsanti();
void stampaPulsFinecorsa();

float DistanzaAvanti();

#define DavantiLungo 0  //A0
#define  DavantiCorto 1 //A1
#define DestraAvanti 2 //A2
#define DestraDietro 3 //A3
#define SinistraAvanti 4  //A4
#define  SinistraDietro 5  //A5

float TemperaturaSX();
float TemperaturaDX();
float voltmetro();

