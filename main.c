#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>

const char sprite_character[][] = {
{
	0,170,160,2,170,160,10,170,168,10,170,168,42,170,168,41,
	85,104,37,85,80,38,169,168,37,186,184,37,169,168,37,85,
	80,37,85,80,5,90,80,1,85,80,0,85,64,2,170,160,
	2,170,160,2,170,160,2,170,160,0,170,128,0,162,128,128
},
{
	0,170,162,2,170,170,10,170,170,10,170,170,42,170,168,41,
	85,80,38,169,168,37,186,184,37,169,168,37,85,80,37,85,
	80,37,85,80,5,90,80,1,85,80,0,85,64,0,170,160,
	2,170,160,2,170,160,2,170,128,0,170,128,0,128,128,128
},

{
	0,170,160,2,170,168,10,170,170,10,170,170,42,170,170,41,
	85,90,37,85,90,38,170,170,37,186,184,37,169,168,37,85,
	80,37,85,80,5,90,80,1,85,80,0,85,64,2,170,160,
	2,170,168,2,170,168,0,170,128,0,170,128,0,40,0,128
}
};

int v = 0xD000;	// START OF DISPLAY CHIP

// Need wait function to slow down x loop
void rasterWait(void) {
	unsigned char raster;
	do {
		raster = PEEK(v + 18);
	} while (raster < 250 || raster > 252);
}

int main (void)
{
	unsigned char n;
	unsigned char x = 0;
	unsigned char frame = 0;
	unsigned char time = 0;
	unsigned char sprite_x = 30;
	printf ("%c", 147);

	POKE(v + 21, 255); // enable all sprites
	POKE(2040, 13); // pointer sprite 0, blk 13th (13*64) 
	POKE(2041, 14); // pointer sprite 1 
	POKE(53276, 0b00000010); // enable multicolor on sprite 1

	/*for (n = 0 ; n < sizeof(sprite) ; n++) {
		POKE(13*64 + n, sprite[n]);
	}*/

	// stand pose
	for (n = 0 ; n < sizeof(sprite_character[0]); n++) {
		POKE(14*64 + n, sprite_character[0][n]);
	}


	/*for (n = 0 ; n < 64; n++) {
		POKE(13*64 + n, 255);
	}*/

	// 0b00 is transparency
	POKE(53285, 10); // shared multicolor 0b01
	POKE(53286, 1); // shared multicolor 0b11

	POKE(53287, 1); // uni-color sprite 0 0b10
	POKE(53288, 0); // uni-color sprite 1 0b10

	//POKE(v+29, 0b00000001); // x expand sprite 0
	//POKE(v+23, 0b00000001); // y expand sprite 0

	POKE(v + 0, (320/2)+(24/2));
	POKE(v + 2, sprite_x);
	POKE(v + 3, 220);

	while(1)
	{
		if (x >= 130)
		{
			POKE(v+29, 0b00000001);
			POKE(v+23, 0b00000001);
			POKE(v + 0, (320/2));
		}		
		else 
		{
			POKE(v + 1, x);
			//x += 1;
		}

		if(sprite_x >= 255)
			POKE(53264, 0b00000010);
		else {
			if(PEEK(53264) == 0b00000010 && sprite_x > 100){
				POKE(53264, 0b00000000);
				sprite_x = 0;
			}
		}

		// if no key pressed show stand pose	
		if(PEEK(0xDC01) == 255) {
			for (n = 0 ; n < sizeof(sprite_character[0]); n++) {
				POKE(14*64 + n, sprite_character[0][n]);
			}
		}
		
		// right
		if((PEEK(0xDC01) & 0b00001000) == 0) {
			POKE(v + 2, sprite_x += 1);
			if(time >= 20){
				for (n = 0 ; n < sizeof(sprite_character[frame]); n++) {
					POKE(14*64 + n, sprite_character[frame][n]);
				}
				frame += 1;
				if(frame > 2)
					frame = 1;
				time = 0;
			}
			time += 2;
		}

		// left
		if((PEEK(0xDC01) & 0b00000100) == 0) {
			POKE(v + 2, sprite_x -= 1);
		}

		rasterWait();
	} 

	return EXIT_SUCCESS;	
}
