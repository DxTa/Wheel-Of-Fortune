#ifndef H_GAME_CONTROLLER_H
#define H_GAME_CONTROLLER_H

#include "wheel.h"
#include "WheelSpecial.h"
#include "Keyboard.h"
#include "Quiz.h"
#include "Gift.h"
#include "Cursor.h"
#include "Player.h"

class PlayerController {
public :
	std::list<Player*> playerlist;
	string chose;
	string ss;
	string specialGift;
	bool scenePlayerMenu_start;
	bool scenePlayerMenu_on;
	bool scenePlayerGift_start;
	bool scenePlayerGift_on;
	Player* g_player;

	PlayerController();
	void newPlayer();
	void newPlayer(string name,double x,double y);
	void updatePlayer();
	void deletePlayer();
	void drawPlayer(Font*,Quiz*);

	bool isEndStage();
};

#endif