#include <stdio.h>
#include "devkit/SMSlib.h"
#include "graphics.h"  // compiled with folder2c

#define SPRITES 256
#define BACKGROUND 0
#define JUMP_FRAME_NUMBERS 20

unsigned char playerPosition[2];
int jumpFrame;
int frameNr;

void input () {
	int joy;
	joy = SMS_getKeysStatus();

	// if (joy & PORT_A_KEY_UP) {
  //   playerPosition[1]--;
	// }
	// if (joy & PORT_A_KEY_DOWN) {
  //   playerPosition[1]++;
	// }
	if (joy & PORT_A_KEY_LEFT) {
    playerPosition[0]--;
	}
	if (joy & PORT_A_KEY_RIGHT) {
    playerPosition[0]++;
	}
  if (joy & PORT_A_KEY_1) {
    if (frameNr - jumpFrame > JUMP_FRAME_NUMBERS) {
      jumpFrame = frameNr;
    }
  }
}


int collideWithPlatform () {
  if (playerPosition[1] < 100) {
    return 0;
  } else {
    return 1;
  }
}

void gravity () {
  int jumping = frameNr - jumpFrame < JUMP_FRAME_NUMBERS / 2;
  if (jumping) {
    playerPosition[1]--;
  }

  if (!collideWithPlatform() && !jumping) {
    playerPosition[1]++;
  }
}

void load_assets(void) {
  SMS_loadPSGaidencompressedTiles(titletiles_psgcompr, BACKGROUND);
  SMS_loadSTMcompressedTileMap(0, 0, titletilemap_stmcompr);
  // SMS_loadTileMap(0, 0, titletilemap_bin, 32 * 25 * 2);
  SMS_loadBGPalette(titlepal_bin);

  SMS_loadPSGaidencompressedTiles(blatiles_psgcompr, SPRITES);
  SMS_loadSpritePalette(blapal_bin);
}


void main() {
  int turn = 0;
	frameNr = 0;
  jumpFrame = 0;
  playerPosition[0]=10;
  playerPosition[1]=100;

  SMS_setSpriteMode (SPRITEMODE_TALL);
  SMS_VDPturnOnFeature(VDPFEATURE_USETALLSPRITES);
  SMS_VDPturnOffFeature(VDPFEATURE_HIDEFIRSTCOL);

  load_assets();
  SMS_displayOn();

	for (;;) {
		frameNr = frameNr + 1;
		input();
    gravity();
    SMS_initSprites();
    SMS_addSprite(playerPosition[0], playerPosition[1], SPRITES);
    SMS_finalizeSprites();
    SMS_waitForVBlank();
    SMS_copySpritestoSAT();
	}
}
