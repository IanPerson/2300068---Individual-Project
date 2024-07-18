#include "tile_Single.h"

//Default Constructor//
tile_Single::tile_Single() {};

//Constructor//
tile_Single::tile_Single(sf::Texture& texture, sf::Vector2f position) {
	//Set the texture of the tile//
	setTexture(&texture);
	//Set the postion of the tile//
	setPosition(position);
	//Set the size of the tile to 6.25 x larger than texture size to make tile size 100x100//
	setSize({ 16.f * 6.25f, 16.f * 6.25f });

};

//Deconstructor//
tile_Single::~tile_Single() {};

//Setting tileListPos//
void tile_Single::setListPos(int positionInList) {
	listPos = positionInList;

}

//Get the listPos//
int tile_Single::getListPos() {
	return listPos;
}