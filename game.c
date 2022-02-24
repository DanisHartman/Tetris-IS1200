

int a[3][3] = {  
   {1, 0, 0},   
   {1, 1, 0},  
   {0, 1, 0}  
};

int playerPosX = 10;
int playerPosY = 35;



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

void update() {
	playerPosY += -3;
	drawMatrix( playerPosX, playerPosY);
	draw_borders();
	render();
}
