#pragma once
#include "Framework/GameObject.h"
#include <SFML/Graphics.hpp>

class tile_Single : public GameObject
{
public:
	//defualt consturctor//
	tile_Single();

	//consturctor//
	tile_Single(sf::Texture& texture, sf::Vector2f position);

	//deconsturctor//
	~tile_Single();

	//Setting the positon in the list of the tile//
	void setListPos(int positionInList);

	//Getting the position in the list of the tile//
	int getListPos();

private:
	int listPos;

};

