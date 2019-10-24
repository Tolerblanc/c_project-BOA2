#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640; //��ũ�� ����
const int SCREEN_HEIGTH = 480; //��ũ�� ����

enum KeyPressSurfaces //Ű�Է� ����
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL,
};

bool init(); //�ʱ�ȭ
bool loadMedia(); //�̵�� �ε�
void close(); // SDL ���̺귯�� �ݱ�

SDL_Surface* loadSurface(char* path);

SDL_Window* gWindow = NULL; //������ ������ ������

SDL_Surface* gScreenSurface = NULL; //��ũ�����ǽ� ������

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
				}

				//�̹��� ����
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				//���ǽ� ������Ʈ
				SDL_UpdateWindowSurface(gWindow);
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
		gWindow = SDL_CreateWindow("image renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGTH, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("�����츦 ������ �� �����ϴ�. : %s", SDL_GetError());
			Sflag = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);

			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
		
			SDL_UpdateWindowSurface(gWindow);
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

bool loadMedia()
{
	bool Sflag = true; //success flag

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

	return Sflag;
}

void close()
{
	//���ǽ� �ʱ�ȭ
	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;

	//������ �ݱ�
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//sdl ���̺귯�� �ݱ�
	SDL_Quit();
}

