#ifndef H_GAME_CONTROLLER_H
#define H_GAME_CONTROLLER_H

#include "scene.h"

using namespace Scene;

namespace GameController {
	
	void updateWheel();
	void updateWheelMouseMotion();

	void updatePlayer();
	void deletePlayer();
	void resetPlayer();
	void spinPlayer();
	void drawPlayer();

	void showGift();
	void hideGift();
	void updateGiftMouseMove();
	void updateGiftMouseButton();
	void deleteGift();

	void PlayerMenu();
	void PlayerGift();

	void draw();
	void release();
}

void GameController::drawPlayer() {
	std::list<Player*>::iterator iter;
	iter = Scene::playerlist.begin();
	Player*temp;
	while(iter!= Scene::playerlist.end()) {
		temp = *iter;
		temp->draw(system12);
		iter++;
	}
	if((!isEndStage()) && (!Scene::quiz->isFinish()))
		Scene::g_player->showPlay(system12);
	if(specialGift != "" ) {
		if((Scene::g_player->getStatus() == Player::WIN_SPECIAL)) {
			system12->setScale(1.5f);
			string temp = "You Have Won " + specialGift; 
			system12->Print(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2,temp,D3DCOLOR_XRGB(200,33,54));
		}
	}
	if((Scene::g_player->getStatus() == Player::LOSED_SPECIAL)) {
		system12->Print(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2,"You have l ost",D3DCOLOR_XRGB(200,33,54));
		}
	if((timebar->getVisible()==true) && ((g_player->getStatus() == Player::FULL_SPECIAl) ||g_player->getStatus() == Player::READY_TO_ANSWER ||g_player->getStatus() == Player::READY_TO_FULL_ANSWER))
		timebar->draw();
}

void GameController::updatePlayer() {
	if(isEndStage() == true) {
		Scene::quiz->openAll();
		Scene::keyboard->setStatus(Keyboard::UNAVAILABLE);
		if((Next_Stage->getVisible()==false) && (Scene::g_player->getStatus() != Player::LOSED_SPECIAL)) {
			Sprite * next = new Sprite();
			next->loadImage("source/emotion/nextstage.png");
			next->setPosition(Next_Stage->getPosition().getX()-400,Next_Stage->getPosition().getY()+next->getHeight()/2);
			next->setObjectType(Scene::NEXT_STAGE);
			g_engine->addEntity(next);
			next= new Sprite();
			next->loadImage("source/emotion/Loseall.png");
			next->setPosition(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2-200);
			next->setObjectType(Scene::LOSEALL);
			next->setLifetime(4000);
			g_engine->addEntity(next);
		}
		Scene::Next_Stage->setVisible(true);
		Scene::Next_Stage->setCollidable(true);
		return;
	}
	std::list<Player*>::iterator iter;
	iter = Scene::playerlist.begin();
	while (iter != Scene::playerlist.end()) {
		Scene::g_player = *iter;
		if(Scene::g_player) {
			if(Scene::g_player->getID() == Player::getCurrentPlayer()) {
				int count = 0;
				while(Scene::g_player->getStatus() == Player::LOSED) {
					Player::setCurrentPlayer(Player::getCurrentPlayer()+1);
					++iter;
					if (iter == Scene::playerlist.end()) {
						Scene::g_player = *Scene::playerlist.begin();
				        Player::setCurrentPlayer(Scene::g_player->getID());
						iter = Scene::playerlist.begin();
			        }
			        else Scene::g_player = *iter;
					count++;
					if(count > Player::getNumPlayer())
						return;
				}
				if((Scene::g_player->getStatus()!= Player::LOSED) && (Scene::g_player->getStatus()!= Player::READY_TO_ANSWER) && (Scene::g_player->getStatus()!= Player::SPINNING)&& (Scene::g_player->getStatus()!= Player::READY_TO_FULL_ANSWER) && (Scene::g_player->getStatus() == Player::WIN_STAGE) && (Scene::g_player->getStatus() == Player::BEGIN_SPECIAL) && (Scene::g_player->getStatus() == Player::FULL_SPECIAl) && (Scene::g_player->getStatus() == Player::WIN_SPECIAL) && (Scene::g_player->getStatus() == Player::LOSED_SPECIAL))
					Scene::g_player->setStatus(Player::PLAYING);
				break;
			}
		}
		++iter;
	}
}

