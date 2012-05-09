#pragma once


#include "GameController.h"
#include "ConfigController.h"

using namespace GameController;
using namespace ConfigController;

class Game {
private:
	const string gameTitle;
public :
	Game();
	~Game();
	static void init();
	static void update();
	static void updateScreen();
	static void updateMouseMotion();
	static void updateMouseButton();
	static void updateMouseMove(double delta_x,double delta_y,double fx,double fy);

	static void draw();
	static void release();
};

Game::Game() : gameTitle("Wheel Of Fortune") {
	Game::init();
}

Game::~Game() {
	Game::release();
}

void Game::init() {
	menu = new Menu();

	background_image = new Texture();

	background = new Sprite();
	background->setCollidable(false);
	g_engine->addEntity(background);

	cursor = new Cursor();
	cursor->loadImage("source/gameplay/Cursor_564.tga");
	cursor->setScale(0.5);
	cursor->setCollisionMethod(COLLISION_RECT);
	cursor->setObjectType(Scene::CURSOR);
	g_engine->addEntity(cursor);

	system12 = new Font();
	if (!system12->loadImage("source/data/font.tga")) {
		g_engine->message("Error loading font.tga");
		return;
	}

	system12->setColumns(16);
	system12->setCharSize(21,32);

	if (!system12->loadWidthData("source/data/font.dat")) {
		g_engine->message("Error loading font.dat");
		return;
	}

	wheel = new Wheel();
	wheel->loadImage("source/gameplay/wheel.tga");
	wheel->setPosition(0,500);
	wheel->setOR(wheel->getX() + wheel->getWidth()/2,wheel->getY() + wheel->getHeight()/2,(wheel->getHeight()/2));
	wheel->setObjectType(Wheel::WHEEL_POS);
	wheel->setVisible(false);
	wheel->setCollidable(false);
	g_engine->addEntity(wheel);

	wheel_special = new WheelSpecial();
	wheel_special->loadImage("source/gameplay/wheel_special.tga");
	wheel_special->setPosition(0,500);
	wheel_special->setOR(wheel_special->getX() + wheel_special->getWidth()/2,wheel_special->getY() + wheel_special->getHeight()/2,(wheel_special->getHeight()/2));
	wheel_special->setObjectType(Wheel::WHEEL_POS_SPECIAL);
	wheel_special->setVisible(false);
	wheel_special->setCollidable(false);
	g_engine->addEntity(wheel_special);

	arrow = new Sprite();
	arrow->loadImage("source/gameplay/arrow.tga");
	arrow->setPosition(222.0f,440);
	arrow->setVisible(false);
	arrow->setCollidable(false);
	g_engine->addEntity(arrow);

	keyboard = new Keyboard();
	keyboard->setPosition(550,550);
	keyboard->setVisible(false);
	keyboard->addEntity();

	quiz = new Quiz();
	quiz->setPosition(50,125);
	quiz->setSize(400,200);
	quiz->inputLog();

	PlayerMenu_Spin = new Button("spin_button");
	PlayerMenu_Spin->setCallback(spin);
	PlayerMenu_Spin->setCollidable(false);
	PlayerMenu_Spin->setVisible(false);
	PlayerMenu_Spin->setPosition(400,300);
	g_engine->addEntity(PlayerMenu_Spin);

	PlayeMenu_Guess = new Button("guess_button");
	PlayeMenu_Guess->setCallback(guess);
	PlayeMenu_Guess->setCollidable(false);
	PlayeMenu_Guess->setVisible(false);
	PlayeMenu_Guess->setPosition(600,300);
	g_engine->addEntity(PlayeMenu_Guess);

	Next_Stage = new Button("NextStage_button");
	Next_Stage->setCallback(Scene::nextStage);
	Next_Stage->setCollidable(false);
	Next_Stage->setVisible(false);
	Next_Stage->setPosition(g_engine->getScreenWidth()-Next_Stage->getWidth(),g_engine->getScreenHeight()/2-Next_Stage->getHeight()/2+100);
	g_engine->addEntity(Next_Stage);

	button_ok = new Button("ok_button");
	button_ok->setCallback(hideGift);
	button_ok->setCollidable(false);
	button_ok->setVisible(false);
	button_ok->setPosition(600-75,400);
	g_engine->addEntity(button_ok);

	button_ready = new Button("ready_button");
	button_ready->setCallback(readyspecial);
	button_ready->setCollidable(false);
	button_ready->setVisible(false);
	button_ready->setPosition(g_engine->getScreenWidth()/2-button_ready->getWidth()/2,g_engine->getScreenHeight()/2-button_ready->getHeight()/2);
	g_engine->addEntity(button_ready);

	back_button = new Button("back");
	back_button->setCollidable(false);
	back_button->setVisible(false);
	back_button->setPosition(0,700-back_button->getHeight());
	back_button->setCallback(back2main);
	g_engine->addEntity(back_button);

	pause_button = new Button("pause_button");
	pause_button->setCollidable(false);
	pause_button->setVisible(false);
	pause_button->setPosition(g_engine->getScreenWidth()-pause_button->getWidth()-20,10);
	pause_button->setCallback(stop2pause);
	g_engine->addEntity(pause_button);

	music_button = new Button("music_button");
	music_button->setCollidable(true);
	music_button->setVisible(true);
	music_button->setPosition(g_engine->getScreenWidth()-music_button->getWidth()-20,60);
	music_button->setCallback(switchMute);
	g_engine->addEntity(music_button);

	startGame = new Button("start_game");
	startGame->setCollidable(false);
	startGame->setVisible(false);
	startGame->setCallback(ConfigController::enableConfigure);
	startGame->setPosition(1200-startGame->getWidth(),700-startGame->getHeight());
	g_engine->addEntity(startGame);

	Scene::score_background = new Sprite();
	score_background->loadImage("source/gameplay/score_background.tga");
	score_background->setPosition(590,-40);
	score_background->setVisible(false);
	score_background->setCollidable(false);
	g_engine->addEntity(score_background);

	Scene::timebar = new Timebar();
	timebar->loadImage("source/gameplay/timebar.tga");
	timebar->setTotalFrames(60);
	timebar->setSize(489,10);
	timebar->setColumns(1);
	timebar->setCurrentFrame(59);
	timebar->setPosition(g_engine->getScreenWidth()/2-timebar->getWidth()/2,g_engine->getScreenHeight()/2+100);
	timebar->setVisible(false);


	g_engine->audio->Load("source/audio/bgm.mp3","bgm");
	g_engine->audio->Load("source/audio/mainmenu.mp3","main");
	g_engine->audio->Load("source/audio/clock.mp3","clock");
	g_engine->audio->Load("source/audio/open.mp3","open");
	g_engine->audio->Load("source/audio/guesswrong.mp3","guesswrong");
	g_engine->audio->Load("source/audio/specialwrong.mp3","specialwrong");
	g_engine->audio->Load("source/audio/answertrue.mp3","answertrue");
	g_engine->audio->Load("source/audio/guesstrue.mp3","guesstrue");
	g_engine->audio->Load("source/audio/answerwrong.mp3","answerwrong");
	g_engine->audio->Load("source/audio/bankrupt.mp3","bankrupt");

	Scene::scenePlayerMenu_start = true;
	Scene::checkNextStage = false;
	Scene::isMute = false;
}

