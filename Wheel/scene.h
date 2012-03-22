#pragma once

#include "Menu.h"
#include "wheel.h"
#include "Keyboard.h"
#include "Quiz.h"
#include "Engine/Advanced2D.h"

ostringstream heee;

namespace Scene {
	std::list<Player*> playerlist;
	Player* g_player;
	Menu* menu;
	Sprite* background;
	Texture* background_image;
	Sprite* arrow;
	Wheel* wheel;
	Keyboard* keyboard;
	Quiz* quiz;
	Button* PlayerMenu_Spin;
	Button* PlayeMenu_Guess;
	bool scene1_start=false;
	bool scene2_start=false;
	bool scene3_start=false;
	bool scene4_start=false;
	bool sceneplay_start = false;
	bool scenePlayerMenu_start = false;
	bool scene1_on = false;
	bool scene2_on = false;
	bool scene3_on = false;
	bool scene4_on = false;
	bool sceneplay_on = false;
	bool scenePlayerMenu_on = false;

	void scene1();
	void scene2();
	void scene3();
	void scene4();
	void sceneplay();
	void scenePlayerMenu();

	void init();
	void update();
	void updateMouseButton();
	void updateMouseMove(double delta_x,double delta_y,double fx,double fy);
	void release();

	void newPlayer();
	void updatePlayer();
	void deletePlayer();

}

void g_exit() {
	g_engine->Close();
}

void spin() {
	Scene::wheel->setStatus(Wheel::WAIT);
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayeMenu_Guess->setCollidable(false);
}

void guess() {
	Scene::g_player->setStatus(Player::READY_TO_ANSWER);
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayeMenu_Guess->setCollidable(false);
}

void Scene::newPlayer() {
	Player* player = new Player();
	player->setID(Player::getNumPlayer());
	playerlist.push_back(player);
}

void Scene::updatePlayer() {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end())
	{
		g_player = *iter;
		if(g_player) {
			if(g_player->getID() == Player::getCurrentPlayer()) {
				if((g_player->getStatus()!= Player::LOSED) && (g_player->getStatus()!= Player::READY_TO_ANSWER) && (g_player->getStatus()!= Player::SPINNING))
					g_player->setStatus(Player::PLAYING);
					break;
			}
		}
		++iter;
	}
}

void Scene::deletePlayer() {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end())
	{
		g_player = *iter;
		delete g_player;
		iter = playerlist.erase(iter);
	}
}

void scene1_to_scene4() {
	Scene::scene1_start = false;
	Scene::scene1_on = false;
	Scene::scene4_start = true;
}

void scene4_to_play() {
	Scene::scene4_start = false;
	Scene::scene4_on = false;
	Scene::sceneplay_start = true;
}


void Scene::init() {
	menu = new Menu();
	background = new Sprite();
	background_image = new Texture();
	Scene::background->setCollidable(false);
	g_engine->addEntity(Scene::background);

	wheel = new Wheel();
	wheel->loadImage("wheel.png");
	wheel->setPosition(0,300);
	wheel->setOR(wheel->getX() + wheel->getWidth()/2,wheel->getY() + wheel->getHeight()/2,(wheel->getHeight()/2));
	wheel->setObjectType(Wheel::WHEEL_POS);
	wheel->setVisible(false);
	wheel->setCollidable(false);
	g_engine->addEntity(wheel);

	arrow = new Sprite();
	arrow->loadImage("arrow.png");
	arrow->setRotation(g_engine->math->toRadians(90));
	arrow->setPosition(220,230);
	arrow->setVisible(false);
	arrow->setCollidable(false);
	g_engine->addEntity(arrow);

	keyboard = new Keyboard();
	keyboard->setPosition(380,0);
	keyboard->addEntity();

	PlayerMenu_Spin = new Button("spin_button");
	PlayerMenu_Spin->setCallback(spin);
	PlayerMenu_Spin->setCollidable(false);
	PlayerMenu_Spin->setVisible(false);
	PlayerMenu_Spin->setPosition(400,150);
	g_engine->addEntity(PlayerMenu_Spin);

	PlayeMenu_Guess = new Button("guess_button");
	PlayeMenu_Guess->setCallback(guess);
	PlayeMenu_Guess->setCollidable(false);
	PlayeMenu_Guess->setVisible(false);
	PlayeMenu_Guess->setPosition(520,150);
	g_engine->addEntity(PlayeMenu_Guess);
	Scene::newPlayer();
	Scene::newPlayer();
	Scene::newPlayer();
	Player::setCurrentPlayer(1);

	quiz = new Quiz();
	quiz->setPosition(0,0);
	quiz->setWidth(380);
	quiz->setHeight(200);
	quiz->inputLog();
	quiz->inputQuiz(0,Player::getNumPlayer());
	quiz->change();
}

