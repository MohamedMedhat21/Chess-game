#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include<iostream>
#include <vector>
#include "Piece.h"
#include "Connector.hpp"

using namespace sf;

Vector2f offset(28, 28);
Piece p[32];
int size = 56;
bool undo = 0;
int n = 0;
char turn = 'W';
std::string position = "";
std::vector<std::string>moves;
std::vector<std::string>movesFull;
sf::RectangleShape rectangle(sf::Vector2f(56, 56)), rectangle2(sf::Vector2f(56, 56));
sf::SoundBuffer movee,eat;
sf::Sound sound,sound2;
std::vector<Texture> texture(32);
std::vector<Sprite> sprites(32);


int board[8][8] =
{-1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1 };

int dx[8] = { 2 , 2 , 1 , -1, -2, -2 , 1, -1};
int dy[8] = {-1 , 1 , 2 ,  2,  1, -1, -2, -2};

bool isKingOk(Piece obj, Vector2f newPos)
{
	int DX[] = { 1,-1,1,-1,1,-1,0,0 };
	int DY[] = { 1,1,-1,-1,0, 0,1,-1 };

	for (int i = 0; i < 8; i++)
	{
		Vector2f cur = newPos;
		bool flag = 0;
		while (cur.x + (size*DX[i]) + 28 < 476 && cur.y + (DY[i] * size) + 28 < 476 && cur.y + (DY[i] * size) + 28 > 14 && cur.x + (size*DX[i]) + 28 > 14)
		{
			std::cout << cur.x << " " << cur.y << std::endl << std::endl;;
			cur.x += (DX[i] * size);
			cur.y += (DY[i] * size);

			for (int j = 0; j < 32; j++)if(p[j].num !=obj.num)
			{
				if (p[j].pic.getPosition() == cur)
				{
					if (obj.team != p[j].team)
					{
						std::cout << p[j].pic.getPosition().x << " " << p[j].pic.getPosition().y << " " << cur.x << " " << cur.y << std::endl;
						if (p[j].type == 'Q')
							return 0;
						else if (i < 4 && (p[j].type == 'B'))
							return 0;
						else if (i > 3 && (p[j].type == 'R'))
							return 0;
					}
					flag = 1;
					break;
				}
			}
			if (flag)
				break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		
		if (newPos.x + (size*dx[i]) + 28 < 476 && newPos.y + (dy[i] * size) + 28 < 476 && newPos.y + (dy[i] * size) + 28 > 14 && newPos.x + (size*dx[i]) + 28 > 14)
		{
			
			for (int j = 0; j < 32; j++)if (p[j].num != obj.num)
				if (p[j].pic.getPosition().x == newPos.x + (size*dx[i]) && p[j].pic.getPosition().y == newPos.y + (size*dy[i]) && p[j].team != obj.team)
				{
					if (p[j].type == 'K')
						return 0;
				}
		}
		
	}
}
void possibleMoves(Piece obj)
{
	if (obj.type == 'K')
	{
		int k = 0;
		int T = 24;
		for (int i = 0; i < 8; i++)
		{
			if (obj.pic.getPosition().x + (size*dx[i]) + 28 < 476 && obj.pic.getPosition().y + (dy[i] * size) + 28 < 476 && obj.pic.getPosition().y + (dy[i] * size) + 28 > 14 && obj.pic.getPosition().x + (size*dx[i]) + 28 > 14)
			{
			sprites[k].setPosition(obj.pic.getPosition().x+(size*dx[i])+28,obj.pic.getPosition().y+(dy[i]*size)+28);
			for(int j =0; j<32; j++)
				if (p[j].pic.getPosition().x == obj.pic.getPosition().x + (size*dx[i]) && p[j].pic.getPosition().y == obj.pic.getPosition().y + (size*dy[i]) && p[j].team != obj.team)
				{
					sprites[T].setPosition(obj.pic.getPosition().x + (size*dx[i]) + 28, obj.pic.getPosition().y + (dy[i] * size) + 28);
					T++;
				}
			}
			k++;
		}
	}
	if (obj.type == 'B')
	{
		int DX[4] = { 1,-1,1,-1 };
		int DY[4] = { -1, 1,1,-1 };
		int k = 0;
		int t = 24;
		for (int i = 0; i < 4; i++)
		{
					Vector2f cur = obj.pic.getPosition();
					while (cur.x + (size*DX[i]) + 28 < 476 && cur.y + (DY[i] * size) + 28 < 476 && cur.y + (DY[i] * size) + 28 > 14 && cur.x + (size*DX[i]) + 28 > 14)
					{
						bool flag = 0;
						cur.x += (DX[i] * size);
						cur.y += (DY[i] * size);
						sprites[k].setPosition(cur+offset);
						k++;
						for(int j=0; j<32; j++)
						if (p[j].pic.getPosition() == cur)
						{
							if (p[j].team != obj.team)
							{
							sprites[t].setPosition(cur + offset);
							t++;
							}
							flag = 1;
							break;
						}
						if (flag)
							break;
					}
		}
	}
	if (obj.type == 'R')
	{
		int DX[] = { 1,0,-1,0 };
		int DY[] = { 0,1,0,-1 };
		int k = 0;
		int t = 24;
			for (int i = 0; i < 4; i++)
			{
				Vector2f cur = obj.pic.getPosition();
				while (cur.x + (size*DX[i]) + 28 < 476 && cur.y + (DY[i] * size) + 28 < 476 && cur.y + (DY[i] * size) + 28 > 14 && cur.x + (size*DX[i]) + 28 > 14)
				{
					bool flag = 0;
						cur.x += (DX[i]*size);
						cur.y += (DY[i]*size);
						sprites[k].setPosition(cur + offset);
						k++;
					for(int j=0; j<32; j++)
					if (p[j].pic.getPosition() == cur)
					{
						if (p[j].team != obj.team)
						{
						sprites[t].setPosition(cur + offset);
						t++;
						}
						flag = 1;
						break;
					}
					if (flag)
						break;
				}

			}
		
	}
	if (obj.type == 'Q')
	{
		int DX[] = {1,1,1,-1,-1,-1,0,0};
		int DY[] = {1,0,-1,-1,1, 0,1,-1};
		int k = 0;
		int t = 24;
		for (int i = 0; i < 8; i++)
		{
			Vector2f cur = obj.pic.getPosition();
			bool flag = 0;
			while (cur.x + (size*DX[i]) + 28 < 476 && cur.y + (DY[i] * size) + 28 < 476 && cur.y + (DY[i] * size) + 28 > 14 && cur.x + (size*DX[i]) + 28 > 14)
			{
					cur.x += (DX[i]*size);
					cur.y += (DY[i]*size);
					sprites[k].setPosition(cur + offset);
						k++;
				for(int j=0; j<32; j++)
				if (p[j].pic.getPosition() == cur)
				{
					if (obj.team != p[j].team)
					{
						sprites[t].setPosition(cur + offset);
						t++;
					}
					flag = 1;
					break;
				}
				if (flag)
					break;
			}

		}
	}
	if (obj.type == '#')
	{
		int DX[] = { 1,1,1,-1,-1,-1,0,0 };
		int DY[] = { 1,0,-1,-1,1, 0,1,-1 };
		int k = 0;
		int t = 24;
		for (int i = 0; i < 8; i++)
		{
			Vector2f cur = obj.pic.getPosition();
			bool flag = 0;
			if (cur.x + (size*DX[i]) + 28 < 476 && cur.y + (DY[i] * size) + 28 < 476 && cur.y + (DY[i] * size) + 28 > 14 && cur.x + (size*DX[i]) + 28 > 14)
			{
				cur.x += (DX[i] * size);
				cur.y += (DY[i] * size);
				if (isKingOk(obj, cur))
				{
				sprites[k].setPosition(cur + offset);
				k++;
				}
				for (int j = 0; j < 32; j++)
					if (p[j].pic.getPosition() == cur)
					{
						if (obj.team != p[j].team)
						{
							sprites[t].setPosition(cur + offset);
							t++;
						}
						flag = 1;
						break;
					}
			}
		}
	}
	if (obj.type == 'P')
	{
		int DX[] = { 0 ,-1 , 1};
		int DY[] = { 1 ,1 ,  1};
		int team;
		if (obj.team == 'W')
			team = -1;
		else
			team = 1;
		int k = 0;
		int t = 24;
		int num=0;
		
			Vector2f cur = obj.pic.getPosition();
			while (cur.x + (size*DX[0]*team) + 28 < 476 && cur.y + (DY[0] * size*team) + 28 < 476 && cur.y + (DY[0] * size*team) + 28 > 14 && cur.x + (size*DX[0]*team) + 28 > 14)
			{
				num++;
				bool flag = 0;
				if(num < 2)
				for(int i=1; i<3; i++)
				for (int j = 0; j < 32; j++)
					if (p[j].pic.getPosition().x == cur.x + DX[i] * size *team && p[j].pic.getPosition().y == cur.y + DY[i] * size * team && p[j].team != obj.team)
					{
						sprites[t].setPosition(cur.x + DX[i] * size *team + 28 , cur.y + DY[i] * size * team + 28);
						t++;
					}
				cur.x += (DX[0] * size *team);
				cur.y += (DY[0] * size *team);
				sprites[k].setPosition(cur + offset);
				k++;
				for (int j = 0; j < 32; j++)
					if (p[j].pic.getPosition() == cur)
					{
						flag = 1;
						break;
					}
				if (flag)
					break;
				if (obj.firstMove)
				{
					if (num == 2)
						break;
				}
				else break;
			}
		
	}
}
	

bool checkForMoves(Vector2f newPos,Vector2f oldPos)
{
	
	if (p[n].type == 'P')
	{
		//pawn move for my <3
	}
	if (p[n].type == 'K')
	{
		for (int i = 0; i < 8; i++)
		{
			if (newPos.x == dx[i] * size + oldPos.x && newPos.y == dy[i] * size + oldPos.y)
				return 1;
		}
		return 0;
	}
	if (p[n].type == 'B')
	{
		int x, y;
			if (newPos.x > oldPos.x)
				x = 1;
			else
				x = -1;
			if (newPos.y < oldPos.y)
				y = -1;
			else
				y = 1;

		if (abs(newPos.x - oldPos.x) == abs(newPos.y - oldPos.y))
		{
			for (int i = 0; i < 32; i++)if(i != n)
			{
				Vector2f cur = oldPos;
				while (cur != newPos)
				{
					cur.x += (x*size);
					cur.y += (y*size);
					if (p[i].pic.getPosition() == cur && p[i].pic.getPosition() != newPos )
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
	if (p[n].type == 'R')
	{
		int x, y;
		if (newPos.x > oldPos.x)
			x = 1;
		else
			x = -1;
		if (newPos.y < oldPos.y)
			y = -1;
		else
			y = 1;

		if (newPos.x == oldPos.x || newPos.y == oldPos.y)
		{
			for (int i = 0; i < 32; i++)if (i != p[n].num)
			{
				Vector2f cur = oldPos;
				while (cur != newPos)
				{
					if(newPos.x != oldPos.x)
					cur.x += (x*size);
					else
					cur.y += (y*size);
					if (p[i].pic.getPosition() == cur && p[i].pic.getPosition() != newPos)
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
	if (p[n].type == 'Q')
	{
		int x, y;
		if (newPos.x > oldPos.x)
			x = 1;
		else
			x = -1;
		if (newPos.y < oldPos.y)
			y = -1;
		else
			y = 1;

		if (newPos.x == oldPos.x || newPos.y == oldPos.y || (abs(newPos.x - oldPos.x) == abs(newPos.y - oldPos.y)))
		{
			for (int i = 0; i < 32; i++)if (i != p[n].num)
			{
				Vector2f cur = oldPos;
				while (cur != newPos)
				{
					if (newPos.x == oldPos.x)
						cur.y += (y*size);
					else if(newPos.y == oldPos.y)
						cur.x += (x*size);
					else
					{
						cur.x += (x*size);
						cur.y += (y*size);
					}

					if (p[i].pic.getPosition() == cur && p[i].pic.getPosition() != newPos)
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
	if (p[n].type == '#')
	{
		if (((abs(newPos.x - oldPos.x) == 56 || abs(newPos.x - oldPos.x) == 0) && (abs(newPos.y - oldPos.y) == 56 || abs(newPos.y - oldPos.y) == 0)) && isKingOk(p[n],newPos))
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
			if (!n) continue;
			 (n > 0)? p[k].team = 'W' : p[k].team = 'B';
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			p[k].pic.setTextureRect(IntRect(size*x, size*y, size, size));
			p[k].pic.setPosition(size*j, size*i);
			p[k].num = k;
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
std::string chessNote(Vector2f p)
{
	std::string s = "";
	s += char(p.x / size + 97);
	s += char(7 - p.y / size + 49);

	return s;
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
		if (p[i].pic.getPosition() == newPos)
		{
			sound2.play();
			p[i].pic.setPosition(-100, -100);
			p[i].state = 0;
		}
	for (int i = 0; i < 32; i++)
		if (p[i].pic.getPosition() == oldPos)
		{	
			p[i].pic.setPosition(newPos);
		}
	//castling       //if the king didn't move
	if (str == "e1g1")  move("h1f1");
	if (str == "e8g8")  move("h8f8");
	if (str == "e1c1")  move("a1d1");
	if (str == "e8c8")  move("a8d8");
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
void Undo()
{
	undo = 1;
	if (position.length() >= 6)
		position.erase(position.length() - 6, 5);
	else
		position = "";
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

int main()
{
	RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");
	movee.loadFromFile("Audio/chessMove.wav");
	eat.loadFromFile("Audio/Eat.wav");
	sound.setBuffer(movee);
	sound2.setBuffer(eat);

	ConnectToEngine((char *)"C:/Users/EGYPT_LAPTOP/source/repos/Project11/Project11/stockfish_10_x64.exe");
	for(int i=0; i<32; i++)
	{
		if (i < 24)
			texture[i].loadFromFile("images/dot.png");
		else
			texture[i].loadFromFile("images/TargetPiece.png");
	}

	
	Texture t1, t2;
	t1.loadFromFile("images/figures.png");
	t2.loadFromFile("images/board.png");
	for (int i = 0; i < 32; i++) p[i].pic.setTexture(t1);
	Sprite sBoard(t2);

	rectangle.setPosition(-100, -100);
	rectangle2.setPosition(-100, -100);
	rectangle.setFillColor(sf::Color(184,198,96,150));
	rectangle2.setFillColor(sf::Color(184, 198, 96, 150));

	load();
	for (int i = 0; i < 32; i++)
	{
		sprites[i].setTexture(texture[i]);
		//if(i<24)
		sprites[i].setColor(sf::Color(0, 128, 0, 200));
		sprites[i].setPosition(-120, -120);
	}
	
	

	bool isMove = false;
	float dx = 0, dy = 0;
	Vector2f oldPos, newPos;
	std::string str;

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
						if (p[i].pic.getGlobalBounds().contains(pos.x, pos.y) && p[i].team == turn)
						{
							isMove = true; 
							n = i;
							possibleMoves(p[n]);
							
							dx = pos.x - p[i].pic.getPosition().x;
							dy = pos.y - p[i].pic.getPosition().y;
							oldPos = p[i].pic.getPosition();
						}

			if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Left)
				{
					for (int i = 0; i < 32; i++)
					{
						sprites[i].setTexture(texture[i]);
						sprites[i].setPosition(-100, -100);
					}
					if (isMove)
					{
						isMove = false;
						bool availbe = true;
						Vector2f po = p[n].pic.getPosition() + Vector2f(size / 2, size / 2);
						newPos = Vector2f(size*int(po.x / size), size*int(po.y / size));
						p[n].pic.setPosition(newPos);
						for (int i = 0; i < 32; i++)
							if(i!=n)
							{
								if (p[i].pic.getPosition() == newPos)
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
						{
							//std::cout << p[n].pic.getPosition().x << " " << p[n].pic.getPosition().y << std::endl;
							if (oldPos != newPos && checkForMoves(newPos,oldPos))
							{
								if (undo && moves.empty())
								{
									movesFull.clear();
									undo = 0;
								}
								str = chessNote(oldPos) + chessNote(newPos);
								position += str + " ";
								moves.push_back(str);
								movesFull.push_back(str);
								std::cout << str << std::endl;
								move(str);
							}
							else
								newPos = oldPos;
						}
						p[n].pic.setPosition(newPos);
						/*std::cout << newPos.x << " " << newPos.y << std::endl;*/
						

					}
				}

			//computer move
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Space)
				{
					str = getNextMove(position);

					oldPos = toCoord(str[0], str[1]);
					newPos = toCoord(str[2], str[3]);
				
					for (int i = 0; i < 32; i++)if (p[i].pic.getPosition() == oldPos)n = i;

					//animation//
					for (int i = 0; i < 300; i++)
					{
						Vector2f po = newPos - oldPos;
						p[n].pic.move(po.x / 300, po.y / 300);
						window.draw(sBoard);
						for (int i = 0; i < 32; i++) p[i].pic.move(offset);
						for (int i = 0; i < 32; i++) window.draw(p[i].pic); window.draw(p[n].pic);
						for (int i = 0; i < 32; i++) p[i].pic.move(-offset);
						window.display();
					}
					moves.push_back(str);
					movesFull.push_back(str);
					std::cout << str << std::endl;
					move(str);
					position += str + " ";
					p[n].pic.setPosition(newPos);
				}
		}

		if (isMove)
		{
		if(pos.x < 440 && pos.y < 440 && pos.x > 10 && pos.y > 10)
			p[n].pic.setPosition(pos.x - dx, pos.y - dy);
		}

		////// draw  ///////
		window.clear();
		window.draw(sBoard);
		window.draw(rectangle);
		window.draw(rectangle2);
		for (int i = 0; i < 32; i++) p[i].pic.move(offset);
		for (int i = 0; i < 32; i++)window.draw(sprites[i]);
		for (int i = 0; i < 32; i++) window.draw(p[i].pic); window.draw(p[n].pic);
		for (int i = 0; i < 32; i++) p[i].pic.move(-offset);
		window.display();
	}
	CloseConnection();
	
	return 0;
}