void Game::update() {
	Scene::updateAudio();
	Scene::sceneplay();
	Scene::sceneSpecial();
	Scene::sceneNewGame();
	Scene::sceneMain();
	Scene::sceneHelp();
	Scene::scenePause();
	if((Scene::sceneplay_start == true) || (Scene::sceneSpecial_start == true)) {
		GameController::updatePlayer();
		GameController::PlayerMenu();
		GameController::PlayerGift();
		Scene::PlayerMenu_Spin->reset();
		Scene::PlayeMenu_Guess->reset();
		Scene::Next_Stage->reset();
		static int cad = 1;
		if(Scene::wheel->getStatus() == Wheel::WAIT) {
			if(Scene::arrorcheck.stopwatch(14)) {
				Scene::arrow->setScale(Scene::arrow->getScale() - 0.01*cad);
			}
			if(Scene::arrow->getScale() < 0.8)
				cad = -1;
			if(Scene::arrow->getScale() > 1)
				cad = 1;
			if(Scene::arrow->getColor() == D3DCOLOR_XRGB(255,255,255))
				Scene::arrow->setColor(D3DCOLOR_XRGB(244,32,43));
			else
				Scene::arrow->setColor( D3DCOLOR_XRGB(255,255,255));
		}
		else {
			Scene::arrow->setColor(D3DCOLOR_XRGB(255,255,255));
			Scene::arrow->setScale(1.0f);
		}
		if((Scene::quiz->isFinish() == true) && (Scene::isEndStage() == false) && ((Scene::sceneplay_start == true) || (Scene::sceneSpecial_start == true))) {
			if((Scene::Next_Stage->getVisible()==false)&& (Scene::g_player->getStatus() != Player::WIN_SPECIAL)) {
				Sprite * next = new Sprite();
				next->loadImage("source/emotion/nextstage.tga");
				next->setPosition(Scene::Next_Stage->getPosition().getX()-400,Scene::Next_Stage->getPosition().getY()+next->getHeight()/2);
				next->setObjectType(Scene::NEXT_STAGE);
				g_engine->addEntity(next);
			}
			Scene::Next_Stage->setVisible(true);
			Scene::Next_Stage->setCollidable(true);
			Scene::keyboard->setStatus(Keyboard::UNAVAILABLE);
		}
		if(Scene::scenePause_start == false) {
			if((Scene::timebar->getVisible() == true) &&(Scene::g_player->getStatus() == Player::FULL_SPECIAl)) { 
				if(timebar->specialTimeUp()) {
					Scene::g_player->setStatus(Player::LOSED_SPECIAL);
					Scene::g_player->answer(Scene::keyboard,Scene::quiz);
					if(openspecial == 0)  {
						g_engine->audio->Play("specialwrong");
						Sprite * temp = new Sprite();
						temp->loadImage("source/emotion/SoClose.tga");
						temp->setPosition(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2-200);
						temp->setObjectType(Scene::EMO_SPECIAL);
						g_engine->addEntity(temp);
						Next_Stage->setVisible(true);
						Next_Stage->setCollidable(true);
						openspecial = 1;
					}
				}
			}
			if((Scene::timebar->getVisible() == true) &&(Scene::g_player->getStatus() == Player::READY_TO_ANSWER)) { 
				if(Scene::timebar->guessTimeUp()) {
					Scene::timebar->setCurrentFrame(59);
					Scene::g_player->end_play();
					scenePlayerMenu_start = true;
				}
			}
			if((Scene::timebar->getVisible() == true) &&(Scene::g_player->getStatus() == Player::READY_TO_FULL_ANSWER)) { 
				if(Scene::timebar->answerTimeUp()) {
					Sprite * over = new Sprite();
					over->loadImage("source/emotion/overtime.tga");
					over->setObjectType(Scene::OVERTIME);
					over->setCollisionMethod(COLLISION_DIST);
					over->setPosition(g_engine->getScreenWidth()/2-75,75);
					over->setLifetime(3000);
					g_engine->addEntity(over);
					Scene::timebar->setCurrentFrame(59);
					Scene::g_player->setStatus(Player::LOSED);
					Scene::g_player->answer(Scene::keyboard,Scene::quiz);
					Scene::g_player->end_play(Player::LOSED);
					scenePlayerMenu_start = true;
				}
			}
			if(Scene::g_player->getStatus() == Player::WIN_SPECIAL) {
				Scene::scenePlayerMenu_start = false;
				Scene::g_player->setSpecialGift(specialGift);
				if(openspecial == 0) {
					Sprite * temp = new Sprite();
					temp->loadImage("source/emotion/winner.tga");
					temp->setPosition(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2-200);
					temp->setObjectType(Scene::EMO_SPECIAL);
					temp->setLifetime(5000);
					g_engine->addEntity(temp);
					openspecial = 1;
				}
			}
			if(Scene::g_player->getStatus() == Player::LOSED_SPECIAL) {
				Scene::scenePlayerMenu_start = false;
			}
		}
	}
	if(Scene::sceneMain_start == true) {
		Scene::pause_button->setVisible(false);
		Scene::pause_button->setCollidable(false);
	}
}

