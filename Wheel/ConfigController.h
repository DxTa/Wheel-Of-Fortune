#ifndef H_CONFIG_CONTROLLER_H
#define H_CONFIG_CONTROLLER_H

#include "Scene.h"
using namespace Scene;

namespace ConfigController {

	void newPlayer();
	void newPlayer(string name,double x,double y);

	void initConfigController();
	void updateGiftControllerMouseMove();
	void updateGiftControllerMouseButton();

	void updatePlayerControllerMouseMove();
	void updatePlayerControllerMouseButton();
	void clearGift();
	bool configGift();
	bool configPlayer();
	void deleteGiftConfig();
	void deletePlayerConfig();
	void newGift(string);
	void setGiftPosition(int,int);
	void enableConfigure();

	void release();

	enum {CHOSEPLAYER = 10000,START_GAME};
}

void ConfigController::initConfigController() {
	string label[] = {"bag","book","dress","glasses","globe","hat","ipod","lollipop","shirt","shoe"};
	giftconfig.clear();
	playerconfig.clear();
	for(int i = 0;i < 10;i++) {
		Button* temp = new Button(label[i]);
		temp->setPosition(50+(temp->getWidth()+10)*i,475);
		if(i > 5)
			temp->toggle();
		g_engine->addEntity(temp);
		giftconfig.push_back(temp);
	}
	numGift = 6;

	menu = new Menu("newGame","Time","10_button","20_button","30_button",NULL);
	menu->setPosition(170,120);
	menu->getButton("Time")->setCollidable(false);
	menu->getButton("Time")->setPosition(menu->getButton("Time")->getPosition().getX()-30,menu->getButton("Time")->getPosition().getY());
	menu->getButton("10_button")->pressed();
	menu->addCallback("10_button",timechose10);
	menu->addCallback("20_button",timechose20);
	menu->addCallback("30_button",timechose30);

	char name[8];
	for(int i = 1;i <= 4;i++) {
		sprintf(name,"player%d",i);
		Button* temp = new Button(name);
		temp->setPosition(470+ ((i-1)%2)*(temp->getWidth()+100),120+ ((i-1)/2)*(temp->getHeight()+75));
		if(i==4)
			temp->toggle();
		playerconfig.push_back(temp);
		g_engine->addEntity(temp);
	}
	numPlayer = 3;
}

void ConfigController::newGift(string name) {
	Gift* gift = new Gift(name);
	gift->setVisible(false);
	gift->setCollidable(false);
	gift->setCallback(showButtonOk);
	g_engine->addEntity(gift);
	Scene::giftlist.push_back(gift);
}

void ConfigController::clearGift() {
	list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->setAlive(false);
		iter = Scene::giftlist.erase(iter);
	}
	giftlist.clear();
}

void ConfigController::deleteGiftConfig() {
	if(giftconfig.empty() == true)
		return;
	list<Button*>::iterator iter;
	iter = Scene::giftconfig.begin();
	Button* gift;
	while(iter!=Scene::giftconfig.end()) {
		gift = *iter;
		gift->setAlive(false);
		iter = giftconfig.erase(iter);
	}
	giftconfig.clear();
}

void ConfigController::deletePlayerConfig() {
	if(playerconfig.empty() == true)
		return;
	list<Button*>::iterator iter;
	iter = Scene::playerconfig.begin();
	Button* player;
	while(iter!=Scene::playerconfig.end()) {
		player = *iter;
		player->setAlive(false);
		iter = playerconfig.erase(iter);
	}
	playerconfig.clear();
}

void ConfigController::setGiftPosition(int x,int y) {
	list<Gift*>::iterator iter;
	Gift* gift;
	iter = Scene::giftlist.begin();
	int i = 0;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->setPosition(x + (i%3)*gift->getWidth(),y + (int)(i/3)*gift->getHeight());
		++i;
		++iter;
	}
}

void ConfigController::updateGiftControllerMouseMove() {
	list<Button*>::iterator iter;
	iter = Scene::giftconfig.begin();
	Button* gift;
	while(iter!=Scene::giftconfig.end()) {
		gift = *iter;
		gift->updateMouseMove(cursor);
		++iter;
	}
}

void ConfigController::updateGiftControllerMouseButton() {
	list<Button*>::iterator iter;
	iter = Scene::giftconfig.begin();
	Button* gift;
	while(iter!=Scene::giftconfig.end()) {
		gift = *iter;
		if(gift->isPosition()==true) {
			gift->toggle();		
			if(gift->getStatus() == Button::BUTTON_PRESSED)
				Scene::numGift--;
			else
				Scene::numGift++;
		}
		++iter;
	}
}

void ConfigController::updatePlayerControllerMouseMove() {
	list<Button*>::iterator iter;
	iter = Scene::playerconfig.begin();
	Button* player;
	while(iter!=Scene::playerconfig.end()) {
		player = *iter;
		player->updateMouseMove(cursor);
		++iter;
	}
}

void ConfigController::updatePlayerControllerMouseButton() {
	list<Button*>::iterator iter;
	iter = Scene::playerconfig.begin();
	Button* player;
	while(iter!=Scene::playerconfig.end()) {
		player = *iter;
		if(player->isPosition()==true) {
			player->toggle();		
			if(player->getStatus() == Button::BUTTON_PRESSED)
				Scene::numPlayer--;
			else
				Scene::numPlayer++;
		}
		++iter;
	}
}

bool ConfigController::configGift() {
	clearGift();
	if(numGift != 6)
		return false;
	list<Button*>::iterator iter;
	iter = Scene::giftconfig.begin();
	Button* gift;
	while(iter!=Scene::giftconfig.end()) {
		gift = *iter;
		if(gift->getStatus() == Button::BUTTON_NORMAL) {
			newGift(gift->getLabel());
		}
		iter++;
	}
	setGiftPosition(button_ok->getPosition().getX()-100,button_ok->getPosition().getY()-200);
	return true;
}

bool ConfigController::configPlayer() {
	if(Scene::numPlayer <= 0)
		return false;
	list<Button*>::iterator iter;
	iter = Scene::playerconfig.begin();
	Button* player;
	int count = 0;
	while(iter!=Scene::playerconfig.end()) {
		player = *iter;
		if(player->getStatus() == Button::BUTTON_NORMAL) {
			newPlayer(player->getLabel(),740+110*count,70);
			count++;
		}
		iter++;
	}
	return true;
}

void ConfigController::newPlayer() {
	Player* player = new Player();
	player->setID(Player::getNumPlayer());
	Scene::playerlist.push_back(player);
}

void ConfigController::newPlayer(string name,double x,double y) {
	Player* player = new Player(name);
	player->setID(Player::getNumPlayer());
	player->setScale(0.25);
	player->setRotation(g_engine->math->toRadians(-20));
	player->setPosition(x,y);
	Scene::playerlist.push_back(player);
}

void ConfigController::enableConfigure() {
	if(configGift() && configPlayer()) {
		config2play();
		ConfigController::release();
	}	
	else {
		if(back->getVisible() == true)
			back->setVisible(false);
		else
			back->setVisible(true);
		startGame->reset();
	}
}

void ConfigController::release() {
	menu->close();
	deleteGiftConfig();
	deletePlayerConfig();
}

void Scene::sceneNewGame() {
	if(sceneNewGame_on == true)
		return;
	if((sceneNewGame_start == true) && (sceneNewGame_on == false)) {
		menu->close();
		ConfigController::initConfigController();
		startGame->setVisible(true);
		startGame->setCollidable(true);
		sceneNewGame_on = true;
	}
}

#endif