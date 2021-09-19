#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bangtal>
using namespace bangtal;

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	const int location[16][2] = { {143, 543}, {373, 543}, {603, 543}, {833, 543},
								{143, 715}, {373, 715}, {603, 715}, {833, 715},
								{143, 887}, {373, 887}, {603, 887}, {833, 887},
								{143, 1059}, {373, 1059}, {603, 1059}, {833, 1059} };
	
	auto ready = Scene::create("대기화면", "Images/원본.png");
	auto play = Scene::create("퍼즐맞추기", "Images/배경.png");
	auto startButton = Object::create("Images/시작버튼.png", ready, 900, 550);

	auto p2 = Object::create("Images/2.png", play);
	auto p3 = Object::create("Images/3.png", play);
	auto p4 = Object::create("Images/4.png", play);
	auto p5 = Object::create("Images/5.png", play);
	auto p6 = Object::create("Images/6.png", play);
	auto p7 = Object::create("Images/7.png", play);
	auto p8 = Object::create("Images/8.png", play);
	auto p9 = Object::create("Images/9.png", play);
	auto p10 = Object::create("Images/10.png", play);
	auto p11 = Object::create("Images/11.png", play);
	auto p12 = Object::create("Images/12.png", play);
	auto p13 = Object::create("Images/13.png", play);
	auto p14 = Object::create("Images/14.png", play);
	auto p15 = Object::create("Images/15.png", play);
	auto p16 = Object::create("Images/16.png", play);
	

	auto timer = Timer::create(12.f);

	startButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		startButton->hide();
		play->enter();
		timer->set(12.f);
		timer->start();

		

		
		return true;
		});

	showTimer(timer);
	timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("도전 실패");

		startButton->setImage("Images/재시작버튼.png");
		startButton->locate(play, 900, 550);
		startButton->show();

		return true;
		});

	startGame(ready);

	return 0;
}