#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include<iostream>
#include <vector>
#include "Piece.h"

using namespace sf;

Vector2f offset(78, 28);
Piece p[32];
int size = 56;
bool undo = 0;
int n = 0;
char turn = 'W';
Sprite f[32];
std::vector<std::string>moves;
std::vector<std::string>movesFull;
sf::RectangleShape rectangle(sf::Vector2f(56, 56)), rectangle2(sf::Vector2f(56, 56));
sf::SoundBuffer buffer,buffer2;
sf::Sound sound,sound2;


int board[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1 };

int dx[8] = { 2 , 2 , 1 , -1, -2, -2 , 1, -1};
int dy[8] = {-1 , 1 , 2 ,  2,  1, -1, -2, -2};

bool checkForMoves(Piece &obj,Vector2f newPos)
{
	if (obj.type == 'P')
	{
		
		if (obj.team == 'W')
		{
			if (obj.firstMove)
			{
				if (((newPos.y == obj.cord.y - 56 && newPos.x == obj.cord.x) || (newPos.y == obj.cord.y - 112 && newPos.x == obj.cord.x)))
				{
					for (int i = 0; i < 32; i++)
					{
						if ((p[i].cord.y == obj.cord.y - 56 && p[i].cord.x == obj.cord.x && newPos.x == obj.cord.x) || (p[i].cord.y == obj.cord.y - 112 && p[i].cord.x == obj.cord.x && newPos.y == obj.cord.y - 112 && newPos.x == obj.cord.x))
						{
							return 0;
						}
					}
					obj.firstMove = 0;
					return 1;
				}
				else if(newPos.x != obj.cord.x && newPos.y == obj.cord.y - 56)
				{
					for (int i = 0; i < 32; i++)
					{
						if (p[i].cord == newPos)
						{
							return 1;
						}
					}
					return 0;
				}
				else
					return 0;
			}
			else
			{
				if (newPos.y == obj.cord.y - 56 && newPos.x == obj.cord.x)
				{
					for (int i = 0; i < 32; i++)
					{
						if (p[i].cord.y == obj.cord.y - 56 && p[i].cord.x == obj.cord.x && newPos.x == obj.cord.x)
						{
							return 0;
						}
					}
					obj.firstMove = 0;
					return 1;
				}
				else if (newPos.x != obj.cord.x && newPos.y == obj.cord.y - 56)
				{
					for (int i = 0; i < 32; i++)
					{
						if (p[i].cord == newPos)
						{
							return 1;
						}
					}
					return 0;
				}
				else
					return 0;
			}
		}
		else
		{
			if (obj.firstMove)
			{
				if (((newPos.y == obj.cord.y + 56 && newPos.x == obj.cord.x) || (newPos.y == obj.cord.y + 112 && newPos.x == obj.cord.x)))
				{
					for (int i = 0; i < 32; i++)
					{
						if ((p[i].cord.y == obj.cord.y + 56 && p[i].cord.x == obj.cord.x && newPos.x == obj.cord.x) || (p[i].cord.y == obj.cord.y + 112 && p[i].cord.x == obj.cord.x && newPos.y == obj.cord.y + 112 && newPos.x == obj.cord.x))
						{
							std::cout << p[i].cord.x << " " << p[i].cord.y << "    " << newPos.x << " " << newPos.y << std::endl;
							return 0;
						}
					}
					obj.firstMove = 0;
					return 1;
				}
				else if (newPos.x != obj.cord.x && newPos.y == obj.cord.y + 56)
				{
					for (int i = 0; i < 32; i++)
					{
						if (p[i].cord == newPos)
						{
							return 1;
						}
					}
					return 0;
				}
				else
					return 0;
			}
			else
			{
				if (newPos.y == obj.cord.y + 56 && newPos.x == obj.cord.x)
				{
					for (int i = 0; i < 32; i++)
					{
						if (p[i].cord.y == obj.cord.y + 56 && p[i].cord.x == obj.cord.x && newPos.x == obj.cord.x)
						{
							std::cout << p[i].cord.x << " " << p[i].cord.y << "    " << newPos.x << " " << newPos.y << std::endl;
							return 0;
						}
					}
					obj.firstMove = 0;
					return 1;
				}
				else if (newPos.x != obj.cord.x && newPos.y == obj.cord.y + 56)
				{
					for (int i = 0; i < 32; i++)
					{
						if (p[i].cord == newPos)
						{
							return 1;
						}
					}
					return 0;
				}
				else
					return 0;
			}
		}
	}
	if (obj.type == 'K')
	{
		for (int i = 0; i < 8; i++)
		{
			if (newPos.x == dx[i] * size + obj.cord.x && newPos.y == dy[i] * size + obj.cord.y)
				return 1;
		}
		return 0;
	}
	if (obj.type == 'B')
	{
		int x, y;
			if (newPos.x > obj.cord.x)
				x = 1;
			else
				x = -1;
			if (newPos.y < obj.cord.y)
				y = -1;
			else
				y = 1;

		if (abs(newPos.x - obj.cord.x) == abs(newPos.y - obj.cord.y))
		{
			for (int i = 0; i < 32; i++)if(i != obj.num)
			{
				Vector2f cur = obj.cord;
				while (cur != newPos)
				{
					cur.x += (x*size);
					cur.y += (y*size);
					if (p[i].cord == cur && p[i].cord != newPos )
					{
						return 0;
					}
				}

			}
			return 1;
		}
		else
			return 0;
	}
	if (obj.type == 'R')
	{
		int x, y;
		if (newPos.x > obj.cord.x)
			x = 1;
		else
			x = -1;
		if (newPos.y < obj.cord.y)
			y = -1;
		else
			y = 1;

		if (newPos.x == obj.cord.x || newPos.y == obj.cord.y)
		{
			for (int i = 0; i < 32; i++)if (i != obj.num)
			{
				Vector2f cur = obj.cord;
				while (cur != newPos)
				{
					if(newPos.x != obj.cord.x)
					cur.x += (x*size);
					else
					cur.y += (y*size);
					if (p[i].cord == cur && p[i].cord != newPos)
					{
						return 0;
					}
				}

			}
			return 1;
		}
		else
			return 0;
	}
	if (obj.type == 'Q')
	{
		int x, y;
		if (newPos.x > obj.cord.x)
			x = 1;
		else
			x = -1;
		if (newPos.y < obj.cord.y)
			y = -1;
		else
			y = 1;

		if (newPos.x == obj.cord.x || newPos.y == obj.cord.y || (abs(newPos.x - obj.cord.x) == abs(newPos.y - obj.cord.y)))
		{
			for (int i = 0; i < 32; i++)if (i != obj.num)
			{
				Vector2f cur = obj.cord;
				while (cur != newPos)
				{
					if (newPos.x == obj.cord.x)
						cur.y += (y*size);
					else if(newPos.y == obj.cord.y)
						cur.x += (x*size);
					else
					{
						cur.x += (x*size);
						cur.y += (y*size);
					}

					if (p[i].cord == cur && p[i].cord != newPos)
					{
						return 0;
					}
				}

			}
			return 1;
		}
		else
			return 0;
	}
	if (obj.type == '#')
	{
		if ((abs(newPos.x - obj.cord.x) == 56 || abs(newPos.x - obj.cord.x) == 0) && (abs(newPos.y - obj.cord.y) == 56 || abs(newPos.y - obj.cord.y) == 0))
			return 1;
		return 0;
	}
}

