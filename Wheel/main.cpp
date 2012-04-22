/*
  Name: Wheel of Fortune
  Date: 04/2012
  Description:Project OOP ICT 2012
*/

#include "Engine/Advanced2D.h"
#include "Keyboard.h"
#include "wheel.h"
#include "Menu.h"
#include "Quiz.h"
#include "Player.h"
#include "scene.h"
#include "game.h"
#include <math.h>


using namespace Advanced2D;
using namespace std;
using namespace Game;

bool game_preload() 
{
	g_engine->setAppTitle("WHEEL OF FORTUNE");
	g_engine->setScreenWidth(1200);
	g_engine->setScreenHeight(700);
	g_engine->setColorDepth(32);
	g_engine->setFullscreen(false);
	ShowCursor(FALSE);
	return true;
}

bool game_init(HWND) 
{	
	Game::init();
	//Scene::sceneNewGame_start = true;
	Scene::sceneMain_start = true;
	return true;
}

void game_update() 
{
	Game::update();
}

void game_render2d()
{	
	Game::draw();
}

void game_end() 
{
	Game::release();
	g_engine->Close();
}

void game_keyPress(int key) 
{ 
}

void game_keyRelease(int key) 
{ 
	Timer checkpress;
	switch (key) {
		case DIK_ESCAPE:
			if((sceneHelp_start == true) || (sceneNewGame_start == true))
				back2main();
			if((scenePause_start == false) && ((sceneplay_start == true) || (sceneSpecial_start == true))) {
				stop2pause();
				break;
			}
			if(scenePause_start == true)
				pause2resume();
			break;
		case DIK_SPACE:
			if(scenePause_start == true)
				pause2main();
			break;
	}
}

void game_render3d()
{
    g_engine->ClearScene(D3DCOLOR_XRGB(0,0,80));
	g_engine->SetIdentity();
	GameController::updateWheel();
}

void game_mouseButton(int button) {
	switch(button) {
	case 0 :	
			Game::updateMouseButton();
	}
}

void game_mouseMotion(int x,int y) {
	cursor->setDeltaX((float)x);
	cursor->setDeltaY((float)y);
	GameController::updateWheelMouseMotion();
}

void game_mouseMove(int x,int y) {
	double fx= (float)x;
	double fy = (float)y;
	Game::updateMouseMove(cursor->getDeltaX(),cursor->getDeltaY(),fx,fy);
	cursor->setPosition(fx,fy);
}

void game_mouseWheel(int wheel) {}

void game_entityRender(Advanced2D::Entity* entity) { }

void game_entityUpdate(Advanced2D::Entity* entity) {
	if((entity->getObjectType() == Letters::LETTER_TEMP)) {
		Letters *temp =(Letters*)entity;
		if((quiz->getClearTemp() == true) && (Scene::cleartemp == true))
			temp->setAlive(false);
		if(sceneMain_start == true)
			temp->setAlive(false);
	}
	if((entity->getObjectType() == Letters::OFF_TEMP)&& (quiz->getClearOff() == true)) {
		Letters *temp =(Letters*)entity;
		temp->setAlive(false);
		if(sceneMain_start == true)
			temp->setAlive(false);
	}
	if((entity->getObjectType() == Scene::EMOTION_GIFT) || (entity->getObjectType() == Scene::NEXT_STAGE) 
		||(entity->getObjectType() == Player::NEXT_PLAYER) || (entity->getObjectType() == Scene::GUESSAWORD) || (entity->getObjectType() == Scene::OVERTIME)
		||(entity->getObjectType() == Scene::NOTIFY_GIFT)) {
		static int cad = 1;
		Sprite *temp = (Sprite*)entity;
		if(emotioncheck.stopwatch(14)) {
			temp->setScale(temp->getScale() - 0.01*cad);
		}
		if(temp->getScale() < 0.85)
			cad = -1;
		if(temp->getScale() > 1)
			cad = 1;
		if(Scene::scenePause_start == true)
			temp->setAlive(false);
		if(entity->getObjectType() == Scene::EMOTION_GIFT) {
			if(button_ok->getVisible() == true)
				temp->setAlive(false);
		}
		if((entity->getObjectType() == Scene::NEXT_STAGE)) {
			if(Next_Stage->getVisible() == false)
				temp->setAlive(false);
		}
		if((entity->getObjectType() == Player::NEXT_PLAYER) || (entity->getObjectType() == Scene::OVERTIME)) {
			if((wheel->getStatus() == Wheel::WAIT) || (g_player->getStatus() == Player::READY_TO_FULL_ANSWER))
				temp->setAlive(false);
		}
		if(entity->getObjectType() == Scene::GUESSAWORD) {
			if((g_player->getStatus() == Player::FULL_SPECIAl))
				temp->setAlive(false);
		}
		if(entity->getObjectType() == Scene::NOTIFY_GIFT) {
			if(sceneNewGame_start != true)
				temp->setAlive(false);
		}
	}	
	if(entity->getObjectType() == Scene::EMO_SPECIAL) {
		Sprite *temp = (Sprite*)entity;
		if(sceneSpecial_start == false)
			temp->setAlive(false);
	}
	if(entity->getObjectType() == Scene::LOSEALL) {
		Sprite *temp = (Sprite*)entity;
		if(Next_Stage->getVisible() == false)
			temp->setAlive(false);
	}
	if(entity->getObjectType() == Player::WRONGFULLGUESS) {
		Sprite *temp = (Sprite*)entity;
		if((wheel->getStatus() == Wheel::WAIT) || (g_player->getStatus() == Player::READY_TO_FULL_ANSWER))
			temp->setAlive(false);
	}
	if(entity->getObjectType() == Scene::NEWGAME_TITTLE) {
		Sprite *temp = (Sprite*)entity;
		if(sceneNewGame_start != true)
			temp->setAlive(false);
	}
	if(entity->getObjectType() == Scene::PAUSE_LAYER) {
		Sprite *temp = (Sprite*)entity;
		if(scenePause_start != true)
			temp->setAlive(false);
	}
}

void game_entityCollision(Advanced2D::Entity* entity1,Advanced2D::Entity* entity2) {
	if(entity1->getObjectType() == CURSOR) {
		if(entity2->getObjectType() == Button::BUTTON) {
			Button* temp = (Button*)entity2;
			temp->setCheckPosition(true);
			temp->setScale(1.02);
		}
		if((entity2->getObjectType() == Wheel::WHEEL_POS)) {
			wheel->updateMousePosition(cursor);
		}
		if((entity2->getObjectType() == Wheel::WHEEL_POS_SPECIAL)) {
			wheel_special->updateMousePosition(cursor);
		}
	}
	if(entity1->getObjectType() == OVERTIME && entity2->getObjectType() == LOSEALL) {
		Sprite* meme  = new Sprite();
		meme = (Sprite*)entity1;
		meme->setAlive(false);
	}
}		
