#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <SDL_ttf.h>              //SDL ttf lib
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include <stdint.h>               //use uint32_t
#include "game.h"
#include "han2unicode.h"
#include "timer.h"



bool init()
{
	bool Sflag = true; //success flag

	//SDL �ʱ�ȭ
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL�� �ʱ�ȭ �� �� �����ϴ�. : %s", SDL_GetError());
		Sflag = false;
	}
	else
	{
		//������ ����
		gWindow = SDL_CreateWindow("BoA2 Production", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("�����츦 ������ �� �����ϴ�. : %s", SDL_GetError());
			Sflag = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); //������ ����
			if (gRenderer == NULL)
			{
				printf("������ ���� ����! : %s", SDL_GetError());
				Sflag = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //�������� ���ȭ������ �׸����� ����


				int imgFlags = IMG_INIT_PNG; //PNG �ε� �÷���
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL�̹��� ����� �ʱ�ȭ �� �� �����ϴ�! : %s", IMG_GetError());
					Sflag = false;
				}
				else if (TTF_Init() == -1)
				{
					printf("SDL_TTF ����� �ʱ�ȭ �� �� �����ϴ�! : %s", TTF_GetError());
					Sflag = false;
				}
				else
				{
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}


			}
		}
	}
	//�÷��̾�&Ÿ�̸� ���� �ʱ�ȭ

	gPlayer.Player_HEIGHT = 78;
	gPlayer.Player_WIDTH = 78;
	gPlayer.Player_VEL = 2;

	//�⺻ �÷��̾� ��ġ ����
	gPlayer.mBox.x = 0;
	gPlayer.mBox.y = 0;
	gPlayer.mBox.w = gPlayer.Player_WIDTH;
	gPlayer.mBox.h = gPlayer.Player_HEIGHT;

	gPlayer.mVelX = 0;
	gPlayer.mVelY = 0;


	timer.mStartTicks = 0;
	timer.mPauseTicks = 0;
	timer.mPaused = false;
	timer.mStarted = false;

	return Sflag;

}
SDL_Surface* loadSurface(char* path)
{
	//����� �̹��� �ε�
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL) {
		printf("�̹��� �ε� ����! : %s\n", SDL_GetError());
	}
	return loadedSurface;
}
SDL_Texture* loadTexture(char* path)
{
	SDL_Texture* newTexture = NULL;

	//SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("�̹��� �ε� ����! : %s\n", IMG_GetError()); //IMG��� ���� SDL_GetError ��� IMG_GetError���
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("�ؽ��� ���� ����! : %s\n", SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;

}
bool loadMedia(_LTile *tiles[])
{
	bool Sflag = true; //success flag

	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT], gRenderer, "images/player_front.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ��� �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_UP], gRenderer, "images/player_back.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_UP].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(up) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_DOWN], gRenderer, "images/player_front.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_DOWN].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(down) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_LEFT], gRenderer, "images/player_left.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_LEFT].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(left) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_RIGHT], gRenderer, "images/player_right.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_RIGHT].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(right) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_O], gRenderer, "images/duck_insane.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_O].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(insane) �ε� ����!\n");
		Sflag = false;
	}


	loadFromFile(&gDuckTexture, gRenderer, "images/duck_front.png");
	if (gDuckTexture.mTexture == NULL)
	{
		printf("���� �ε� ����!\n");
		Sflag = false;
	}

	loadFromFile(&gSightLimiter, gRenderer, "images/SightLimiter.png");
	if (gSightLimiter.mTexture == NULL)
	{
		printf("����Ʈ������ �ε� ����!\n");
		Sflag = false;
	}

	gLetterbox = loadTexture("images/Lbox.png");
	if (gLetterbox == NULL)
	{
		printf("�ؽ����̹���(gLetterbox) �ε� ����!\n");
		Sflag = false;
	}

	gMinimap = loadTexture("images/minimap.png");
	if (gMinimap == NULL)
	{
		printf("�ؽ����̹���(gMinimap) �ε� ����!\n");
		Sflag = false;
	}

	//���� ���� ��Ʈ �ҷ���
	gFont = TTF_OpenFont("korean.ttf", 28);
	if (gFont == NULL)
	{
		printf("��Ʈ �ε� ����! : %s", TTF_GetError());
		Sflag = false;
	}
	else
	{
		SDL_Color textColor = { 255,255,255 };

		if (!loadFromRenderedText(&gTextTexture[0], gRenderer, gFont, "������ ���� ��Ƴ�������!", textColor))
		{
			printf("�ؽ��� ���� ����!");
			Sflag = false;
		}
		if (!loadFromRenderedText(&gTimeText, gRenderer, gFont, "Time :", textColor))
		{
			printf("Ÿ�̸Ӹ� ������ �� �����ϴ�! \n");
		}

		textColor.g = 0;
		textColor.b = 0;
		if (!loadFromRenderedText(&gTextTexture[1], gRenderer, gFont, "�ð��� �� ���� �ʾ� ������ �̵��ӵ��� �����մϴ�!", textColor))
		{
			printf("�ؽ��� ���� ����!");
			Sflag = false;
		}

	}
	loadFromFile(&gTileTexture[0], gRenderer, "images/tile00.png");
	loadFromFile(&gTileTexture[1], gRenderer, "images/tile01.png");
	loadFromFile(&gTileTexture[2], gRenderer, "images/tile02.png");
	loadFromFile(&gTileTexture[3], gRenderer, "images/tile03.png");
	loadFromFile(&gTileTexture[4], gRenderer, "images/tile04.png");
	loadFromFile(&gTileTexture[5], gRenderer, "images/tile05.png");
	loadFromFile(&gTileTexture[6], gRenderer, "images/tile06.png");
	loadFromFile(&gTileTexture[7], gRenderer, "images/tile07.png");
	loadFromFile(&gTileTexture[8], gRenderer, "images/tile08.png");
	loadFromFile(&gTileTexture[9], gRenderer, "images/tile09.png");
	loadFromFile(&gTileTexture[10], gRenderer, "images/tile10.png");
	loadFromFile(&gTileTexture[11], gRenderer, "images/tile11.png");
	for (int i = 0; i < 12; i++) {
		if (gTileTexture[i].mTexture == NULL)
		{
			printf("Ÿ�� �ؽ���(%d) �ε� ����! \n", i);
			Sflag = false;
			break;
		}
	}
	if (!setTiles(tiles))
	{
		printf("Ÿ�ϼ� ���� ����!\n");
		Sflag = false;
	}
	//���� ���� ���ϴ� �̹����� �����Ϳ� ������ �������� ���� ��ġ���Ѽ� ������Ʈ 
	return Sflag;
}
void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyTexture(gMinimap);
	gMinimap = NULL;

	SDL_DestroyTexture(gLetterbox);
	gLetterbox = NULL;

	for (enum KeyPressSurfaces i = KEY_PRESS_SURFACE_DEFAULT; i < KEY_PRESS_SURFACE_TOTAL; i++)
		lfree(&gMainplayerTexture[i]);
	for (int i = 0; i < 12; i++)
		lfree(&gTileTexture[i]);
	lfree(&gFont);
	//���ǽ� �ʱ�ȭ
	lfree(&gCurrentSurface);


	TTF_CloseFont(gFont);
	gFont = NULL;
	//������ �ݱ�
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//sdl ���̺귯�� �ݱ�
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}
bool loadFromFile(_LTexture* LT, SDL_Renderer* Renderer, char* path)
{
	lfree(LT);

	SDL_Texture* newTexture = NULL;  //�ؽ��� ������

	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface == NULL)
	{
		printf("�̹��� �ε� ����! : %s", IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("�ؽ��� ���� ����! : %s", SDL_GetError());
		}
		else
		{
			LT->mWidth = loadedSurface->w;
			LT->mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}

	LT->mTexture = newTexture;
	return LT->mTexture != NULL;
}
void lfree(_LTexture* LT) //�ؽ��� ����ü�� �����ؼ� �η� �ʱ�ȭ
{
	if (LT != NULL)
	{
		if (LT->mTexture != NULL)
		{
			SDL_DestroyTexture(LT->mTexture);
			LT->mTexture = NULL;
			LT->mWidth = 0;
			LT->mHeight = 0;
		}
	}
}
void render(_LTexture* LT, SDL_Renderer* Renderer, int x, int y) //������
{
	SDL_Rect renderQuad = { x,y,LT->mWidth,LT->mHeight };
	SDL_RenderCopy(Renderer, LT->mTexture, NULL, &renderQuad);
}
bool loadFromRenderedText(_LTexture* LT, SDL_Renderer* Renderer, TTF_Font* Font, char* stringText, SDL_Color textColor)
{
	lfree(LT);
	unsigned short unicode[128]; //�����ڵ� �迭 ����
	han2unicode(stringText, unicode); //��Ʈ���� �����ڵ�� ��ȯ
	SDL_Surface* textSurface = TTF_RenderUNICODE_Solid(Font, unicode, textColor); //���ǽ� �����Ϳ� ��Ʈ �����Լ��ֱ�
	if (textSurface == NULL)
	{
		printf("�ؽ��� ���ǽ� ���� ����! : %s\n", TTF_GetError());
	}
	else
	{
		LT->mTexture = SDL_CreateTextureFromSurface(Renderer, textSurface);
		if (LT->mTexture == NULL)
		{
			printf("�ؽ��� ���� ���� : %s", SDL_GetError());
		}
		else
		{
			LT->mWidth = textSurface->w;
			LT->mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return LT->mTexture != NULL;
}//�ؽ��� �ε�
void V_handleEvent(_LPlayer* LP, SDL_Event* e) //Ű�Է¿� ���� ��ǥ����
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY += LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX -= LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX += LP->Player_VEL; break;
		}
	}
	else if (e->type == SDL_KEYUP && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY += LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX += LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX -= LP->Player_VEL; break;
		}
	}
}