void load()
{
	turn = 'W';
	int k = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int n = board[i][j];
			switch (abs(board[i][j]))
			{
				case 1: p[k].type = 'R'; break;
				case 2:	p[k].type = 'K'; break;
				case 3: p[k].type = 'B'; break;
				case 4:	p[k].type = 'Q'; break;
				case 5: p[k].type = '#'; break;
				case 6: p[k].type = 'P'; break;
			}
			 (n > 0)? p[k].team = 'W' : p[k].team = 'B';
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			f[k].setTextureRect(IntRect(size*x, size*y, size, size));
			f[k].setPosition(size*j, size*i);
			p[k].num = k;
			p[k].cord.x = size * j;
			p[k].cord.y = size * i;
			p[k].firstMove = 1;

			k++;
		}
}
	

Vector2f toCoord(char a, char b)
{
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;

	return Vector2f(x*size, y*size);
}
void move(std::string str)
{
	Vector2f oldPos = toCoord(str[0], str[1]);
	Vector2f newPos = toCoord(str[2], str[3]);

	sound.play();

	if (turn == 'W')
		turn = 'B';
	else
		turn = 'W';
			rectangle2.setPosition(newPos + offset);
			rectangle.setPosition(oldPos + offset);
	for (int i = 0; i < 32; i++)
		if (f[i].getPosition() == newPos)
		{
			sound2.play();
			f[i].setPosition(-100, -100);
			p[i].cord.x = -100;
			p[i].cord.y = -100;
			p[i].state = 0;
		}
	for (int i = 0; i < 32; i++)
		if (f[i].getPosition() == oldPos)
		{	
			p[i].cord = newPos;
			f[i].setPosition(newPos);
		}
	//castling       //if the king didn't move
	if (str == "e1g1")  move("h1f1");
	if (str == "e8g8")  move("h8f8");
	if (str == "e1c1")  move("a1d1");
	if (str == "e8c8")  move("a8d8");
}
std::string chessNote(Vector2f p)
{
	std::string s = "";
	s += char(p.x / size + 97);
	s += char(7 - p.y / size + 49);

	return s;
}
bool isKingThreatend(Piece obj)
{
	return 0;
}

