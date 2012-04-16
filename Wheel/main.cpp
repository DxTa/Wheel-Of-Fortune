/*
  Name: Spinning Wheel Test
  Date: 04/2012
  Description:Spinning Wheel Test
*/

#include "Engine/Advanced2D.h"
#include "Keyboard.h"
#include "wheel.h"
#include "Menu.h"
#include "Quiz.h"
#include "Player.h"
#include "scene.h"
#include <math.h>


using namespace Advanced2D;
using namespace std;
using namespace Scene;

bool game_preload() 
{
	g_engine->setAppTitle("TEST WHEEL");
	g_engine->setScreenWidth(1200);
	g_engine->setScreenHeight(700);
	g_engine->setColorDepth(32);
	g_engine->setFullscreen(false);
	ShowCursor(FALSE);
	return true;
}

bool game_init(HWND) 
{	
	Scene::init();
	Scene::sceneplay_start = true;
	return true;
}

void game_update() 
{
	Scene::update();
	if(g_player->getStatus() != Player::READY_TO_FULL_ANSWER)
		chose = "";
}

void game_render2d()
{	
	Scene::spinPlayer();
	system12->setRotation(0);
	system12->setScale(1.0f);
	Scene::score_background->draw();
	Scene::quiz->arrangeQuestion(system12);
	Scene::cursor->draw();
	Scene::drawPlayer();
	if((timebar->getVisible()==true) && ((g_player->getStatus() == Player::FULL_SPECIAl) ||g_player->getStatus() == Player::READY_TO_ANSWER ||g_player->getStatus() == Player::READY_TO_FULL_ANSWER))
		timebar->draw();
}

void game_end() 
{
	Scene::release();
}

void game_keyPress(int key) 
{ 
}

void game_keyRelease(int key) 
{ 
	Timer checkpress;
	switch (key) {
		case DIK_ESCAPE:
			g_engine->Close();
			break;
		case DIK_SPACE:
			break;
	}
}

void game_render3d()
{
    g_engine->ClearScene(D3DCOLOR_XRGB(0,0,80));
	g_engine->SetIdentity();
	Scene::wheel->update();
	Scene::wheel_special->update();
}

void game_mouseButton(int button) {
	switch(button) {
	case 0 :	
			Scene::updateMouseButton();
	}
}

void game_mouseMotion(int x,int y) {
	cursor->setDeltaX((float)x);
	cursor->setDeltaY((float)y);
	Scene::wheel->updateDirection(cursor->getDeltaX(),cursor->getDeltaY(),cursor);
	Scene::wheel_special->updateDirection(cursor->getDeltaX(),cursor->getDeltaY(),cursor);
}

void game_mouseMove(int x,int y) {
	double fx= (float)x;
	double fy = (float)y;
	Scene::updateMouseMove(cursor->getDeltaX(),cursor->getDeltaY(),fx,fy);
	cursor->setPosition(fx,fy);
}

void game_mouseWheel(int wheel) {}
void game_entityRender(Advanced2D::Entity* entity) { }
void game_entityUpdate(Advanced2D::Entity* entity) {
	if((entity->getObjectType() == Letters::LETTER_TEMP)&& (quiz->getClearTemp() == true) && (Scene::cleartemp == true)) {
		Letters *temp =(Letters*)entity;
		temp->setAlive(false);
	}
	if((entity->getObjectType() == Letters::OFF_TEMP)&& (quiz->getClearOff() == true)) {
		Letters *temp =(Letters*)entity;
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
}		
