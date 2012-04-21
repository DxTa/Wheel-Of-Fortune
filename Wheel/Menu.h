#ifndef H_MENU_H
#define H_MENU_H

#include "Engine/Advanced2D.h"
#include "button.h"
#include "Cursor.h"

using namespace std;
class Menu {
private:
	int ID;
	int status;
	int buttonType;
	string name;
	std::list<Button*> menu;

public:
	enum status {OFF,ON};
	enum buttonType{NORMAL,TOGGLE};
	Menu();
	Menu(string ...);
	~Menu();

	string getName() const {return name;}
	void setName(string pname) {name = pname;}
	int getStatus() const {return status;}
	void setStatus(int pstatus) {status =pstatus;}
	int getButtonType() const {return buttonType;}
	void setButtonType(int bt) {buttonType = bt;}

	void addButton(string );
	void addButton(Button*);
	void setPosition(double,double);
	Button* getButton(string );

	void update();
	void reset();
	void updateMouseButton();
	void updateMouseMove(Cursor*);
	void close();
	void addCallback(string,void(*function)());
};

#endif