void GameController::deletePlayer() {
	if(playerlist.empty() == true)
		return;
	std::list<Player*>::iterator iter;
	iter = Scene::playerlist.begin();
	while (iter != Scene::playerlist.end()) {
		Scene::g_player = *iter;
		delete Scene::g_player;
		iter = Scene::playerlist.erase(iter);
	}
	playerlist.clear();
	Player::setNumPlayer(0);
}

void GameController::spinPlayer() {
	static Sprite* emo;
	Scene::wheel->setHolding(false);
	Scene::wheel_special->setHolding(false);
	if(sceneplay_start == true) {
		if(wheel->getStatus() == Wheel::STOP)
			return;
		timebar->setVisible(true);
		int toss = Scene::g_player->spin(wheel);
		if((toss == 700) || (toss == 800) ||(toss == 900)) {
			int sa = rand()%3;
			if(sa) {
				emo = new Sprite();
				emo->loadImage("source/emotion/bigToss.png");
				emo->setPosition(20,g_engine->getScreenHeight()/2-50);
				emo->setLifetime(1000);
				g_engine->addEntity(emo);
			}
		}
		if((toss == 200) || (toss == 100) || (toss == 300)) {
			int sa = rand()%3;
			if(sa) {
				emo = new Sprite();
				emo->loadImage("source/emotion/smallToss.png");
				emo->setPosition(20,g_engine->getScreenHeight()/2-50);
				emo->setLifetime(1500);
				g_engine->addEntity(emo);
			}
		}
		switch(toss) {
		case Wheel::G_BANKRUPT :
			Scene::g_player->setScore(0);
			Scene::g_player->end_play(Wheel::G_BANKRUPT);
			Scene::scenePlayerMenu_start = true;
			emo = new Sprite();
			emo->loadImage("source/emotion/BankRupt.png");
			emo->setPosition(wheel->getPosition().getX()+30,wheel->getPosition().getY()-200);
			emo->setLifetime(1000);
			g_engine->addEntity(emo);
			break;
		case Wheel::G_LOSEATURN :
			Scene::g_player->end_play(Wheel::G_LOSEATURN);
			Scene::scenePlayerMenu_start = true;
			emo = new Sprite();
			emo->loadImage("source/emotion/Loseturn.png");
			emo->setPosition(wheel->getPosition().getX()+wheel->getWidth()-200,wheel->getPosition().getY()-120);
			emo->setLifetime(1000);
			g_engine->addEntity(emo);
			break;
		case Wheel::G_GIFT :
			Scene::scenePlayerGift_start = true;
			break;
		}
		return;
	}
	if(sceneSpecial_start == true) {
		if(wheel_special->getStatus() == Wheel::STOP)
			return;
		switch(Scene::g_player->spin(wheel_special)) {
		case WheelSpecial::G_1mil :
			specialGift = "$1.000.000 ";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			emo = new Sprite();
			emo->loadImage("source/emotion/guessaword.png");
			emo->setPosition(button_ready->getPosition());
			emo->setObjectType(Scene::GUESSAWORD);
			g_engine->addEntity(emo);
			break;
		case WheelSpecial::G_PHONE :
			specialGift = "an iPhone";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			emo = new Sprite();
			emo->loadImage("source/emotion/guessaword.png");
			emo->setPosition(button_ready->getPosition());
			emo->setObjectType(Scene::GUESSAWORD);
			g_engine->addEntity(emo);
			break;
		case WheelSpecial::G_TABLET :
			specialGift = "an iPad";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			emo = new Sprite();
			emo->loadImage("source/emotion/guessaword.png");
			emo->setPosition(button_ready->getPosition());
			emo->setObjectType(Scene::GUESSAWORD);
			g_engine->addEntity(emo);
			break;
		case WheelSpecial::G_TRIP :
			specialGift = "an Europe Trip";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			emo = new Sprite();
			emo->loadImage("source/emotion/guessaword.png");
			emo->setPosition(button_ready->getPosition());
			emo->setObjectType(Scene::GUESSAWORD);
			g_engine->addEntity(emo);
			break;
		case WheelSpecial::G_TV :
			specialGift = "a Tivi";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			emo = new Sprite();
			emo->loadImage("source/emotion/guessaword.png");
			emo->setPosition(button_ready->getPosition());
			emo->setObjectType(Scene::GUESSAWORD);
			g_engine->addEntity(emo);
			break;
		case WheelSpecial::G_CAR :
			specialGift = "a Car";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			emo = new Sprite();
			emo->loadImage("source/emotion/guessaword.png");
			emo->setPosition(button_ready->getPosition());
			emo->setObjectType(Scene::GUESSAWORD);
			g_engine->addEntity(emo);
			break;
		}
		return;
	}
}

