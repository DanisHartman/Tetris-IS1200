#include <stdio.h>

int a[3][3] = {  
   {1, 0, 0},   
   {1, 0, 0},  
   {1, 0, 0}  
};

int arena[32][10] = {2};

int main()
{
	printf("%d\n", arena[50][14] != 2);
	return 0;
}


/*void collision() {
	for (var y = 0; y < player.pMatrix.length; y++) {
		for (var x = 0; x < player.pMatrix[y].length; x++) {
			if(m[y][x] !== 0 &&
				(
				arena[y + o.y][x + o.x]) !== 0) {
				return true;
			}	
		}
	}
	return false;
}*/

