#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//SDL �̴ϼȶ���¡&������ ����
bool init();
//�̵�� �ε�
bool loadMedia();
//��� �ݱ�
void close();
//������ ������
SDL_Window* gWindow = NULL; //�׻� null�� �ʱ�ȭ �Ұ�
//sdl surface
SDL_Surface* gScreenSurface = NULL; //�׻� null�� �ʱ�ȭ �Ұ�
//ǥ���� �̹���
SDL_Surface* gHelloWorld = NULL; //�׻� null�� �ʱ�ȭ �Ұ�

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("�̴ϼȶ����� ����!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("�̵�� �ε� ����!\n");
		}
		else
		{
			//���� ���� �÷���
			bool quit = false;

			//�̺�Ʈ �ڵ鷯
			SDL_Event e;

			//�������϶�
			while (!quit)
			{
				//ť���ٰ� �̺�Ʈ �ڵ�
				while (SDL_PollEvent(&e) != 0)
				{
					//���� ������ ���� ��û��
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				//�ε�� surface�� surface ���� blit
				SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
				//��ũ�� ������Ʈ
				SDL_UpdateWindowSurface(gWindow);
			}

			
			
			
		}
	}
	close();

	return 0;
}

bool init()
{
	//init flag
	bool success = true;

	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init_ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//������ ����
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("������ ���� ���� : %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//window surface ��������
			gScreenSurface = SDL_GetWindowSurface(gWindow); 
		}
	}

	return success;
}

bool loadMedia()
{
	//�÷���
	bool success = true;
	//��Ʈ���̹��� �ҷ�����
	gHelloWorld = SDL_LoadBMP("test.bmp");

	if (gHelloWorld == NULL)
	{
		printf("�̹��� �ε� ���� : %s", SDL_GetError());
		success = false;
	}
	return success;
}

void close()
{
	//���ǽ� ����
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;
	//������ �ı�
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//SDL����
	SDL_Quit();
}