void GameController::PlayerMenu() {
	if((Scene::scenePlayerMenu_on == true) || (Scene::isEndStage() == true) ||(Scene::quiz->isFinish() == true))
		return;
	if((scenePlayerMenu_start == true) && (scenePlayerMenu_on == false)) {
		Scene::scenePlayerMenu_on = true;
		Scene::PlayerMenu_Spin->setVisible(true);
		Scene::PlayerMenu_Spin->setCollidable(true);
		Scene::PlayeMenu_Guess->setVisible(true);
		Scene::PlayeMenu_Guess->setCollidable(true);
	}
}

void GameController::PlayerGift() {
	if((Scene::scenePlayerGift_on == true) || (Scene::isEndStage() == true) ||(Scene::quiz->isFinish() == true))
		return;
	if((Scene::scenePlayerGift_start == true) && (Scene::scenePlayerGift_on == false)) {
		Scene::scenePlayerGift_on = true;
		showGift();
	}
}

void GameController::showGift() {
	Scene::arrow->setVisible(false);
	Sprite* hehe = new Sprite();
	hehe->loadImage("source/emotion/chooseagift.png");
	hehe->setPosition(g_engine->getScreenWidth()/2 -400,g_engine->getScreenHeight()/2-20);
	hehe->setObjectType(EMOTION_GIFT);
	hehe->setLifetime(5000);
	g_engine->addEntity(hehe);
	list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->setVisible(true);
		gift->setCollidable(true);
		++iter;
	}
	if(Scene::button_ok->getStatus()==Button::BUTTON_PRESSED) {
		Scene::button_ok->reset();
		Scene::button_ok->setVisible(false);
		Scene::button_ok->setCollidable(false);
	}
}

void GameController::hideGift() {
	list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->setVisible(false);
		gift->setCollidable(false);
		++iter;
	}
	Scene::scenePlayerGift_start = false;
	Scene::scenePlayerGift_on = false;
	Scene::scenePlayerMenu_start = true;
	Scene::arrow->setVisible(true);
}

void GameController::deleteGift() {
	list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->setAlive(false);
		iter = Scene::giftlist.erase(iter);
	}
}

void GameController::updateGiftMouseButton() {
	list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		if(gift->isPosition()==true) {
			if(gift->getStatus() == Button::BUTTON_NORMAL) {
				Scene::g_player->setGift(gift->getLabel());	
				gift->pressed();
			}
		}	
		++iter;
	}
}

void GameController::updateGiftMouseMove() {
	list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->updateMouseMove(cursor);
		++iter;
	}
}

void GameController::updateWheel() {
	Scene::wheel->update();
	Scene::wheel_special->update();
}

void GameController::updateWheelMouseMotion() {
	Scene::wheel->updateDirection(cursor->getDeltaX(),cursor->getDeltaY(),cursor);
	Scene::wheel_special->updateDirection(cursor->getDeltaX(),cursor->getDeltaY(),cursor);
}

void GameController::draw() {
	if((Scene::sceneplay_start == true) || (Scene::sceneSpecial_start == true)) {
		GameController::drawPlayer();
		GameController::spinPlayer();
	}
	Scene::quiz->arrangeQuestion(system12);
	Scene::cursor->draw();
};

void GameController::release() {
	deletePlayer();
	deleteGift();
}

#endif
