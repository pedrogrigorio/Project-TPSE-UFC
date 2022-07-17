#include "bbb_setup.h"

/*

typedef enum _LED{
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
	LED6,
	LED7,
	LED8,
	LED9	
}LED;
*/

int table[3][3];
int path[3][3];		
int winMove[3][3];
int mapeamento[3][3] = {49, 50, 51, 52, 53, 54,55, 56, 57};
unsigned int player1[9] = {1, 2	, 3, 4, 5, 6, 7, 8, 9};
unsigned int player2[9] = {11,22,33, 44, 55, 66, 77, 88, 99};
int op = 49;
int mark = 1;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  memsetMatrix
 *  Description:  Função zera uma matriz.
 * =====================================================================================
 */
void memsetMatrix(int n, int x, int matrix[n][n]){
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			matrix[i][j] = x;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mapPlay
 *  Description:  Preenche, na matriz table, a posição equivalente ao led selecionado.
 * =====================================================================================
 */
void mapPlay(int player){
	for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            if(op == mapeamento[i][j]){
                table[i][j] = player;
			}
		}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  checkStatus
 *  Description:  Verifica se o player passado ganhou, se sim, retorna o número do
 * 				  jogador, senao retorna 0.
 * =====================================================================================
 */
int checkStatus(int player){
	for(int i = 0; i < 3; i++)
		if(table[i][0] == player && table[i][1] == player && table[i][2] == player){
		winMove[i][0] = player;
		winMove[i][1] = player;
		winMove[i][2] = player;
		return player;
		}
	for(int j = 0; j < 3; j++)
		if(table[0][j] == player && table[1][j] == player && table[2][j] == player){
		winMove[0][j] = player;
		winMove[1][j] = player;
		winMove[2][j] = player;
		return player;
	}
	if(table[0][0] == player && table[1][1] == player && table[2][2] == player){
		winMove[0][0] = player;
		winMove[1][1] = player;
		winMove[2][2] = player;
		return player;
	}
	else if(table[0][2] == player && table[1][1] == player && table[2][0] == player){
		winMove[0][2] = player;
		winMove[1][1] = player;
		winMove[2][0] = player;
		return player;
	}
	else
		return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  checkTie
 *  Description:  Verifica se houve empate. Se sim retorna 1, senão retorna 0.
 * =====================================================================================
 */
int checkTie(void){
	for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(table[i][j] == 0){
                return 0;
			}
	return 1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  winTreatment
 *  Description:  Tratamento para vitória.
 * =====================================================================================
 */
void winTreatment(int player){

	for(int i = 0; i < 9; i++){
		ledOff(player1[i]);
		ledOff(player2[i]);
	}
	while(reset){
		switch (player){
			case 1:
				for(int i = 0; i < 3; i++)
					for(int j = 0; j < 3; j++)
						if(winMove[i][j] == player){
							ledOn(player1[i*3+j]);
							delay(250);
						}
				for(int i = 2; i >= 0; i--)
					for(int j = 2; j >= 0; j--)
						if(winMove[i][j] == player){
							ledOff(player1[i*3+j]);
							delay(250);
						}
				break;
			case 2:
				for(int i = 0; i < 3; i++)
					for(int j = 0; j < 3; j++)
						if(winMove[i][j] == player){
							ledOn(player2[i*3+j]);
							delay(250);
						}
				for(int i = 2; i >= 0; i--)
					for(int j = 2; j >= 0; j--)
						if(winMove[i][j] == player){
							ledOff(player2[i*3+j]);
							delay(250);
						}
				break;
			default: 
				break;
		}
	}

	for(int i = 0; i < 9; i++){
		ledOff(player1[i]);
		ledOff(player2[i]);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tieTreatment
 *  Description:  Tratamento para empate.
 * =====================================================================================
 */
void tieTreatment(){

	for(int i = 0; i < 9; i++){
		ledOff(player1[i]);
		ledOff(player2[i]);
	}
	delay(500);

	while(reset){
		for(int i = 0; i < 9; i++){
			ledOn(player1[i]);
			ledOn(player2[8-i]);
			delay(250);
		}
		for(int i = 8; i >= 0; i--){
			ledOff(player1[i]);
			ledOff(player2[8-i]);
			delay(250);
		}
	}

	for(int i = 0; i < 9; i++){
		ledOff(player1[i]);
		ledOff(player2[i]);
	}

}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  nextStep
 *  Description:  Seleciona a próxima posição livre de LEDs para piscar.
 * =====================================================================================
 */
int nextStep(){
	for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            if((table[i][j] == 0) && (mapeamento[i][j] != op) && (path[i][j] == 0)){ 
                op = mapeamento[i][j];
                return 0;
			}
			path[i][j] = 1;
		}
	memsetMatrix(3, 0, path);
	return 1;
}
// 012345678
//
//
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: walk
 *  Description: Tratamento para a função de andar pelas posições de LEDs
 * =====================================================================================
 */
void walk(void){

    /* Clear the status of the interrupt flags */
    HWREG(GPIO1_IRQSTATUS_1) = (1<<28); // referente ao botão 02
	int stop = 1;
	while(stop)
		stop = nextStep();
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  player1Move
 *  Description:  Tratamento para a jogada do player 1.
 * =====================================================================================
 */
void player1Move(void){

    /* Clear the status of the interrupt flags */
	HWREG(GPIO3_IRQSTATUS_0) = (1<<19); // Limpa interrupção
	mark = 1;
	// Mantem aceso o LED do player 1 e apaga o outro.
	switch(op){
		case 49://LED1:
			ledOn(1);
			ledOff(11);
			break;
		case 50://LED2::
			ledOn(2);
			ledOff(22);
			break;
		case 51://LED3::
			ledOn(3);
			ledOff(33);
			break;
		case 52://LED4::
			ledOn(4);
			ledOff(44);
			break;
		case 53://LED5::
			ledOn(5);
			ledOff(55);
			break;
		case 54://LED6::
			ledOn(6);
			ledOff(66);
			break;
		case 55://LED7::
			ledOn(7);
			ledOff(77);
			break;
		case 56://LED8::
			ledOn(8);
			ledOff(88);
			break;
		case 57://LED9::
			ledOn(9);
			ledOff(99);
			break;
		default:
			break;
	}

	mapPlay(1);								//mapeia a jogada na matriz
	int winner = checkStatus(1);			//verifica status do jogo (se o player 1 ganhou, se deu velha ou se o jogo não terminou)

	//tratamentos para caso de vitória ou velha.
	if(winner == 1){
		op = 100;
		return;
	}
	else if(winner == 0 && checkTie()){
		op = 102;
		return;
	}

	memsetMatrix(3, 0, path);
	nextStep();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  player2Move
 *  Description:  Tratamento para a jogada do player 2.
 * =====================================================================================
 */
void player2Move(void){

    /* Clear the status of the interrupt flags */
	HWREG(GPIO3_IRQSTATUS_1) = (1<<20); // Limpa interrupção
	mark = 1;
	switch(op){
		case 49://LED1::
			ledOn(11);
			ledOff(1);
			break;
		case 50://LED2::
			ledOn(22);
			ledOff(2);
			break;
		case 51://LED3::
			ledOn(33);
			ledOff(3);
			break;
		case 52://LED4::
			ledOn(44);
			ledOff(4);
			break;
		case 53://LED5::
			ledOn(55);
			ledOff(5);
			break;
		case 54://LED6::
			ledOn(66);
			ledOff(6);
			break;
		case 55://LED7::
			ledOn(77);
			ledOff(7);
			break;
		case 56://LED8::
			ledOn(88);
			ledOff(8);
			break;
		case 57://LED9::
			ledOn(99);
			ledOff(9);
			break;
		default:
			break;
	}

	mapPlay(2);
	int winner = checkStatus(2);			//verifica status do jogo (se o player 1 ganhou, se deu velha ou se o jogo não terminou)

	//tratamentos para caso de vitória ou velha.
	if(winner == 2){
		op = 101;
		return;
	}
	else if(winner == 0 && checkTie()){
		op = 102;
		return;
	}

	memsetMatrix(3, 0, path);
	nextStep();
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  resetTreatment
 *  Description:  Reseta o jogo após o término.
 * =====================================================================================
 */
void resetTreatment(void){
	/* Clear the status of the interrupt flags */
	HWREG(GPIO1_IRQSTATUS_0) = (1<<16); // Limpa a interrupção
	reset = false;

	op = 49;
	memsetMatrix(3, 0, table);
	memsetMatrix(3, 0, path);
	memsetMatrix(3, 0, winMove);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ISR_Handler
 *  Description:  Rotina de tratamento de interrupções.
 * =====================================================================================
 */
void ISR_Handler(void){
	/* Verifica se é interrupção do DMTIMER7 */
	unsigned int irq_number = HWREG(INTC_SIR_IRQ) & 0x7f;
	if(irq_number == 95)
		timerIrqHandler();
	if(irq_number == 62) 	//GPIO3_A
		player1Move();
	if(irq_number == 63) 	//GPIO3_B
		player2Move();
    if(irq_number == 98){ 	//GPIO1_A
		resetTreatment();
	}
	if(irq_number == 99){	//GPIO1_B
		walk();
	}
	/* Reconhece a IRQ */
	HWREG(INTC_CONTROL) = 0x1;
} 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(void){
	
	/* Hardware setup */
	gpioSetup();
	ledConfig();
	butConfig();
	timerSetup();
	disableWdt();

	memsetMatrix(3,0, path);
	memsetMatrix(3,0, winMove);
	memsetMatrix(3,0, table);

	for(int i = 0; i < 9; i++){
		ledOff(player1[i]);
		ledOff(player2[i]);
	}
	delay(1000);

	//Verificar leds
	/*
	while(1){
		for(int i = 0; i < 9; i++){
			ledOn(player1[i]);
			delay(1000);
			putString("delay",5);
			ledOff(player1[i]);
			ledOn(player2[i]);
			delay(1000);
			ledOff(player2[i]);
		}
	}
	*/ 

	while(1){
		switch(op){
			case 49://LED1::
				if(mark == 0)
					ledOn(1);
				if(mark == 0)
					ledOn(11);
				delay(500);
				if(mark == 0)
					ledOff(1);
				if(mark == 0)
					ledOff(11);
				delay(500);
				if(mark == 1)
					mark = 0;
				break;
			case 50://LED1::
				if(mark == 0)
					ledOn(2);
				if(mark == 0)
					ledOn(22);
				delay(500);
				if(mark == 0)
					ledOff(2);
				if(mark == 0)
					ledOff(22);
				delay(500);
				if(mark == 1);
					mark = 0;
				break;
			case 51://LED1::
				if(mark == 0)
					ledOn(3);
				if(mark == 0)
					ledOn(33);
				delay(500);
				if(mark == 0)
					ledOff(3);
				if(mark == 0)
					ledOff(33);
				delay(500);
				if(mark == 1)
					mark = 1;
				break;
			case 52://LED1::
				if(mark == 0)
					ledOn(4);
				if(mark == 0)
					ledOn(44);
				delay(500);
				if(mark == 0)
					ledOff(4);
				if(mark == 0)
					ledOff(44);
				delay(500);
				if(mark == 1);
					mark = 0;
				break;
			case 53://LED1::
				if(mark == 0)
					ledOn(5);
				if(mark == 0)	
					ledOn(55);
				delay(500);
				if(mark == 0)
					ledOff(5);
				if(mark == 0)	
					ledOff(55);
				delay(500);
				if(mark == 1)	
					mark = 0;
				break;
			case 54://LED1::
				if(mark == 0)
					ledOn(6);
				if(mark == 0)
					ledOn(66);
				delay(500);
				if(mark == 0)
					ledOff(6);
				if(mark == 0)
					ledOff(66);
				delay(500);
				if(mark == 1)
					mark = 0;
				break;
			case 55://LED1::
				if(mark == 0)
					ledOn(7);
				if(mark == 0)
					ledOn(77);
				delay(500);
				if(mark == 0)
					ledOff(7);
				if(mark == 0)
					ledOff(77);
				delay(500);
				if(mark == 1)
					mark = 0;
				break;
			case 56://LED1::
				if(mark == 0)
					ledOn(8);
				if(mark == 0)	
					ledOn(88);
				delay(500);
				if(mark == 0)
					ledOff(8);
				if(mark == 0)
					ledOff(88);
				delay(500);
				if(mark == 1)
					mark = 0;
				break;
			case 57://LED1::
				if(mark == 0)
					ledOn(9);
				if(mark == 0)
					ledOn(99);
				delay(500);
				if(mark == 0)
					ledOff(9);
				if(mark == 0)
					ledOff(99);
				delay(500);
				if(mark == 1)
					mark = 1;
				break;
			case 100:
				winTreatment(1);
				reset = true;
				break;
			case 101:
				winTreatment(2);
				reset = true;
				break;
			case 102:
				tieTreatment();
				reset = true;
			default:
				break;
		}
	}

	return 0;
}