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
* V2.1 Bool para imagen vista para que la ayuda sepa si hay o no imagen cargada. Boton de mentionano cogido. Añadido un icono a la aplicación
* V2.2 Ahora si esta activado el boton mentoniano el mandibular se apaga y viceversa. La función de mandibular solo deja pintar 2 lineas y se desactiva
* V2.2.1	Si la imagen no está cargada no deja pintar
* v2.3 Incluida la función de mentoniano ya pinta una cruceta para señalarlo
* V2.3.1	//poner OR en el mentoniano y cmand para luego sacar las rectas en esta misma función

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
		std::cerr << "SDL_CreateWindow \n\tError: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// crea el "renderer"
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 1);
	if (ren == nullptr) {
		cleanup(win); // SDL_DestroyWindow(win);
		std::cerr << "SDL_CreateRenderer \n\tError: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//Cargar el icono
	std::string PathIcono = "Recursos/MiO4.bmp";
	SDL_Surface *Icono = SDL_LoadBMP(PathIcono.c_str());
	if (Icono == nullptr) {
		cleanup(ren, win); // SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
		std::cerr << "Error al cargar el Icono de la aplicacion. \n\tCodigo de Error de SDL_LoadBMP: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_SetWindowIcon(win, Icono);


	std::string imagePath1 = "Recursos/panel9.bmp";

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
		std::cerr << "Error al cargar la imagen GUI \n\tCodigo de Error de SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
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
	bool imagenOn = false;
	bool cMand = false;
	int vecesMand = 0;
	bool mentoniano = false;
	bool ayuda = false;
	std::string PathAyuda = "Recursos/ayuda2.bmp";
	SDL_Surface *Ayuda = SDL_LoadBMP(PathAyuda.c_str());
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
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

	SDL_Rect rectMento;
	rectMento.x = 355;
	rectMento.y = 51;
	rectMento.w = 9;
	rectMento.h = 9;
	
	SDL_Rect rectMand;
	rectMand.x = 212;
	rectMand.y = 51;
	rectMand.w = 9;
	rectMand.h = 9;

	//----BISECTRIZ--------
	float dAC = 0;
	float dBA = 0;
	int X = 0;
	int Y = 0;
	int a1, a2, b1, b2, c1, c2;
	//----BISECTRIZ END----


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
					SDL_Window *winPregunta = SDL_CreateWindow("Nombre de la Imagen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_SHOWN);
					std::string PathPregunta = "Recursos/pregunta1.bmp";
					SDL_Surface *Pregunta = SDL_LoadBMP(PathPregunta.c_str());
					SDL_Renderer *renPregunta = SDL_CreateRenderer(winPregunta, -1, 1);
					SDL_Texture *texPregunta = SDL_CreateTextureFromSurface(renPregunta, Pregunta);
					SDL_RenderCopy(renPregunta, texPregunta, NULL, NULL);
					SDL_RenderPresent(renPregunta);

					imagenOn = true;

					SDL_Surface* Loading_Surf = SDL_LoadBMP("Ortopantomografia/OP1.bmp");
					if (Loading_Surf == nullptr) {
						cleanup(ren, win);
						std::cerr << "Error al cargar la Ortopantomografia. \n\tCodigo de Error: " << SDL_GetError() << std::endl;
						SDL_Quit();
						return 1;
					}
					Ortopantomografia = SDL_CreateTextureFromSurface(ren, Loading_Surf);
					SDL_FreeSurface(Loading_Surf);
					SDL_RenderCopy(ren, Ortopantomografia, &SrcR, &DestR);
				}

				//BOTON DE AYUDA
				if ((e.button.x > 748 && e.button.x < 775) & (e.button.y > 43 && e.button.y < 70)) {
					//Si hay ortopan que la cargue pero donde diga el usuario
					if (ayuda == false) {
						ayuda = true;
						SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, Ayuda);
						SDL_RenderCopy(ren, texPanel, NULL, NULL);
						SDL_RenderPresent(ren);
					}else {
						ayuda = false;
						if (imagenOn == false) {
							SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, Panel);
							SDL_RenderCopy(ren, texPanel, NULL, NULL);
							SDL_RenderPresent(ren);
						}
						else {
							SDL_Texture *texPanel = SDL_CreateTextureFromSurface(ren, Panel);
							SDL_RenderCopy(ren, texPanel, NULL, NULL);
							SDL_RenderPresent(ren);
							SDL_Surface* Loading_Surf = SDL_LoadBMP("Ortopantomografia/OP1.bmp");
							Ortopantomografia = SDL_CreateTextureFromSurface(ren, Loading_Surf);
							SDL_FreeSurface(Loading_Surf);
							SDL_RenderCopy(ren, Ortopantomografia, &SrcR, &DestR);
						}
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
				//BOTON Mentoniano		CRUCETA
				if ((imagenOn == true) & (e.button.x > 249 && e.button.x < 371) & (e.button.y > 46 && e.button.y < 67)) {
					cMand = false;
					SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
					SDL_RenderFillRect(ren, &rectMand);
					
					if (mentoniano == false) {
						mentoniano = true;
						SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
						SDL_RenderFillRect(ren, &rectMento);
						SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
					}
					else {
						SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
						SDL_RenderFillRect(ren, &rectMento);
						mentoniano = false;
					}
				}
				//BOTON C.Mandibular
				if ((imagenOn == true) & (e.button.x > 109 && e.button.x < 234) & (e.button.y > 46 && e.button.y < 67)) {
					mentoniano = false;
					SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
					SDL_RenderFillRect(ren, &rectMento);

					if (cMand == false) {
						cMand = true;
						SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
						SDL_RenderFillRect(ren, &rectMand);
						SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
					}else {
						SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
						SDL_RenderFillRect(ren, &rectMand);
						cMand = false;
					}
				}

				//Panel Para pintar lineas
				if (((cMand == true) || (mentoniano == true)) & (e.button.x > 10 && e.button.x < 790) & (e.button.y > 80 && e.button.y < 590)) {
					
					
					
					if (cMand == true){
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
						b1 = x1;
						b2 = y1;
						a1 = x2;
						a2 = y2;
						vecesPintar++;
						vecesMand++;
						if (vecesPintar == 3) {
							x2 = e.button.x;
							y2 = e.button.y;
							SDL_RenderDrawLine(ren, a1, a2, x2, y2);
							vecesPintar = 0;
						}
						c1 = x2;
						c2 = y2;


						float A[2] = { a1,a2 }, B[2] = { b1,b2 }, C[2] = { c1,c2 };


						dBA = sqrt(((a1 - b1) ^ 2) + ((a2 - b2) ^ 2));
						dAC = sqrt((c1 - a1) ^ 2 + (c2 - a2) ^ 2);
						float r = dAC / dBA;

						float  t[] = { X,Y };

						X = (b1 + r*c1) / (1 + r);
						Y = (b2 + r*c2) / (1 + r);
						std::cout << "(" << X << "," << Y << ")";


						SDL_RenderDrawLine(ren, a1, a2, X, Y);


						if (vecesMand == 4) {
							vecesMand = 0;
							cMand = false;
							SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
							SDL_RenderFillRect(ren, &rectMand);
						}
					}


					/*
					if (cMand == true){
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
						vecesMand++;
						if (vecesPintar == 2)
							vecesPintar = 0;
						if (vecesMand == 4) {
							vecesMand = 0;
							cMand = false;
							SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
							SDL_RenderFillRect(ren, &rectMand);
						}
					}*/
					if (mentoniano == true) {
						x1 = e.button.x;
						y1 = e.button.y;
						SDL_RenderDrawLine(ren, x1, y1 + 100, x1, y1 - 100);
						SDL_RenderDrawLine(ren, x1 + 100, y1, x1 - 100, y1);

					}
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