void Scene::scene2() {
	scene1_start = false;
	menu->close();
	menu = new Menu("Scene2","about_button","exit_button",NULL);
	menu->setPosition(100,100);
}

void Scene::scene3() {
	scene2_start = false;
	menu->close();
	menu = new Menu("Scene3","about_button","Highscore_button",NULL);
	menu->setPosition(100,100);
}

void Scene::scene4() {
	if(scene4_on == true)
		return;
	if((scene4_start == true) && (scene4_on == false)) {
		menu->close();
		background_image->Release();
		delete menu;
		background_image->Load("scene4.jpg");
		menu = new Menu("Scene4","play_button","Highscore_button","about_button","exit_button",NULL);
		menu->addFunction("exit_button",g_exit);
		menu->addFunction("play_button",scene4_to_play);
		menu->setPosition(200,100);
		scene4_on = true;
	}
}

void Scene::sceneplay() {
	if(sceneplay_on == true)
		return;
	if((sceneplay_start == true) && (sceneplay_on == false)) {
		menu->close();
		background_image->Release();
		background_image->Load("scene1.png");
		wheel->setVisible(true);
		wheel->setCollidable(true);
		arrow->setVisible(true);
		sceneplay_on = true;
	}
}

void Scene::scenePlayerMenu() {
	if(scenePlayerMenu_on == true)
		return;
	if((scenePlayerMenu_start == true) && (scenePlayerMenu_on == false)) {
		PlayerMenu_Spin->setVisible(true);
		PlayerMenu_Spin->setCollidable(true);
		PlayeMenu_Guess->setVisible(true);
		PlayeMenu_Guess->setCollidable(true);
		scenePlayerMenu_on = true;
	}
}

void Scene::update() {
	Scene::background->setImage(Scene::background_image);
	Scene::updatePlayer();
	scene4();
	sceneplay();
	scenePlayerMenu();
	Scene::menu->update();
	PlayerMenu_Spin->reset();
	PlayeMenu_Guess->reset();
	if(quiz->isFinish() == true) {
		//quiz->inputQuiz(0,Player::getNumPlayer());
		quiz->change();
		keyboard->reset();
	}
}

void Scene::updateMouseButton() {
	Scene::wheel->updateMouseButton();
	Scene::menu->updateMouseButton();
	if(PlayerMenu_Spin->isPosition() == true)
		PlayerMenu_Spin->pressed();
	if(PlayeMenu_Guess->isPosition() == true)
		PlayeMenu_Guess->pressed();
}

void Scene::updateMouseMove(double delta_x,double delta_y,double fx,double fy) {
	Scene::wheel->updateMouseMove(delta_x,delta_y,fx,fy);
	Scene::menu->updateMouseMove();
	Scene::keyboard->updateMouseMove();
	PlayerMenu_Spin->setCheckPosition(false);
	PlayeMenu_Guess->setCheckPosition(false);
}

void Scene::release() {
	deletePlayer();
	delete menu;
	delete background;
	delete background_image;
	delete arrow;
	delete wheel;
	delete keyboard;
	delete quiz;
	delete PlayerMenu_Spin;
	delete PlayeMenu_Guess;

}