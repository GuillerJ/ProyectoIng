/**
  * Changelog:
  * V1.0 Botones cerra y crear linea funcionales. Ya pinta lineas.
  * V1.1 Corregida la documentación
  * V1.2 Codigo ordenado.
*/


#include <iostream>
#include <string>
#include <SDL.h>
#include "cleanup.h"


int main(int, char**){

    // inicia SDL
    SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// crea la ventana
    SDL_Window *win = SDL_CreateWindow("Gestor de eventos de teclado y raton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // crea el "renderer"
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 1);
    if (ren == nullptr){
        cleanup(win); // SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    std::string imagePath1 = "panel3.bmp";

    SDL_Surface *panel = SDL_LoadBMP(imagePath1.c_str());
    if (panel == nullptr){
        cleanup(ren, win); // SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
        std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, panel);
    if (texPanel == nullptr){
        cleanup(ren, win); // SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(ren, 0, 204, 255, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawLine(ren, 10, 10, 200, 200);

    SDL_RenderCopy(ren, texPanel, NULL, NULL);
    SDL_RenderPresent(ren);

	SDL_Event e;

    ///Variables
    bool quit = false;
    int i=0;
    bool pinta=false;
    ///Variables END

	while (!quit){

		while (SDL_PollEvent(&e)){

			if (e.type == SDL_QUIT){
				quit = true;
			}

            //Pulsaciones de Teclado
			if (e.type == SDL_KEYDOWN) {
                SDL_Keycode key = e.key.keysym.sym;
                std::cout << "Tecla pulsada: " << SDL_GetKeyName(key) << std::endl;
                if (key == SDLK_ESCAPE){
                    quit = true;
                }
			}

            //Pulsaciones de Raton
			if (e.type == SDL_MOUSEBUTTONDOWN){
				std::cout << "Boton de raton pulsado en X = " << e.button.x << "\tY = " << e.button.y << std::endl;

                //BOTON DE CERRAR
                if ((e.button.x > 708 && e.button.x < 800) & (e.button.y > 0 && e.button.y < 55)){
                    quit = true;
                }

                //BOTON CREAR LINEA
                if ((e.button.x > 708 && e.button.x < 800) & (e.button.y > 115 && e.button.y < 168)){
                    if (pinta == false)
                        pinta=true;
                    else
                        pinta=false;
                }

                //Panel Para pintar lineas
                if ((pinta == true) & (e.button.x > 12 && e.button.x < 692) & (e.button.y > 77 && e.button.y < 588)){

                    int x1;
                    int y1;
                    int x2;
                    int y2;

                    if (i==0){
                        std::cout <<" 1 ";
                        x1 = e.button.x;
                        y1 = e.button.y;
                    }
                    if (i==1){
                        std::cout <<" 2 ";
                        x2 = e.button.x;
                        y2 = e.button.y;
                        SDL_RenderDrawLine(ren, x1, y1, x2, y2);
                    }
                    i++;
                    if (i==2)
                        i=0;
                }

			}

		}

        SDL_RenderPresent(ren);
	}

    cleanup(ren, win);
	std::cout << "Odontologic Cerrado. Adios" << std::endl;
	SDL_Quit();

	return 0;
}
