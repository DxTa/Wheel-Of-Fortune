/*
  Name: Spinning Wheel Test
  Date: 03/2012
  Description:Spinning Wheel Test
*/

#include "Engine/Advanced2D.h"
#include "Keyboard.h"
#include "wheel.h"
#include "Menu.h"
#include "Quiz.h"
#include "Player.h"
#include "scene.h"
#include "Cursor.h"
#include <math.h>


using namespace Advanced2D;
using namespace std;
using namespace Scene;

Cursor* cursor;

Font* system12;

string chose;

enum {CURSOR=150};
bool game_preload() 
{
	g_engine->setAppTitle("TEST WHEEL");
	g_engine->setScreenWidth(1024);
	g_engine->setScreenHeight(513);
	g_engine->setColorDepth(32);
	g_engine->setFullscreen(false);
	ShowCursor(FALSE);
	return true;
}

bool game_init(HWND) 
{	
	cursor = new Cursor();
	cursor->loadImage("Cursor_564.png");
	cursor->setScale(0.5);
	cursor->setCollisionMethod(COLLISION_RECT);
	cursor->setObjectType(CURSOR);
	g_engine->addEntity(cursor);
	
	Scene::init();
	system12 = new Font();
	if (!system12->loadImage("system12.tga")) {
		g_engine->message("Error loading system12.tga");
		return false;
	}
	system12->setColumns(16);
	system12->setCharSize(14,16);

	if (!system12->loadWidthData("system12.dat")) {
		g_engine->message("Error loading system12.dat");
		return false;
	}

	Scene::sceneplay_start = true;
	return true;
}



void game_update() 
{
	Scene::update();
	
}

void game_render2d()
{	
	ostringstream ss;
	Scene::wheel->setHolding(false);
	g_player->spin(wheel);
	ss << quiz->isFinish();
	system12->Print(0,0,g_player->getName(),D3DCOLOR_XRGB(255,255,100));
	system12->Print(0,50,ss.str(),D3DCOLOR_XRGB(255,255,100));
	quiz->drawQuiz();
	cursor->draw();	
}

void game_end() 
{
	delete cursor;
	Scene::release();
}

void game_keyPress(int key) 
{ 
}

void game_keyRelease(int key) 
{ 
	switch (key) {
		case DIK_ESCAPE:
			g_engine->Close();
			break;
		case DIK_SPACE:
			Scene::scenePlayerMenu_start = true;
			break;
	}
}

void game_render3d()
{
    g_engine->ClearScene(D3DCOLOR_XRGB(0,0,80));
	g_engine->SetIdentity();
	Scene::wheel->update();
}

void game_mouseButton(int button) {
	switch(button) {
	case 0 :	
			Scene::updateMouseButton();
			g_player->answer(keyboard,quiz);
			break;
	}
}

void game_mouseMotion(int x,int y) {
	cursor->setDeltaX((float)x);
	cursor->setDeltaY((float)y);
	Scene::wheel->updateDirection(cursor->getDeltaX(),cursor->getDeltaY(),cursor);
}

void game_mouseMove(int x,int y) {
	double fx= (float)x;
	double fy = (float)y;
	Scene::updateMouseMove(cursor->getDeltaX(),cursor->getDeltaY(),fx,fy);
	cursor->setPosition(fx,fy);
}

void game_mouseWheel(int wheel) {}
void game_entityRender(Advanced2D::Entity* entity) { }
void game_entityUpdate(Advanced2D::Entity* entity) { }
void game_entityCollision(Advanced2D::Entity* entity1,Advanced2D::Entity* entity2) {
	if(entity1->getObjectType() == CURSOR) {
		if(entity2->getObjectType() == Button::BUTTON) {
			Button* temp = (Button*)entity2;
			temp->setCheckPosition(true);
		}
		if(entity2->getObjectType() == Wheel::WHEEL_POS) {
			wheel->updateMousePosition(cursor);
		}
	}
}		
