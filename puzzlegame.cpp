#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
using namespace bangtal;

#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <time.h>

ScenePtr play;
ObjectPtr game_board[16], game_original[16];
ObjectPtr startButton;

TimerPtr playingt;
time_t start_time = 0, end_time = 0;
float playing_time = 0;						//소요시간 계산을 위한 변수 및 타이머 선언

TimerPtr timer;
float animationTime = 0.05f;
int mixCount = 50;			

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
	mixCount = 50;		
	timer->set(animationTime);
	timer->start();

	blank = 15;
	game_board[blank]->hide();
	startButton->hide();

	time(&start_time);		//시작 시간 체크
}

bool check_end() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != game_original[i]) return false;
	}
	return true;
}

void end_game() {
	game_board[blank]->show();
	startButton->setImage("Images/재시작버튼.png");
	startButton->locate(play, 900, 550);
	startButton->show();

	playing_time = difftime(end_time, start_time);		//종료 시간 - 시작 시간
	playingt = Timer::create(playing_time);
	showTimer(playingt);
	showMessage("퍼즐 완성! 소요시간은 위와 같습니다.");
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
					time(&end_time);		//종료 시간 체크
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