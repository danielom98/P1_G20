#include "coinRace.h"
#include "Input.h"
#include <time.h>

//declaraci�n de variables.
bool shouldGameContinue;
int cointarget;
int currentCoinMap;
int currentCoinPlayer;
int oldCoin;
dificultad difficulty;
Map map;
CoinManager coinManager;
Player player;

int main() {
	//inicializamos una variable para el correcto funcionamiento del juego.
	oldCoin = 0;

	srand(time(nullptr));
	//bucle en el que funciona el juego.
		system("cls");
		std::cout << "/////////////////////////////////////////" << std::endl;
		std::cout << "                COIN RACE                " << std::endl;
		std::cout << "/////////////////////////////////////////" << std::endl << std::endl;
		std::cout << "Controls: Use WASD to move the player" << std::endl;
		std::cout << "if you want to exit, press ESC" << std::endl << std::endl;
		std::cout << "Choose a difficulty:" << std::endl
			<< "1 - EASY" << std::endl
			<< "2 - MEDIUM" << std::endl
			<< "3 - HARD" << std::endl;
		int input = 0;
		while (input != 1 && input != 2 && input != 3) {
			std::cin.clear();
			std::cin >> input;
		}
		difficulty = static_cast<dificultad>(input);

		map.inicio(difficulty);
		player.inicio(&map);
		coinManager.inicio(&map);

		Input::Key key{ Input::Key::ENTER };
		shouldGameContinue = true;
		clock_t start_time = clock();
		cointarget = 30 + rand() % (static_cast<int>(difficulty) * 30);//c�lculo del n�mero objetivo de monedas a recolectar.
		do {
			if (Input::Key::NONE != key && Input::Key::ESC != key) {
				currentCoinMap = coinManager.ActualCoin;
				currentCoinPlayer = player.score- oldCoin;
				player.movimiento(key);//esto detecta el movimiento del jugador y act�a en consecuencia.
				system("cls");
				map.dibujar();//esto dibuja el mapa despu�s de cada movimiento del jugador.
				std::cout << "Score: " << player.score << "/" << cointarget << std::endl;
				//Con esta condici�n comprobamos si el jugador ha conseguido todas las monedas
				//del mapa. En ese caso, el mapa se rellenar� de nuevo de monedas. Sin embargo,
				//el jugador deber� moverse dos posiciones una vez haya cogido todas las monedas
				//para que se active el respawn.
				if (currentCoinMap - currentCoinPlayer == 0) {
					coinManager.inicio(&map);
					oldCoin = player.score;
				}
			}
			else if (key == Input::Key::ESC) {
				shouldGameContinue = false;
			}
			key = Input::getKey();
		} while (shouldGameContinue && player.score != cointarget);
		system("cls");
		//Finalmente, cuando el jugador haya recolectado todas las monedas, se le mostrar� el
		//tiempo que ha tardado en conseguirlo.
		if (player.score == cointarget) {
			clock_t end_time = clock();
			std::cout << std::endl << "You've picked up ";
			std::cout << cointarget;
			std::cout << " coins in ";
			std::cout << (end_time - start_time) / 1000;
			std::cout << " seconds" << std::endl;
			_getch();
		}
		//Con esto, le permite al jugador observar detenidamente el tiempo que ha tardado 
		//en recolectar las monedas antes de que se cierre el juego.
		std::cout << "Press ESC to exit";
		Input::Key finalkey = Input::Key::NONE;
		while (finalkey == Input::Key::NONE) {
			switch (Input::getKey()) {
			case Input::Key::ESC:
				return 0;
			}
		}
	return 0;
}
//Hay una peque�a probabilidad de que al coger todas las monedas del mapa, �stas no hagan el respawn.