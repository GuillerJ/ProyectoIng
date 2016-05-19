/**
* Changelog:
* V1.0 Botones cerra y crear linea funcionales. Ya pinta lineas.
* V1.1 Corregida la documentación
* V1.2 Codigo ordenado.
* V1.3 Aviso de uso de la herramienta crear lienas.
	   Archivos GUI movidos a la carpeta Recursos.
* V1.4 Carga una imagen
* V1.4.1	Cambiado el nombre a la variable vecesPintar
* V2.0 Calibrado conforme al nuevo panel de la GUI. Eliminado la GUI de Windows. Mensajes de error cambiados. Añadido excepción si no encuentra ortopantomografia en su lugar.
* V2.0.1	Aumento de tamaño de los botones cerrar y minimizar


* TODO:
* 
*/



#include <iostream>
#include <string>
#include "SDL2\SDL.h"
#include "cleanup.h"

int main(int, char**) {

	// inicia SDL
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// crea la ventana
	SDL_Window *win = SDL_CreateWindow("Odontologic V1.4.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);
	if (win == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// crea el "renderer"
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 1);
	if (ren == nullptr) {
		cleanup(win); // SDL_DestroyWindow(win);
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::string imagePath1 = "Recursos/panel8.bmp";

	SDL_Surface *Panel = SDL_LoadBMP(imagePath1.c_str());
	if (Panel == nullptr) {
		cleanup(ren, win); // SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
		std::cerr << "SDL_LoadBMP del panel de la GUI Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, Panel);
	if (texPanel == nullptr) {
		cleanup(ren, win); // SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
		std::cerr << "SDL_CreateTextureFromSurface de la GUI Error: " << SDL_GetError() << std::endl;
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

	//*****************Variables***********************
	bool quit = false;
	register int vecesPintar = 0;
	bool pinta = false;
	bool ayuda = false;
	std::string PathAyuda = "Recursos/panel1.bmp";
	SDL_Surface *Ayuda = SDL_LoadBMP(PathAyuda.c_str());
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	//*****************Variables END*******************

	while (!quit) {
		
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			//Pulsaciones de Teclado
			if (e.type == SDL_KEYDOWN) {
				SDL_Keycode key = e.key.keysym.sym;
				std::cout << "Tecla pulsada: " << SDL_GetKeyName(key) << std::endl;
				
				if (key == SDLK_ESCAPE) {
					quit = true;
				}
			}

			//Pulsaciones de Raton
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				std::cout << "Boton de raton pulsado en X = " << e.button.x << "\tY = " << e.button.y << std::endl;

				//BOTON DE NUEVA IMAGEN
				if ((e.button.x > 22 && e.button.x < 48) & (e.button.y > 42 && e.button.y < 67)) {
					//Preguntar por el nombre del archivo
					//SDL_Window *win1 = SDL_CreateWindow("Nombre de la Imagen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_SHOWN);

					SDL_Texture* Ortopantomografia;
					SDL_Rect SrcR;
					SDL_Rect DestR;
					
					SrcR.x = 0;
					SrcR.y = 0;
					SrcR.w = 780;
					SrcR.h = 510;

					DestR.x = 11;
					DestR.y = 80;
					DestR.w = 780;
					DestR.h = 510;

					SDL_Surface* Loading_Surf = SDL_LoadBMP("Ortopantomografia/OP1.bmp");
					if (Loading_Surf == nullptr) {
						cleanup(ren, win); // SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
						std::cerr << "Error al cargar la Ortopantomografía. Codigo de Error: " << SDL_GetError() << std::endl;
						SDL_Quit();
						return 1;
					}
					Ortopantomografia = SDL_CreateTextureFromSurface(ren, Loading_Surf);
					SDL_FreeSurface(Loading_Surf);
					SDL_RenderCopy(ren, Ortopantomografia, &SrcR, &DestR);
				}

				//BOTON DE AYUDA
				if ((e.button.x > 748 && e.button.x < 775) & (e.button.y > 43 && e.button.y < 70)) {
					//Modificar la imagen de ayuda y meter con ella la otropan si se encuntra abiera
					if (ayuda == false) {
						ayuda = true;
						SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, Ayuda);
						SDL_RenderCopy(ren, texPanel, NULL, NULL);
						SDL_RenderPresent(ren);
					}else {
						ayuda = false;
						SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, Panel);
						SDL_RenderCopy(ren, texPanel, NULL, NULL);
						SDL_RenderPresent(ren);
					}
				}
				
				//BOTON DE CERRAR
				if ((e.button.x > 777 && e.button.x < 800) & (e.button.y > 0 && e.button.y < 24)) {
					quit = true;
				}
				//BOTON DE MINIMIZAR
				if ((e.button.x > 750 && e.button.x < 777) & (e.button.y > 0 && e.button.y < 24)) {
					SDL_MinimizeWindow(win);
				}
				//BOTON CREAR LINEA
				if ((e.button.x > 109 && e.button.x < 234) & (e.button.y > 46 && e.button.y < 67)) {
					SDL_Rect rect1;
					rect1.x = 212;
					rect1.y = 51;
					rect1.w = 9;
					rect1.h = 9;

					if (pinta == false) {
						pinta = true;
						SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
						SDL_RenderFillRect(ren, &rect1);
						SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
					}else {
						SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
						SDL_RenderFillRect(ren, &rect1);
						pinta = false;
					}
				}

				//Panel Para pintar lineas
				if ((pinta == true) & (e.button.x > 10 && e.button.x < 790) & (e.button.y > 80 && e.button.y < 590)) {
					
					if (vecesPintar == 0) {
						std::cout << " 1 ";
						x1 = e.button.x;
						y1 = e.button.y;
					}
					if (vecesPintar == 1) {
						std::cout << " 2 ";
						x2 = e.button.x;
						y2 = e.button.y;
						SDL_RenderDrawLine(ren, x1, y1, x2, y2);
					}
					vecesPintar++;
					if (vecesPintar == 2)
						vecesPintar = 0;

				}
			}
		}

		SDL_RenderPresent(ren);
	}

	cleanup(ren, win);
	std::cout << "\n\tOdontologic Cerrado. Adios" << std::endl;
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}