#ifndef H_LETTER_H
#define H_LETTER_H

#include "Engine/Advanced2D.h"
using namespace Advanced2D;
using namespace std;

enum L_STATUS {OFF,ON};
class Letters : public Sprite {
private:
	int status;
	string label;
	static Texture *image_off;
	Texture *image_on;
public :
	
	Letters(string );
	~Letters();
	inline int getStatus() const {return status;}
	void setStatus(int pstatus) {status = pstatus;}
	string getLabel() const {return label;}
	static Texture* getImage_off() {return image_off;}
	
	void off(); //chưa lật
	void on(); //lật ô chữ
	void release();
};

#endif