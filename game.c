#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>

int globalcount = 0;


int a[3][3] = {  
   {1, 0, 0},   
   {1, 0, 0},  
   {1, 0, 0}  
};

//int **arena;
int arena[10][32] = {0}; // X-Y fel omvänt!!

const int ARENAYOFFSET = 31;
const int ARENAHEIGHT = 32;
const int ARENAWIDTH = 10;

int playerPosX = 13;
int playerPosY = 34;

void timer_init() {
	T2CONSET = 0x70; // 1:256 prescale, internal clock
  	PR2 = ((80000000/256)/100);
  	TMR2 = 0; // reset counter
  	T2CONSET = 0x8000;  // timer on
}

void drawMatrix( int playerPosX,int playerPosY) {
	int x = 0;
	int y = 0;
	for ( x = 0; x < 3; x++)
	{
		for ( y = 0; y < 3; y++)
		{
			if(a[x][y] == 1)
			{
				fillSquare((y*3) + playerPosX, (3*x) + playerPosY-3);
			}
		}
	}
}


int dropInterval = 1000;
int dropInterval2 = 200;
int dropCounter = 0;
int dropCounter2 = 0;
int lastTime = 0;
float time = 0;

bool collision() {
	int x = 0;
	int y = 0;

	for ( y = 0; y < 3; y++) {
		for ( x = 0; x < 3; x++) {

				if(a[y][x] != 0 &&
					(arena[y + playerPosY-ARENAYOFFSET][x + playerPosX]) != 0) { //Sätt till två?
					return true;
				}	
				
				if((playerPosY > ((ARENAHEIGHT*3)+ARENAYOFFSET)-6))
				{
					return true;
				}
			}	
		}
	return false;
}

void drawArena() {
	int y = 0;
	int x = 0;

	for (x = 0; x < ARENAWIDTH; x++) {
		for (y = 0; y < ARENAHEIGHT; y++) {
			if(arena[x][y] != 0)
			{
				fillSquare((x * 3)+1, (ARENAYOFFSET + (y * 3)));
			}
		}		
	}
}

void addToArena() {
	int y = 0;
	int x = 0;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			if(a[y][x] != 0) {
				arena[((playerPosX)/3)+x][((playerPosY-ARENAYOFFSET-3)/3)+y] = 1; 
			}
		}
	}
}

void pieceReset() {
	addToArena();
	playerPosY = 34;
	playerPosX = 13;
}


void playerDrop(void) {
	playerPosY += 3;
	dropCounter = 0;
}

void playerMove(int value) {
	playerPosX += (value*3);

	if(collision()){
		playerPosX += -value;
	}

}

void input(int btns) {
	if((btns & 0x1) == 1) {
		playerPosY += 3;
	}

	if(((btns & 0x8) == 8) ) {
		addToArena();
	}

	if(dropCounter2 > dropInterval2){
		if((btns & 0x2) == 2) {
			playerMove(-1);
			dropCounter2 = 0;
		}

		if((btns & 0x4) == 4) {
			playerMove(1);
			dropCounter2 = 0;
		}
		
	}
	
	
	
}


void start_game() {
	//generateArenaMatrix();
}

void update() {
	int btns = getbtns();
	time += 0.01;


	if (IFS(0) & 0x100) {
		input(btns);

		const int deltaTime = (int)time - lastTime;
		lastTime = time;
 		dropCounter += deltaTime;
 		dropCounter2 += deltaTime;

 		

 		if(dropCounter > dropInterval){
			playerDrop();

			if(collision()){
	 			playerPosY += -3;
	 			pieceReset();
	 		}
 		}


		IFS(0) = 0;
		
		draw_borders();

		drawArena();
		drawMatrix( playerPosX, playerPosY);

		render();
	}
}


