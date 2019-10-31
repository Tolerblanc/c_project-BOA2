Z#include <SDL.h> //SDL lib
#include <stdio.h> 

//��ũ�� ������ ����
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
written by HyunJun KIM (Tolerblanc)
hihj070914@icloud.com
2019/10/20 11:45
Simple SDL Test
*/

/*
SDL ����
http://libsdl.org/download-2.0.php
Development Libraries - Windows:SDL2-devel-2.9.10-VC.zip(Visual C++ 32/64-bit) �ٿ�ε� ��
http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php
��� ���� ����
*/

int main(int argc, char args[])
{
	SDL_Window* window = NULL; //������ ������ 

	SDL_Surface* screenSurface = NULL;

	//SDL �ʱ�ȭ
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//������ ����
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//������ ���ǽ� ������
			screenSurface = SDL_GetWindowSurface(window);

			//�����츦 �Ͼ������ ä��
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//���ǽ� ������� ������Ʈ
			SDL_UpdateWindowSurface(window);

			//������
			SDL_Delay(2000);
		}
	}
	//������ ����
	SDL_DestroyWindow(window);

	//SDL ���̺귯�� ����
	SDL_Quit();

	return 0;
}
