#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <SDL_ttf.h>              //SDL ttf lib
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include <stdint.h>               //use uint32_t
#include "game.h"
#include "timer.h"

SDL_Renderer* gRenderer = NULL;     //������ ������
SDL_Surface* loadSurface(char* path); //��ο� �ִ� ���ǽ� �ε� �Լ�
SDL_Texture* loadTexture(char* path); //���� ���� ��� ( ���ǽ����� ���� ����)
SDL_Window* gWindow = NULL; //������ ������ ������
SDL_Surface* gScreenSurface = NULL; //��ũ�����ǽ� ������
SDL_Texture* gTexture = NULL;       //�⺻ �ؽ��� ������
SDL_Texture* gLetterbox = NULL;     //���ʾƷ� �޽����ڽ� ������
SDL_Texture* gMinimap = NULL;       //������ �Ʒ� �̴ϸ� ������
TTF_Font* gFont = NULL;  //�⺻ ��Ʈ ������

const int LEVEL_WIDTH = 2560;  //������ ����
const int LEVEL_HEIGHT = 1920; //������ ����

const int SCREEN_WIDTH = 1280; //��ũ�� ����
const int SCREEN_HEIGHT = 720; //��ũ�� ����

const int SCREEN_FPS = 60;     //��ũ�� ������
const int SCREEN_TICK_PER_FRAME = 17; //ƽ�� ������

//Ÿ�� ���
const int TILE_WIDTH = 80;          //Ÿ�� ����ũ��
const int TILE_HEIGHT = 80;         //Ÿ�� ����ũ��
const int TOTAL_TILES = 768;        //�� Ÿ�� ���� (32x24)
const int TOTAL_TILE_SPRITES = 23;  //�� Ÿ�� ��������Ʈ ����

//�ؽ��� �� ������ ����ü�� ����
struct _LTexture gMainplayerTexture[KEY_PRESS_SURFACE_TOTAL+2]; //����ĳ���� �ؽ���
struct _LTexture gCurrentSurface; //���� ǥ�õǴ� ���ǽ�
struct _LTexture gDuckTexture[KEY_PRESS_SURFACE_TOTAL+2];  //���� �ؽ�ó
struct _LTexture gCurrentDuck;

struct _LTexture gTimeText;
struct _LTexture gCurrentTime;
struct _LTexture gSightLimiter;    //�þ� ������
struct _LTexture gTextTexture[2];  //�ý�Ʈ�� ����ü �迭�� ���Ͽ� �̸� ����־����, �̺�Ʈ�� ���� ������ ������ �� �� �ִ�
struct _LTexture gCurrentText;     //���� ǥ�õǴ� �ؽ�Ʈ

struct _LTexture gTileTexture[23]; //Ÿ�ϼ� �ؽ�ó
struct _LTexture gLeaderBoard[5];
struct _LTexture gScore[5];
struct _LTexture gStartText;

struct _LPlayer gPlayer;
struct _LPlayer gDuck[5];

struct _LTimer timer; 


