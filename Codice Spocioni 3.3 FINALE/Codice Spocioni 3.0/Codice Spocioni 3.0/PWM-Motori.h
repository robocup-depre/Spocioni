#include <avr/io.h>

//PWM
void initPWM(void);
void startPWM(void);
void StopMotori();
void setSingoliPwm(float AvSX, float AvDX, float DiSX,float DiDX);
void setTuttiPwm(float PWM);
void setCoppiaPwm(float DX, float SX);
void Avanti();
void Indietro();
void RuotaSX();
void RuotaDX();
	

#define RegPwmMotAvDx OCR4B	//22,23  
#define RegPwmMotAvSx OCR4A	//24,25
#define RegPwmMotDiDx OCR4C	//26,27
#define RegPwmMotDiSx OCR3A	//28,29
#define PortaAvanti 0b01011001
#define PortaRuotaDestra 0b10010101
		//			  76543210

#define topPwmMotAvDx 1023/100
#define topPwmMotAvSx 1023/100
#define topPwmMotDiDx 1023/100
#define topPwmMotDiSx 1023/100


//encoder
void setTimerEncoder();
void stopTimerEncoder();

int countFinecorsa;