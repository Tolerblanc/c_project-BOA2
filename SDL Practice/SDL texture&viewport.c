#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib

const int SCREEN_WIDTH = 640; //��ũ�� ����
const int SCREEN_HEIGHT = 480; //��ũ�� ����

enum KeyPressSurfaces //Ű�Է� ����
{
	KEY_PRESS_SURFACE_DEFAULT, //=0
	KEY_PRESS_SURFACE_UP,      //=1
	KEY_PRESS_SURFACE_DOWN,    //=2
	KEY_PRESS_SURFACE_LEFT,    //=3
	KEY_PRESS_SURFACE_RIGHT,   //=4
	KEY_PRESS_SURFACE_TOTAL,   //=5
};

bool init(); //�ʱ�ȭ
bool loadMedia(); //�̵�� �ε�
void close(); // SDL ���̺귯�� �ݱ�

SDL_Surface* loadSurface(char* path); //��ο� �ִ� ���ǽ� �ε� �Լ�
SDL_Texture* loadTexture(char* path); //���� ���� ��� ( ���ǽ����� ���� ����)

SDL_Window* gWindow = NULL; //������ ������ ������
SDL_Surface* gScreenSurface = NULL; //��ũ�����ǽ� ������


SDL_Renderer* gRenderer = NULL;     //������ ������

SDL_Texture* gTexture = NULL;       //�⺻ �ؽ��� ������
SDL_Texture* gLetterbox = NULL;
SDL_Texture* gMinimap = NULL;

SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; //Ű�� ���� �̹��� ������

SDL_Surface* gCurrentSurface = NULL; //���� ǥ�õǴ� �̹����� ����Ű�� ������


int main(int argc, char argcs[])
{
	if (!init())
	{
		printf("�ʱ�ȭ ����!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//�⺻ Ű ���� ���ǽ�
			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					/*
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							printf("UP key\n");
							break;
						case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							printf("DOWN key\n");
							break;
						case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							printf("LEFT key\n");
							break;
						case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							printf("RIGHT key\n");
							break;
						default:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
					*/
				}

				//�̹��� ����
				//SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				//���ǽ� ������Ʈ
				//SDL_UpdateWindowSurface(gWindow);

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				//���ȭ�� ������ �� ������ Ŭ����

				SDL_Rect topViewport;
				topViewport.x = 0;
				topViewport.y = 0;
				topViewport.w = SCREEN_WIDTH;
				topViewport.h = SCREEN_HEIGHT *2 / 3;
				SDL_RenderSetViewport(gRenderer, &topViewport); //������ ������ ����Ʈ�� ������ ����

				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL); //�ؽ��� -> ������ �� ī��
				//���� ȭ�鿡 ���� ������ ���� 
				

				SDL_Rect botLeftViewport;
				botLeftViewport.x = 0;
				botLeftViewport.y = SCREEN_HEIGHT * 2 / 3;
				botLeftViewport.w = SCREEN_WIDTH * 2 / 3;
				botLeftViewport.h = SCREEN_HEIGHT / 3;
				SDL_RenderSetViewport(gRenderer, &botLeftViewport);

				SDL_RenderCopy(gRenderer, gLetterbox, NULL, NULL);

				

				SDL_Rect botRightViewport;
				botRightViewport.x = SCREEN_WIDTH * 2 / 3;
				botRightViewport.y = SCREEN_HEIGHT * 2 / 3;
				botRightViewport.w = SCREEN_WIDTH  / 3;
				botRightViewport.h = SCREEN_HEIGHT / 3;
				SDL_RenderSetViewport(gRenderer, &botRightViewport);

				SDL_RenderCopy(gRenderer, gMinimap, NULL, NULL);

				SDL_RenderPresent(gRenderer);  //Update
			}
		}

	}

	close();

	return 0;
}

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
		gWindow = SDL_CreateWindow("Viewport  Setting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
				if(!(IMG_Init(imgFlags)&imgFlags))
				{
					printf("SDL�̹��� ����� �ʱ�ȭ �� �� �����ϴ�! : %s",IMG_GetError());
					Sflag = false;
				}
				else
				{
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}
				
			}
		}
	}
	return Sflag;
	
}

SDL_Surface* loadSurface(char *path)
{
	//����� �̹��� �ε�
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL) {
		printf("�̹��� �ε� ����! : %s\n", SDL_GetError());
	}
	return loadedSurface;
}

SDL_Texture* loadTexture(char *path)
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

bool loadMedia()
{
	bool Sflag = true; //success flag

	/* ����Ű�� ���� ���ǽ� �ε� 
	//����Ʈ
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("example.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("����Ʈ �̹��� �ε� ����!\n");
		Sflag = false;
	}
	//��
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("�� �̹��� �ε� ����!\n");
		Sflag = false;
	}
	//�ٿ�
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("�Ʒ� �̹��� �ε� ����!\n");
		Sflag = false;
	}
	//����
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("���� �̹��� �ε� ����!\n");
		Sflag = false;
	}
	//������
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("������ �̹��� �ε� ����!\n");
		Sflag = false;
	}
	*/
	gTexture = loadTexture("example.png");
	if (gTexture == NULL)
	{
		printf("�ؽ����̹��� �ε� ����!\n");
		Sflag = false;
	}

	gLetterbox = loadTexture("Lbox.png");
	if (gLetterbox == NULL)
	{
		printf("�ؽ����̹���(gLetterbox) �ε� ����!\n");
		Sflag = false;
	}

	gMinimap = loadTexture("map.png");
	if (gMinimap == NULL)
	{
		printf("�ؽ����̹���(gMinimap) �ε� ����!\n");
		Sflag = false;
	}
	//���� ���� ���ϴ� �̹����� �����Ϳ� ������ �������� ���� ��ġ���Ѽ� ������Ʈ 
	return Sflag;
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	
	//���ǽ� �ʱ�ȭ
	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;

	//������ �ݱ�
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//sdl ���̺귯�� �ݱ�
	SDL_Quit();
	IMG_Quit();
}