void Game::updateMouseMotion() {
	GameController::updateWheelMouseMotion();
}

void Game::updateMouseButton() {
	static Timer keyboardcheck;
	static Timer musiccheck;
	Scene::wheel->updateMouseButton();
	Scene::wheel_special->updateMouseButton();
	if(menucheck.stopwatch(96))
		Scene::menu->updateMouseButton();
	GameController::updateGiftMouseButton();
	if(buttoncheck.stopwatch(200)) {
		ConfigController::updateGiftControllerMouseButton();
		ConfigController::updatePlayerControllerMouseButton();
		if(Scene::startGame->isPosition() == true) {
			Scene::startGame->pressed();
			if((sceneplay_start == true))
				GameController::updatePlayer();
		}
		if(Scene::back_button->isPosition() == true)
			Scene::back_button->pressed();
		if(Scene::Next_Stage->isPosition() == true)
			Scene::Next_Stage->pressed();
		if(Scene::pause_button->isPosition() == true)
			Scene::pause_button->pressed();
		if(musiccheck.stopwatch(200))
			if(Scene::music_button->isPosition() == true)
				Scene::music_button->toggle();
	}
	if((sceneplay_start == true) || (sceneSpecial_start == true)) {
		if(Scene::PlayerMenu_Spin->isPosition() == true)
			Scene::PlayerMenu_Spin->pressed();
		if(Scene::PlayeMenu_Guess->isPosition() == true)
			Scene::PlayeMenu_Guess->pressed();
		if(Scene::button_ok->isPosition() == true)
			Scene::button_ok->pressed();
		if(Scene::button_ready->isPosition() == true)
			Scene::button_ready->pressed();
		if(keyboardcheck.stopwatch(200) && (scenePause_start != true)) {
			ss = Scene::g_player->answer(Scene::keyboard,Scene::quiz);
			if((ss!= "") && (Scene::g_player->getStatus() != Player::READY_TO_FULL_ANSWER) && (Scene::g_player->getStatus() != Player::FULL_SPECIAl) && (Scene::g_player->getStatus() != Player::BEGIN_SPECIAL)) {
				Scene::scenePlayerMenu_start = true;
				Scene::timebar->setCurrentFrame(59);
				openspecial = 0;
			}
		}
		if(Scene::g_player->getStatus() == Player::WIN_GAME) {
			Scene::scenePlayerMenu_start = false;
		}
	}
}

