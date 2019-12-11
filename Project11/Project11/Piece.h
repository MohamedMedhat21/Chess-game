#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;


class Piece
{
public:
	char type;
	char team;
	Vector2f cord;
	int num;
	bool firstMove;
	std::vector<std::string>posMov;
	int state;
	Piece();
	
	~Piece();
};