int main()
{
	srand(time(NULL));
	//SDL ��� �ʱ�ȭ
	if (!init())
	{
		printf("�ʱ�ȭ ����!\n");
	}
	else
	{
		char time[150];
		_LTile tileSet[12000] = { 0, };
		//�̵�� �ε�
		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//���η��� �÷���
			bool quit = false;     //SDL WINDOW Ż�� �÷���
			bool game_end = false; //���� ���� Ż�� �÷���

			//���ھ� ����
			int score = 0;

			//�̺�Ʈ �ڵ鷯
			SDL_Event e;

			//ī�޶� ����ü ����
			SDL_Rect Camera;
			Camera.x = 0;
			Camera.y = 0;
			Camera.w = SCREEN_WIDTH;
			Camera.h = SCREEN_HEIGHT * 2 / 3;  //���� ����Ʈ�� ������ 2/3 �����̱� ������ ī�޶� ���̸� ������ 2/3���� ����

			//�⺻ Ű ���� ���ǽ�
			gCurrentSurface.mTexture = gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mTexture;
			gCurrentSurface.mHeight = gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mHeight;
			gCurrentSurface.mWidth = gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mWidth;

			//�⺻ �ؽ�Ʈ ����
			gCurrentText.mTexture = gTextTexture[0].mTexture;
			gCurrentText.mHeight = gTextTexture[0].mHeight;
			gCurrentText.mWidth = gTextTexture[0].mWidth;

			//�⺻ ���� ��ġ ����
			for (int i = 0; i < 5; i++)
			{
				do {
					int ranX = rand() % LEVEL_WIDTH;
					int ranY = rand() % LEVEL_HEIGHT;
					gDuck[i].mBox.x = ranX;
					gDuck[i].mBox.y = ranY;
				} while (touchesWall(gDuck[i].mBox, tileSet));//���� ������ ��ġ�� ���ϼ��� ������ ����ó��
			}

			//fps����� & ����Ÿ�̸ӿ� Ÿ�̸� ����ü ����
			struct _LTimer fpsTimer;
			struct _LTimer capTimer;
			int  countedFrames = 0;
			timer_start(&fpsTimer);
			int Stime = SDL_GetTicks() / 1000;

			//���η��� 
			while (!quit)
			{
				timer_start(&capTimer);
				//�̺�Ʈ ó��
				while (SDL_PollEvent(&e) != 0)
				{
					//���� ȣ��� ����
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN) //Ű�ٿ� �̺�Ʈ ó��
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_RETURN:
							game_end = false;
							break;
						
						default:
							gCurrentText.mTexture = gTextTexture[0].mTexture;
							gCurrentText.mHeight = gTextTexture[0].mHeight;
							gCurrentText.mWidth = gTextTexture[0].mWidth;
							gCurrentSurface.mHeight = gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mHeight;
							gCurrentSurface.mWidth = gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mWidth;
							gCurrentSurface.mTexture = gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mTexture;
						}
					}

					V_handleEvent(&gPlayer, &e); //Ű�ٿ ���� �̵� �̺�Ʈ
					for (int i = 0; i < 5; i++)
						reverse_V_handleEvent(&gDuck[i], &e); //Ű�ٿ ���� ���� �̵� �̺�Ʈ
					T_handleEvent(&gCurrentSurface, &gMainplayerTexture, &e, SDL_GetTicks() / 250);  //Ű�ٿ ���� �ؽ��� ����
					reverse_T_handleEvent(&gCurrentDuck, &gDuckTexture, &e, SDL_GetTicks());         //������ �ִϸ��̼� �ܿ� ���� �ð� �����Ͻ� ����� �ٲ�� �ϱ� ������ �ð��� ms������ ����
				}

				//������ ���ѿ� ����
				float avgFPS = countedFrames / (getTicks(&fpsTimer) / (float)1000);
				if (avgFPS > 2000000)
				{
					avgFPS = 0;
				}

				//�������� ����
				if (game_end)
				{
					SDL_RenderClear(gRenderer);
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); //������ ���� ���������� ����

					score = 0; //���ھ� ���� �ʱ�ȭ

					gPlayer.mBox.x = 0;
					gPlayer.mBox.y = 0;//�÷��̾� ��ġ �ʱ�ȭ

					render(&gDuckTexture[5], gRenderer, 80, 80);

					for (int i = 0; i < 5; i++)
					{
						render(&gLeaderBoard[i], gRenderer, 240, 75 * (i + 1));
					}//�������� ���� ����

					for (int i = 0; i < 5; i++)
					{
						render(&gLeaderBoard[i], gRenderer, 300, 75 * (i + 1));
					}//�������� ���ھ� ����


					for (int i = 0; i < 5; i++)
					{
						do {
							int ranX = rand() % LEVEL_WIDTH;
							int ranY = rand() % LEVEL_HEIGHT;
							gDuck[i].mBox.x = ranX;
							gDuck[i].mBox.y = ranY; //������ġ ���� ����
						} while (touchesWall(gDuck[i].mBox, tileSet)); //���� ������ ��ġ�� ���ϼ��� ������ ����ó��

					}//������ġ&�ӵ� �缳��

					render(&gStartText, gRenderer, 250, 420);
					SDL_RenderPresent(gRenderer);

					//���ھ� ���&�����ð� ����� ���� ������ �����ϴ� ������ �ð��� �����ص�
					Stime = SDL_GetTicks() / 1000;
				}
				else
				{

					//�ð� ǥ�ø� ���� ���� ����
					SDL_Color timeColor = { 255,255,255 };
					int Ctime = 60 - (SDL_GetTicks() / 1000 - Stime);
					sprintf(time, "%d", Ctime);

					score = (SDL_GetTicks() - Stime * 1000); //�ð�(ms)�� �� ���ھ ��

					if (!loadFromRenderedText(&gCurrentTime, gRenderer, gFont, time, timeColor))
					{
						printf("���� Ÿ�̸Ӹ� ������ �� �����ϴ�! \n");
					}
					if (Ctime <= 0) //���� �ð��� 0�����϶� �й�
					{
						printf("Ÿ�ӿ���!\n");
						score /= 2; //���Ŵϱ� ���ھ� Ÿ�뽺
						printf("\nScore : %d", score);
						game_end = true;
					}

					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); //������ ���� ���������� ����
					SDL_RenderClear(gRenderer);
					//����ȭ�� ������ ���� �� ������ Ŭ����

					//ĳ���� �̵� & ī�޶� ����
					move(&gPlayer, &tileSet); //Ÿ�ϼ°� �÷��̾��� �浹ó��
					for (int i = 0; i < 5; i++)
					{
						move(&gDuck[i], &tileSet); //Ÿ�ϼ°� ������ �浹ó��
						if (checkCollision(gPlayer.mBox, (gDuck + i)->mBox)) //������ �÷��̾� �浹�� �й�
						{
							score /= 2; //���Ŵϱ� ���ھ� Ÿ�뽺
							printf("\nScore : %d", score);
							game_end = true;
						}
					}
					setCamera(&gPlayer, &Camera); //ī�޶� ����(�������� �κ� ����)


					if (checkCollision(gPlayer.mBox, tileSet[733].mBox)) //Ż�ⱸ Ÿ�ϰ� �浹�� �¸�
					{
						printf("\nScore : %d", score);
						game_end = true;
					}


					SDL_Rect botLeftViewport; //�α׹ڽ� ����Ʈ(���� �Ʒ�)
					botLeftViewport.x = 0;
					botLeftViewport.y = SCREEN_HEIGHT * 2 / 3;
					botLeftViewport.w = SCREEN_WIDTH * 2 / 3;
					botLeftViewport.h = SCREEN_HEIGHT / 3;
					SDL_RenderSetViewport(gRenderer, &botLeftViewport);

					SDL_RenderCopy(gRenderer, gLetterbox, NULL, NULL);

					//����(Ÿ�̸�, �ؽ�Ʈ)
					render(&gTimeText, gRenderer, 30, 30);
					render(&gCurrentTime, gRenderer, 130, 30);
					if (Ctime <= 30) //�����ð��� ���� �����϶� 
					{
						gCurrentText.mTexture = gTextTexture[1].mTexture;
						gCurrentText.mHeight = gTextTexture[1].mHeight;
						gCurrentText.mWidth = gTextTexture[1].mWidth;//�ؽ�Ʈ�� �ٲ���
					}
					render(&gCurrentText, gRenderer, 30, 60);

					SDL_Rect botRightViewport; //�̴ϸ� ����Ʈ (������)
					botRightViewport.x = SCREEN_WIDTH * 2 / 3;
					botRightViewport.y = SCREEN_HEIGHT * 2 / 3;
					botRightViewport.w = SCREEN_WIDTH / 3;
					botRightViewport.h = SCREEN_HEIGHT / 3;
					SDL_RenderSetViewport(gRenderer, &botRightViewport);

					SDL_RenderCopy(gRenderer, gMinimap, NULL, NULL);
					//��.png ��.png �ΰ��� ����� Ÿ�ϳѹ��� ���� ���еڿ� 1/3ũ��� ������ �ϸ� �ɵ� ���� tileSet �迭 �����ϱ�


					SDL_Rect topViewport; //���� ���� ����Ʈ
					topViewport.x = 0;
					topViewport.y = 0;
					topViewport.w = SCREEN_WIDTH;
					topViewport.h = SCREEN_HEIGHT * 2 / 3;
					SDL_RenderSetViewport(gRenderer, &topViewport); //������ ������ ����Ʈ�� ������ ����

					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);


					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						if (checkCollision(Camera, (tileSet + i)->mBox))
						{
							render(&gTileTexture[(tileSet + i)->mType], gRenderer, (tileSet + i)->mBox.x - Camera.x, (tileSet + i)->mBox.y - Camera.y);
						} //ī�޶� ���ʿ� ������ Ÿ�ϵ鸸 ��������
					}

					//!!!������ ���� �߿���!!! 
					for (int i = 0; i < 5; i++)
						render(&gCurrentDuck, gRenderer, gDuck[i].mBox.x - Camera.x, gDuck[i].mBox.y - Camera.y); //���� ����
					render(&gCurrentSurface, gRenderer, gPlayer.mBox.x - Camera.x, gPlayer.mBox.y - Camera.y); //�÷��̾� ����
					render(&gSightLimiter, gRenderer, gPlayer.mBox.x - 1350 - Camera.x, gPlayer.mBox.y - 670 - Camera.y);  //����Ʈ ������(�÷��̾�� ���� ������) �÷��̾� �⺻��ġ�� ���־�� ��Ȯ�� ����� ��ġ
					

					SDL_RenderPresent(gRenderer);  //������ �Ȱ� ��� ������Ʈ 
					++countedFrames; //������ �����
				}

				int frameTicks = getTicks(&capTimer);
				if (frameTicks < SCREEN_TICK_PER_FRAME)
				{
					SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
				}//�ʴ� 60������ �̻� ���� ����
			}
		}
	}
	close();
	return 0;
}