void Undo()
{
	undo = 1;
	if (!moves.empty())
	{
		moves.pop_back();
		load();
		for (int i = 0; i < moves.size(); i++)
		{
			move(moves[i]);
		}
	}
	if (moves.empty())
	{
		rectangle.setPosition(-100, -100);
		rectangle2.setPosition(-100, -100);
	}
}
void Redo()
{
	if (movesFull.size() == moves.size())
		return;
	if (movesFull.size())
	{
		load();
		if (!moves.empty())
		{
			for (int i = 0; i < moves.size(); i++)
			{
				move(moves[i]);
			}
		}
		if (movesFull.size() > moves.size())
		{
			{
				move(movesFull[moves.size()]);
				moves.push_back(movesFull[moves.size()]);
			}
		}
	}
}
int main()
{
	RenderWindow window(VideoMode(604, 504), "The Chess! (press SPACE)");
	buffer.loadFromFile("Audio/chessMove.wav");
	buffer2.loadFromFile("Audio/Eat.wav");
	sound.setBuffer(buffer);
	sound2.setBuffer(buffer2);
	std::vector<Texture> texture(32);
	std::vector<Sprite> sprites(32);
	
	for(auto i : texture)
	{
		i.loadFromFile("images/dot.png");
	}
	for (int i = 0; i < 32; i++)
	{
		sprites[i].setTexture(texture[i]);
	}
	
	
	Texture t1, t2,t3;
	t1.loadFromFile("images/figures.png");
	t2.loadFromFile("images/board.png");
	t3.loadFromFile("images/dot.png");
	for (int i = 0; i < 32; i++) f[i].setTexture(t1);
	Sprite sBoard(t2),greenDot(t3);
	sBoard.move(50, 0);
	greenDot.setPosition(offset);
	rectangle.setPosition(-100, -100);
	rectangle2.setPosition(-100, -100);
	rectangle.setFillColor(sf::Color(184,198,96,150));
	rectangle2.setFillColor(sf::Color(184, 198, 96, 150));

	load();
	

	bool isMove = false;
	float dx = 0, dy = 0;
	Vector2f oldPos, newPos;
	std::string str;
	int n = 0;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			////move back//////
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Left)
					Undo();
				
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Right)
					Redo();
				
			/////drag and drop///////
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
					for (int i = 0; i < 32; i++)
						if (f[i].getGlobalBounds().contains(pos.x, pos.y) && p[i].team == turn)
						{
							isMove = true; n = i;
							
							dx = pos.x - f[i].getPosition().x;
							dy = pos.y - f[i].getPosition().y;
							oldPos = f[i].getPosition();
						}

			if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Left)
				{
					if (isMove)
					{
						isMove = false;
						bool availbe = true;
						Vector2f po = f[n].getPosition() + Vector2f(size / 2, size / 2);
						newPos = Vector2f(size*int(po.x / size), size*int(po.y / size));
						for (int i = 0; i < 32; i++)
						{
							if (p[i].cord == newPos)
							{
								if (p[i].team == p[n].team)
								{
									availbe = 0;
									newPos = oldPos;
									break;
								}
							}
						}
						if (availbe)
							if (oldPos != newPos && checkForMoves(p[n], newPos))
							{
								if (undo && moves.empty())
								{
									movesFull.clear();
									undo = 0;
								}
								str = chessNote(oldPos) + chessNote(newPos);
								moves.push_back(str);
								movesFull.push_back(str);
								std::cout << str << std::endl;
								move(str);
							}
							else
								newPos = oldPos;
						f[n].setPosition(newPos);
						p[n].cord = newPos;
					}
				}
		}

		if (isMove)
		{
		if(pos.x < 440 && pos.y < 440 && pos.x > 10 && pos.y > 10)
			f[n].setPosition(pos.x - dx, pos.y - dy);
		}

		////// draw  ///////
		window.clear();
		window.draw(sBoard);
		window.draw(rectangle);
		window.draw(rectangle2);
		window.draw(greenDot);
		for (int i = 0; i < 32; i++) f[i].move(offset);
		for (int i = 0; i < 32; i++) window.draw(f[i]); window.draw(f[n]);
		for (int i = 0; i < 32; i++) f[i].move(-offset);
		for (int i = 0; i < 32; i++)
		{
			window.draw(sprites[i]);
		}
		window.display();
	}
	
	return 0;
}