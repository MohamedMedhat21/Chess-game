#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;


class Piece
{
public:
	char type;
	char team;
	int num;
	bool firstMove;
	std::vector<std::string>posMov;
	Sprite pic;
	int state;
	Piece();
	
	~Piece();
};

