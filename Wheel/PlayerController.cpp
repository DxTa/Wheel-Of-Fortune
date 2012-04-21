#include "PlayerController.h" 

PlayerController::PlayerController() {
	playerlist.clear();
	bool scenePlayerMenu_start = false;
	bool scenePlayerMenu_on = false;
	bool scenePlayerGift_start = false;
	bool scenePlayerGift_on = false;
}

void PlayerController::newPlayer() {
	Player* player = new Player();
	player->setID(Player::getNumPlayer());
	playerlist.push_back(player);
}

void PlayerController::newPlayer(string name,double x,double y) {
	Player* player = new Player(name);
	player->setID(Player::getNumPlayer());
	player->setScale(0.25);
	player->setRotation(g_engine->math->toRadians(-20));
	player->setPosition(x,y);
	playerlist.push_back(player);
}

void PlayerController::drawPlayer(Font* system12,Quiz* quiz) {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	Player*temp;
	while(iter!= playerlist.end()) {
		temp = *iter;
		temp->draw(system12);
		iter++;
	}
	if((!isEndStage()) && (!quiz->isFinish()))
		g_player->showPlay(system12);
	if((g_player->getStatus() == Player::WIN_SPECIAL)) {
		system12->setScale(1.5f);
		string temp = "You Have Won " + specialGift; 
		system12->Print(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2,temp,D3DCOLOR_XRGB(200,33,54));
	}
	if((g_player->getStatus() == Player::LOSED_SPECIAL)) {
		system12->Print(g_engine->getScreenWidth()/2-200,g_engine->getScreenHeight()/2,"You have lost",D3DCOLOR_XRGB(200,33,54));
	}
}

void PlayerController::updatePlayer() {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		g_player = *iter;
		if(g_player) {
			if(g_player->getID() == Player::getCurrentPlayer()) {
				int count = 0;
				while(g_player->getStatus() == Player::LOSED) {
					Player::setCurrentPlayer(Player::getCurrentPlayer()+1);
					++iter;
					if (iter == playerlist.end()) {
						g_player = *playerlist.begin();
						Player::setCurrentPlayer(g_player->getID());
						iter = playerlist.begin();
					}
					else g_player = *iter;
					count++;
					if(count > Player::getNumPlayer())
						return;
				}
				if((g_player->getStatus()!= Player::LOSED) && (g_player->getStatus()!= Player::READY_TO_ANSWER) && (g_player->getStatus()!= Player::SPINNING)&& (g_player->getStatus()!= Player::READY_TO_FULL_ANSWER) && (g_player->getStatus() == Player::WIN_STAGE) && (g_player->getStatus() == Player::BEGIN_SPECIAL) && (g_player->getStatus() == Player::FULL_SPECIAl))
					g_player->setStatus(Player::PLAYING);
				break;
			}
		}
		++iter;
	}
}

void PlayerController::deletePlayer() {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		g_player = *iter;
		delete g_player;
		iter = playerlist.erase(iter);
	}
}

bool PlayerController::isEndStage() {
	Player* player;
	bool result = true;
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		player = *iter;
		if(player) { 
			if(player->getStatus()!= Player::LOSED) {
				result = false;
				break;
			}
		}
		++iter;
	}
	return result;
}