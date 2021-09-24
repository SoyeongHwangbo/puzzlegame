#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
using namespace bangtal;

#include <cstdlib>
#include <ctime>
#include <Windows.h>

ScenePtr play;
ObjectPtr game_board[16], game_original[16];
ObjectPtr startButton;

TimerPtr timer;
float animationTime = 0.05f;
int mixCount = 100;

int blank;

int game_index(ObjectPtr piece) {
	for (int i = 0; i < 16; i++)
		if (game_board[i] == piece) return i;
	return -1;
}

int index2x(int index) {
	return 143 + 230 * (index % 4);
}

int index2y(int index) {
	return 543 - 172 * (index / 4);
}

void game_move(int index) {
	auto piece = game_board[index];
	game_board[index] = game_board[blank];
	game_board[index]->locate(play, index2x(index), index2y(index));
	game_board[blank] = piece;
	game_board[blank]->locate(play, index2x(blank), index2y(blank));
	blank = index;
}

bool check_move(int index) {
	if (blank % 4 > 0 && index == blank - 1) return true;
	if (blank % 4 < 3 && index == blank + 1) return true;
	if (blank / 4 > 0 && index == blank - 4) return true;
	if (blank / 4 < 3 && index == blank + 4) return true;

	return false;
}

int random_move() {
	int index = rand() % 16;
	while (!check_move(index)) {
		index = rand() % 16;
	}
	return index;
}

void start_game() {
	mixCount = 100;
	timer->set(animationTime);
	timer->start();

	blank = 15;
	game_board[blank]->hide();

	startButton->hide();
}

bool check_end() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != game_original[i]) return false;
	}
	return true;
}

void end_game() {
	game_board[blank]->show();
	startButton->show();

	showMessage("완성!");
}


void init_game() {
	play = Scene::create("퍼즐맞추기", "Images/배경.png");

	char path[20];
	for (int i = 0; i < 16; i++) {
		sprintf(path, "Images/%d.png", i + 1);
		game_board[i] = Object::create(path, play, index2x(i), index2y(i));
		game_board[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
			int index = game_index(piece);
			if (check_move(index)) {
				game_move(index);
				
				if (check_end()) {
					end_game();
				}
			}
						
			return true;
			});
		game_original[i] = game_board[i];
	}

	startButton = Object::create("Images/시작버튼.png", play, 900, 550);
	startButton->setOnMouseCallback([&](auto , auto , auto , auto)->bool {
		start_game();
		return true;
		});

	timer = Timer::create(animationTime);
	timer->setOnTimerCallback([&](auto)->bool {
		game_move(random_move());
		
		mixCount--;
		if (mixCount > 0) {
			timer->set(animationTime);
			timer->start();
		}
		
		return true;
		});

	
	startGame(play);
}


int main()
{
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	
	init_game();

	return 0;
}