void move(_LPlayer* LP, _LTile *tiles) //�÷��̾� ���� & �浹ó��
{
	LP->mBox.x += LP->mVelX;


	if ((LP->mBox.x < 0) || (LP->mBox.x + LP->Player_WIDTH > LEVEL_WIDTH) || touchesWall(LP->mBox, tiles))
	{
		LP->mBox.x -= LP->mVelX;
	}

	LP->mBox.y += LP->mVelY;

	if ((LP->mBox.y < 0) || (LP->mBox.y + LP->Player_HEIGHT > LEVEL_HEIGHT) || touchesWall(LP->mBox, tiles))
	{
		LP->mBox.y -= LP->mVelY;
	}
}
void setCamera(_LPlayer* LP, SDL_Rect* camera)
{
	camera->x = (getPosX(LP) + LP->Player_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera->y = (getPosY(LP) + LP->Player_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	if (camera->x > LEVEL_WIDTH - camera->w)
	{
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if (camera->y > LEVEL_HEIGHT - camera->h)
	{
		camera->y = LEVEL_HEIGHT - camera->h;
	}
}

void T_handleEvent(_LTexture* CT, _LTexture* LT, SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP:
			CT->mTexture = (LT + KEY_PRESS_SURFACE_UP)->mTexture;
			CT->mWidth = (LT + KEY_PRESS_SURFACE_UP)->mWidth;
			CT->mHeight = (LT + KEY_PRESS_SURFACE_UP)->mHeight;
			break;
		case SDLK_DOWN:
			CT->mHeight = (LT + KEY_PRESS_SURFACE_DOWN)->mHeight;
			CT->mWidth = (LT + KEY_PRESS_SURFACE_DOWN)->mWidth;
			CT->mTexture = (LT + KEY_PRESS_SURFACE_DOWN)->mTexture;
			break;
		case SDLK_LEFT:
			CT->mHeight = (LT + KEY_PRESS_SURFACE_LEFT)->mHeight;
			CT->mTexture = (LT + KEY_PRESS_SURFACE_LEFT)->mTexture;
			CT->mWidth = (LT + KEY_PRESS_SURFACE_LEFT)->mWidth;
			break;
		case SDLK_RIGHT:
			CT->mHeight = (LT + KEY_PRESS_SURFACE_RIGHT)->mHeight;
			CT->mTexture = (LT + KEY_PRESS_SURFACE_RIGHT)->mTexture;
			CT->mWidth = (LT + KEY_PRESS_SURFACE_RIGHT)->mWidth;
			break;
		case SDLK_o:
			CT->mHeight = (LT + KEY_PRESS_SURFACE_O)->mHeight;
			CT->mWidth = (LT + KEY_PRESS_SURFACE_O)->mWidth;
			CT->mTexture = (LT + KEY_PRESS_SURFACE_O)->mTexture;
			break;
		default:
			CT->mHeight = (LT + KEY_PRESS_SURFACE_DEFAULT)->mHeight;
			CT->mWidth = (LT + KEY_PRESS_SURFACE_DEFAULT)->mWidth;
			CT->mTexture = (LT + KEY_PRESS_SURFACE_DEFAULT)->mTexture;
			break;
		}
	}
}
bool checkCollision(SDL_Rect a, SDL_Rect b) //�浹���� �Լ�(�浹-> return false)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int botA, botB;
	//�浹���� �簢�� ����

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	botA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	botB = b.y + b.h;

	if (botA <= topB)
	{
		return false;
	}
	if (topA >= botB)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}
