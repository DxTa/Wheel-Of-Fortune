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

bool holdCtrl = false;
Game* g_controller;

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
	g_controller= new Game();
	Scene::sceneMain_start = true;
	return true;
}

void game_update() 
{
	g_controller->update();
}

void game_render2d()
{	
	g_controller->draw();
}

void game_keyPress(int key) 
{ 
	holdCtrl = false;
	switch(key) {
	case DIK_LCONTROL :
	case DIK_RCONTROL :
		holdCtrl = true;
	}
}

void game_keyRelease(int key) 
{ 
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
		case DIK_M :
			if((sceneNewGame_start == true) && (holdCtrl == true)) {
				DWORD pid = Utils::FindProcess(L"input.exe");
				if(!pid)
					ShellExecute(GetDesktopWindow(), L"open",L"source\\data\\input.exe", NULL, NULL, SW_SHOWNORMAL);
			}
			break;
	}
}

void game_render3d()
{
    g_engine->ClearScene(D3DCOLOR_XRGB(0,0,80));
	g_engine->SetIdentity();
	g_controller->updateScreen();
}

void game_mouseButton(int button) {
	switch(button) {
	case 0 :	
			g_controller->updateMouseButton();
	}
}

void game_mouseMotion(int x,int y) {
	cursor->setDeltaX((float)x);
	cursor->setDeltaY((float)y);
	g_controller->updateMouseMotion();
}

void game_mouseMove(int x,int y) {
	double fx= (float)x;
	double fy = (float)y;
	g_controller->updateMouseMove(cursor->getDeltaX(),cursor->getDeltaY(),fx,fy);
	cursor->setPosition(fx,fy);
}

void game_mouseWheel(int wheel) {}

void game_entityRender(Advanced2D::Entity* entity) { }

void game_entityUpdate(Advanced2D::Entity* entity) {
	static double cad = 1;
	static double caq = 1;
	if((entity->getObjectType() == Letters::LETTER_TEMP)) {
		Letters *temp =(Letters*)entity;
		if((quiz->getClearTemp() == true) && (Scene::cleartemp == true))
			temp->setAlive(false);
		if(sceneMain_start == true)
			temp->setAlive(false);
	}
	if((entity->getObjectType() == Scene::EMOTION_GIFT) || (entity->getObjectType() == Scene::NEXT_STAGE) 
		||(entity->getObjectType() == Player::NEXT_PLAYER) || (entity->getObjectType() == Scene::GUESSAWORD) || (entity->getObjectType() == Scene::OVERTIME)
		||(entity->getObjectType() == Scene::NOTIFY_GIFT)) {
		Sprite *temp = (Sprite*)entity;
		if(emotioncheck.stopwatch(14)) {
			temp->setScale(temp->getScale() - 0.01*cad);
		}
		if(temp->getScale() < 0.85)
			cad = -1;
		if(temp->getScale() > 1)
			cad = 1;
		if(Scene::sceneMain_start == true)
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
	if(entity->getObjectType() == Player::FULLGUESSWRONG) {
		Sprite *temp = (Sprite*)entity;
		if((wheel->getStatus() == Wheel::WAIT) || (g_player->getStatus() == Player::READY_TO_FULL_ANSWER))
			temp->setAlive(false);
	}
	if(entity->getObjectType() == Player::FULLGUESSTRUE) {
		Sprite *temp = (Sprite*)entity;
		if(Scene::Next_Stage->getVisible()==false)
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
	if(entity->getObjectType() == Button::BUTTON) {
		Button* temp = (Button*)entity;
		if((temp->getLabel() == "play_button") || (temp->getLabel() == "help_button") || (temp->getLabel() == "exit_button") || (temp->getLabel() == "pause_button")) {
			if(temp->isPosition() != false) {
				temp->setRotation(temp->getRotation() - g_engine->math->toRadians(0.0005f)*caq);
				if(temp->getRotation() < g_engine->math->toRadians(-4))
					caq = -1;
				if(temp->getRotation() > g_engine->math->toRadians(4))
					caq = 1;
			}
			else
				temp->setRotation(0);
		}
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
	if(entity1->getObjectType() == Player::WTF && entity2->getObjectType() == Scene::EMO_SPECIAL) {
		Sprite* meme  = new Sprite();
		meme = (Sprite*)entity1;
		meme->setAlive(false);
	}
	if(entity1->getObjectType() == Player::NEXT_PLAYER && entity2->getObjectType() == Scene::LOSEALL) {
		Sprite* meme  = new Sprite();
		meme = (Sprite*)entity1;
		meme->setAlive(false);
	}
	if(entity1->getObjectType() == Scene::NOTIFY_GIFT && entity2->getObjectType() == Scene::NOTIFY_GIFT) {
		Sprite* meme  = new Sprite();
		meme = (Sprite*)entity1;
		meme->setAlive(false);
	}
}		

void game_end() 
{
	g_controller->release;
	g_engine->Close();
}