void Game::updateMouseMove(double delta_x,double delta_y,double fx,double fy) {
	Scene::wheel->updateMouseMove(delta_x,delta_y,fx,fy);
	Scene::wheel_special->updateMouseMove(delta_x,delta_y,fx,fy);
	Scene::menu->updateMouseMove(Scene::cursor);
	Scene::keyboard->updateMouseMove(Scene::cursor);
	GameController::updateGiftMouseMove();
	ConfigController::updateGiftControllerMouseMove();
	ConfigController::updatePlayerControllerMouseMove();
	Scene::PlayerMenu_Spin->updateMouseMove(Scene::cursor);
	Scene::PlayeMenu_Guess->updateMouseMove(Scene::cursor);
	Scene::button_ok->updateMouseMove(Scene::cursor);
	Scene::button_ready->updateMouseMove(Scene::cursor);
	Scene::Next_Stage->updateMouseMove(Scene::cursor);
	Scene::startGame->updateMouseMove(Scene::cursor);
	Scene::back_button->updateMouseMove(cursor);
	Scene::pause_button->updateMouseMove(cursor);
	Scene::music_button->updateMouseMove(cursor);
	if((Scene::sceneplay_start == true) || (Scene::sceneSpecial_start == true))
		if((Scene::g_player->getStatus() == Player::READY_TO_FULL_ANSWER) || (Scene::g_player->getStatus() == Player::FULL_SPECIAl))
			Scene::keyboard->reset();
}

void Game::updateScreen() {
	GameController::updateWheel();
	if(Scene::scenePause_start == true) {
		Scene::keyboard->setCollidable(false);
	}
	if((Scene::sceneplay_start == true) || (sceneSpecial_start == true)) {
		pause_button->setVisible(true);
		pause_button->setCollidable(true);		
	}
}

void Game::draw() {
	GameController::draw();
}

void Game::release() {
	GameController::release();
	ConfigController::release();
	Scene::release();
}