int getPosX(_LPlayer* LP)
{
	return LP->mBox.x;
}
int getPosY(_LPlayer* LP)
{
	return LP->mBox.y;
}
bool setTiles(_LTile *tiles)
{
	printf("setTiles");
	bool loaded = true;
	int x = 0, y = 0, i = 0;

	char tempbuf[2304];
	char* map[2304] = { 0, };
	FILE *fp = fopen("level.map", "r");

	fgets(tempbuf, sizeof(tempbuf), fp);

	char *ptr = strtok(tempbuf, " ");

	while (ptr != NULL)
	{
		map[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}

	if (fp == NULL)
	{
		printf("������ �ε� ����!\n");
		loaded = false;
	}
	else
	{
		printf("else");
		i = 0;
		for (int i = 0; i < 763; i++)
		{

			int tileType = -1;
			if (!strcmp(map[i], "00"))
			{
				tileType = 0;
				printf("\n set tileType : 00");
			}
			else if (!strcmp(map[i], "01"))
			{
				tileType = 1;
				printf("\n set tileType : 01");
			}
			else if (!strcmp(map[i], "02"))
			{
				tileType = 2;
				printf("\n set tileType : 02");
			}
			else if (!strcmp(map[i], "03"))
			{
				tileType = 3;
				printf("\n set tileType : 03");
			}
			else if (!strcmp(map[i], "04"))
			{
				tileType = 4;
				printf("\n set tileType : 04");
			}
			else if (!strcmp(map[i], "05"))
			{
				tileType = 5;
				printf("\n set tileType : 05");
			}
			else if (!strcmp(map[i], "06"))
			{
				tileType = 6;
				printf("\n set tileType : 06");
			}
			else if (!strcmp(map[i], "07"))
			{
				tileType = 7;
				printf("\n set tileType : 07");
			}
			else if (!strcmp(map[i], "08"))
			{
				tileType = 8;
				printf("\n set tileType : 08");
			}
			else if (!strcmp(map[i], "09"))
			{
				tileType = 9;
				printf("\n set tileType : 09");
			}
			else if (!strcmp(map[i], "10"))
			{
				tileType = 10;
				printf("\n set tileType : 10");
			}
			else if (!strcmp(map[i], "11"))
			{
				tileType = 11;
				printf("\n set tileType : 11");
			}

			printf("\nready to set tiles");
			(tiles + i)->mBox.x = x;
			(tiles + i)->mBox.y = y;
			(tiles + i)->mBox.w = TILE_WIDTH;
			(tiles + i)->mBox.h = TILE_HEIGHT;
			(tiles + i)->mType = tileType;

			printf("tiles[%d] = %d\n", i, (tiles + i)->mType);
			x += TILE_WIDTH;

			if (x >= LEVEL_WIDTH)
			{
				x = 0;
				y += TILE_HEIGHT;
			}

		}
	}
	printf("\n Finish setting tiles successfully");
	fclose(fp);
	return loaded;
}

bool touchesWall(SDL_Rect box, _LTile* tiles)
{
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		if (((tiles + i)->mType >= 3) && ((tiles + i)->mType <= 11))
		{
			if (checkCollision(box, (tiles + i)->mBox))
			{
				return true;
			}
		}
	}
	return false;
}