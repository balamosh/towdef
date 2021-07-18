#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#define WIDTH 2

struct road
{
	int len, dx, dy, fin, x, y;
};
struct monster
{
	int health, full_health, part, line, x, y, takt, ntakt, type, gold;
};
struct tower
{
	int hit, radius, state, cost, x, y, takt, ntakt, code, color;
};

int i, j, g, k, spawn, level, defence, money = 1000, cx = 0, cy = 3,
ntakt = 0, kills_left, kills = 0, mouse = 1, it[3], win = 0;

char defence_m[5], money_m[10], level_m[5], kills_m[5];

unsigned char far p[50][160];
monster monst[200];
tower tow[5][50];
road part[10][10][2 * WIDTH + 1];
unsigned char far*m[50];

void roads();
void change_len(int n);
void change_xy(int n);
void draw_road(int n);
void write_x(char *m, int x, int y, int color);
void draw_radius();

void main(void)
{
	clrscr();
	randomize();
	textmode(C4350);

	for(i = 0;i < 50;i++)
	{
		m[i] = (unsigned char far*)0xB8000000 + 160 * i;
	}

	roads();

	level = 0;
	for(i = 0; i < 4;i++)
	{
		change_len(i);
		change_xy(i);
	}
	for(i = 0;i < 3;i++)
	{
		it[i] = 0;
	}
	for(i = 0;i < 200;i++)
	{
		monst[i].health = 0;
		monst[i].ntakt = 0;
		monst[i].takt = 0;
		monst[i].full_health = 0;
		monst[i].type = 0;
		monst[i].gold = 0;
	}
	for(i = 0;i < 50;i++)
	{
		tow[2][i].radius = 5;
		tow[2][i].hit = 12;
		tow[2][i].state = 0;
		tow[2][i].cost = 100;
		tow[2][i].x = 0;
		tow[2][i].y = 0;
		tow[2][i].takt = 12;
		tow[2][i].ntakt = 0;
		tow[2][i].code = 30;
		tow[2][i].color = 2;

	}
	for(i = 0;i < 50;i++)
	{
		tow[1][i].radius = 3;
		tow[1][i].hit = 12;
		tow[1][i].state = 0;
		tow[1][i].cost = 150;
		tow[1][i].x = 0;
		tow[1][i].y = 0;
		tow[1][i].takt = 18;
		tow[1][i].ntakt = 0;
		tow[1][i].code = 127;
		tow[1][i].color = 4;
	}
	for(i = 0;i < 50;i++)
	{
		tow[0][i].radius = 4;
		tow[0][i].hit = 8;
		tow[0][i].state = 0;
		tow[0][i].cost = 200;
		tow[0][i].x = 0;
		tow[0][i].y = 0;
		tow[0][i].takt = 24;
		tow[0][i].ntakt = 0;
		tow[0][i].code = 157;
		tow[0][i].color = 1;
	}



	defence = 30;
	kills_left = 50;
	do
	{
		k = 0;

		if(kbhit())
		{
			k = getch();
		}

		for(i = 0;i < 50;i++)
		{
			for(j = 0;j < 80;j++)
			{
				p[i][2 * j] = ' ';
				p[i][2 * j + 1] = 0;
			}
		}

		draw_road(level);

		//attack of 'type_I' towers
		for(g = 0;g < 50;g++)
		{
			if(tow[0][g].ntakt == tow[0][g].takt)
			{
				tow[0][g].ntakt = 0;
				for(i = 0;i < 200;i++)
				{
					if(abs(monst[i].y - tow[0][g].y) <= tow[0][g].radius
					&& abs(monst[i].x - tow[0][g].x) <= tow[0][g].radius
					&& tow[0][g].state == 1 && monst[i].health > 0)
					{
						write_x("HIT!!!", 74, 49, 10);
						if(monst[i].health > tow[1][g].hit)
							monst[i].health -= tow[1][g].hit;
						else
						{
							monst[i].health = 0;
							money += monst[i].gold;
							kills_left --;
							kills ++;

						}
					}
				}
			}
			tow[0][g].ntakt ++;
		}

		//attack of 'type II' towers
		for(g = 0;g < 50;g++)
		{
			if(tow[1][g].ntakt == tow[1][g].takt)
			{
				tow[1][g].ntakt = 0;
				for(i = 0, k = 0;i < 200;i++)
				{
					if(abs(monst[i].y - tow[1][g].y) <= tow[1][g].radius
					&& abs(monst[i].x - tow[1][g].x) <= tow[1][g].radius
					&& tow[1][g].state == 1 && monst[i].health > 0)
					{
						write_x("HIT!!!", 74, 49, 10);
						if(monst[i].health > tow[1][g].hit)
							monst[i].health -= tow[1][g].hit;
						else
						{
							monst[i].health = 0;
							money += monst[i].gold;
							kills_left --;
							kills ++;
						}
						k++;
						if(k > 2)
							break;
					}
				}
			}
			tow[1][g].ntakt ++;
		}

		//attack of 'type III' towers
		for(g = 0;g < 50;g++)
		{
			if(tow[2][g].ntakt == tow[2][g].takt)
			{
				tow[2][g].ntakt = 0;
				for(i = 0;i < 200;i++)
				{
					if(abs(monst[i].y - tow[2][g].y) <= tow[2][g].radius
					&& abs(monst[i].x - tow[2][g].x) <= tow[2][g].radius
					&& tow[2][g].state == 1 && monst[i].health > 0)
					{
						write_x("HIT!!!", 74, 49, 10);
						if(monst[i].health > tow[2][g].hit)
							monst[i].health -= tow[2][g].hit;
						else
						{
							monst[i].health = 0;
							money += monst[i].gold;
							kills_left --;
							kills ++;
						}
						break;
					}
				}
			}
			tow[2][g].ntakt ++;
		}


		for(i = 0;i < 200;i++)
		{
			if(monst[i].ntakt == monst[i].takt)
			{
				monst[i].ntakt = 0;

				//move monster on other part
				if(monst[i].x
				== part[level][monst[i].part + 1][monst[i].line].x
				&& monst[i].y
				== part[level][monst[i].part + 1][monst[i].line].y
				&& monst[i].health > 0)
					monst[i].part++;

				//move monster
				if(monst[i].health > 0)
				{
					monst[i].x = monst[i].x
					+ part[level][monst[i].part][WIDTH].dx;
					monst[i].y = monst[i].y
					+ part[level][monst[i].part][WIDTH].dy;
				}
				//decrease the 'defence'
				if(p[monst[i].y][2 * monst[i].x] == 176
				&& p[monst[i].y][2 * monst[i].x + 1] == 10
				&& monst[i].health > 0)
				{
					monst[i].health = 0;
					if(defence - 1 >= 0)
						defence --;
					else
						break;
				}

			}
			//input monster on the screen
			if(monst[i].health > 0)
			{
				if(monst[i].type == 0)
					p[monst[i].y][2 * monst[i].x] = 1;
				if(monst[i].type == 1)
					p[monst[i].y][2 * monst[i].x] = 2;
				if(monst[i].type == 2)
					p[monst[i].y][2 * monst[i].x] = 15;
				if((100 * monst[i].health) / monst[i].full_health >= 75)
					p[monst[i].y][2 * monst[i].x + 1] = 10;
				else if((100 * monst[i].health) / monst[i].full_health >= 50)
					p[monst[i].y][2 * monst[i].x + 1] = 14;
				else if((100 * monst[i].health) / monst[i].full_health >= 25)
					p[monst[i].y][2 * monst[i].x + 1] = 4;
				else
					p[monst[i].y][2 * monst[i].x + 1] = 15;
			}
			monst[i].ntakt ++;
		}
		if(ntakt % 64 == 0)
		{
			//spawn a monster
			spawn = random(100);
			if(spawn < 50)
			{
				for(i = 0;i < 200;i++)
				{
					if(monst[i].health == 0)
					{
						monst[i].line = random(2 * WIDTH + 1);
						monst[i].part = 0;
						monst[i].ntakt = 0;
						monst[i].x = part[level][0][monst[i].line].x;
						monst[i].y = part[level][0][monst[i].line].y;
						p[monst[i].y][2 * monst[i].x + 1] = 10;

						spawn = random(100);
						if(spawn < 60)
						{
							monst[i].health = monst[i].full_health = 200;
							monst[i].takt = 16;
							p[monst[i].y][2 * monst[i].x] = 1;
							monst[i].type = 0;
							monst[i].gold = 50;
						}
						else
						{
							spawn = random(100);
							if(spawn < 50)
							{
								monst[i].health = monst[i].full_health = 300;
								monst[i].takt = 32;
								p[monst[i].y][2 * monst[i].x] = 2;
								monst[i].type = 1;
								monst[i].gold = 70;
							}
							else
							{
								monst[i].health = monst[i].full_health = 100;
								monst[i].takt = 8;
								p[monst[i].y][2 * monst[i].x] = 15;
								monst[i].type = 2;
								monst[i].gold = 35;
							}
						}
						break;
					}
				}
			}
		}
		//input towers on screen
		for(i = 0;i < 3;i++)
		{
			for(j = 0;j < 50;j++)
			{
				if(tow[i][j].state == 1)
				{
					p[tow[i][j].y][2 * tow[i][j].x]
					= tow[i][j].code;
					p[tow[i][j].y][2 * tow[i][j].x + 1]
					= tow[i][j].color;
				}
			}
		}

		switch(k)
		{
			case 72:
				if(cy <= 3)
					break;
				else
				{
					p[cy][2 * cx] = ' ';
					cy--;
				}
				break;
			case 80:
				if(cy >= 47)
					break;
				else
				{
					p[cy][2 * cx] = ' ';
					cy++;
				}
				break;
			case 75:
				if(cx == 0)
					break;
				else
				{
					p[cy][2 * cx] = ' ';
					cx--;
				}
				break;
			case 77:
				if(cx == 79)
					break;
				else
				{
					p[cy][2 * cx] = ' ';
					cx++;
				}
				break;
			case 32:
				if(mouse == 2)
					mouse = 1;
				else
					mouse = ! mouse;
				break;
			case '1':
				if(money - tow[0][0].cost >= 0)
				{
					if(mouse == 0 && p[cy][2 * cx] == ' ')
					{

						tow[0][it[0]].state = 1;
						tow[0][it[0]].x = cx;
						tow[0][it[0]].y = cy;

						p[tow[0][it[0]].y]
						[2 * tow[0][it[0]].x]
						= tow[0][it[0]].code;

						p[tow[0][it[0]].y]
						[2 * tow[0][it[0]].x + 1]
						= tow[0][it[0]].color;

						money -= tow[0][0].cost;
						it[0]++;
						mouse = 1;
					}
					else
						break;
				}
				else
					mouse = 2;
				break;
			case '2':
				if(money - tow[1][0].cost >= 0)
				{
					if(mouse == 0 && p[cy][2 * cx] == ' ')
					{

						tow[1][it[1]].state = 1;
						tow[1][it[1]].x = cx;
						tow[1][it[1]].y = cy;

						p[tow[1][it[1]].y]
						[2 * tow[1][it[1]].x]
						= tow[1][it[1]].code;

						p[tow[1][it[1]].y]
						[2 * tow[1][it[1]].x + 1]
						= tow[1][it[1]].color;

						money -= tow[1][0].cost;
						it[1]++;
						mouse = 1;
					}
					else
						break;
				}
				else
					mouse = 2;
				break;
			case '3':
				if(money - tow[2][0].cost >= 0)
				{
					if(mouse == 0 && p[cy][2 * cx] == ' ')
					{

						tow[2][it[2]].state = 1;
						tow[2][it[2]].x = cx;
						tow[2][it[2]].y = cy;

						p[tow[2][it[2]].y]
						[2 * tow[2][it[2]].x]
						= tow[2][it[2]].code;

						p[tow[2][it[2]].y]
						[2 * tow[2][it[2]].x + 1]
						= tow[2][it[2]].color;

						money -= tow[2][0].cost;
						it[2]++;
						mouse = 1;
					}
					else
						break;
				}
				else
					mouse = 2;
				break;
			case 'e':
				draw_radius();
				getch();
				break;

		}

		if(mouse != 0)
		{
			write_x("Press 'space' to chose tower", 0, 48, 15);
			write_x("Press 'e' to pause game and print radiuses", 0, 49, 15);
		}
		if(mouse == 0)
		{
			write_x("Eletric tower Exploding tower Shooting tower       Press 1, 2 or 3", 0, 48, 15);
			write_x("     $200          $150           $100                 to chose", 0, 49, 15);
		}

		if(mouse == 2)
			write_x("You don't have enough money (Press 'space')", 0, 49, 4);
		p[cy][2 * cx] = 219;
		p[cy][2 * cx + 1] = 4;



		itoa(defence, defence_m, 10);
		itoa(money, money_m, 10);
		itoa(level + 1, level_m, 10);
		itoa(kills_left, kills_m, 10);

		write_x("Monsters left to lose: ", 0, 0, 4);
		write_x(defence_m, 23, 0, 4);
		write_x("Monsters left to kill: ", 0, 1, 2);
		write_x(kills_m, 23, 1, 2);

		write_x("Money: $", 40, 0, 2);
		write_x(money_m, 48, 0, 2);
		write_x("Level: ", 40, 1, 1);
		write_x(level_m, 47, 1, 1);

		ntakt++;
		for(i = 0;i < 50;i++)
		{
			for(j = 0;j < 160;j++)
			{
				m[i][j] = p[i][j];
			}
		}

		if(kills_left == 0)
		{
			if(level == 3)
			{
				win = 1;
				break;
			}
			cx = 0;
			cy = 3;
			level ++;
			kills_left = 50;
			for(i = 0;i < 3;i++)
			{
				for(j = 0;j < 50;j++)
				{
					tow[i][j].state = 0;
				}
			}
			for(i = 0;i < 200;i++)
			{
				monst[i].health = 0;
			}
		}
		if(defence == 0)
		{
			win = 0;
			break;
		}
		delay(20);
	}while(k != 27);

	for(i = 0;i < 50;i++)
	{
		for(j = 0;j < 80;j++)
		{
			p[i][2 * j] = ' ';
			p[i][2 * j + 1] = 0;
		}
	}
	itoa(kills, kills_m, 10);
	itoa(money, money_m, 10);
	itoa(level + 1, level_m, 10);
	if(win == 0)
		write_x("YOU LOSE!", 35, 25, 10);
	if(win)
		write_x("YOU WIN!", 35, 25, 10);
	write_x("You have reached level ", 28, 26, 1);
	write_x(level_m, 51, 26, 1);
	write_x("You have earned $",29, 27, 10);
	write_x(money_m, 46, 27, 10);
	write_x("You have killed ", 29, 28, 4);
	write_x(kills_m, 45, 28, 4);

	for(i = 0;i < 50;i++)
	{
		for(j = 0;j < 160;j++)
		{
			m[i][j] = p[i][j];
		}
	}

	getch();
}

void draw_radius()
{
	for(i = 0;i < 50;i++)
	{
		for(j = 0;j < 3;j++)
		{
			if(tow[j][i].state)
			{
				for(g = 0;g < 50;g++)
				{
					for(k = 0;k < 80;k++)
					{
						if(abs(g - tow[j][i].y) <= tow[j][i].radius
						&& abs(k - tow[j][i].x) <= tow[j][i].radius)
						{
							m[g][2 * k + 1] = 64;
						}
					}
				}
			}
		}
	}
}

void write_x(char *c, int x, int y, int color)
{
	for(int i = 0;c[i] != 0;i++)
	{
		p[y][2 * (x + i)] = c[i];
		p[y][2 * (x + i) + 1] = color;
	}
}

void change_len(int n)
{
	int f = 0, a;
	for(int i = 0;;i++)
	{
		for(int j = -WIDTH;j <= WIDTH;j++)
		{
			if(i == 0)
			{
				if(part[n][i + 1][WIDTH].dx == 1
				|| part[n][i + 1][WIDTH].dy == 1)
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len - j;
				}
				if(part[n][i + 1][WIDTH].dx == -1
				|| part[n][i + 1][WIDTH].dy == -1)
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len + j;
				}
			}
			else if(part[n][i][WIDTH].fin == 1)
			{
				if(part[n][i - 1][WIDTH].dx == 1
				|| part[n][i - 1][WIDTH].dy == 1)
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len + j;
				}
				if(part[n][i - 1][WIDTH].dx == -1
				|| part[n][i - 1][WIDTH].dy == -1)
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len - j;
				}
			}
			else
			{
				if((part[n][i - 1][WIDTH].dx == -1
				 && part[n][i + 1][WIDTH].dx == 1)
				|| (part[n][i - 1][WIDTH].dy == -1
				 && part[n][i + 1][WIDTH].dy == 1))
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len + 2 * j;
				}
				else if((part[n][i - 1][WIDTH].dx == 1
				 && part[n][i + 1][WIDTH].dx == -1)
				|| (part[n][i - 1][WIDTH].dy == 1
				 && part[n][i + 1][WIDTH].dy == -1))
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len - 2 * j;
				}
				else
				{
					part[n][i][WIDTH + j].len
					= part[n][i][WIDTH].len;
				}
			}
		}
		for(j = -WIDTH;j <= WIDTH;j++)
		{
			if(part[n][i][WIDTH].dx == 1)
			{
				if(part[n][i + 1][WIDTH].dy == 1)
				{
					if(f == 1)
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;

					}
				}
				else
				{
					if(f == 1)
						f = 0;
					else
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
						f = 1;
					}
				}
			}
			if(part[n][i][WIDTH].dx == -1)
			{
				if(part[n][i + 1][WIDTH].dy == -1)
				{
					if(f == 1)
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
					}
				}
				else
				{
					if(f == 1)
						f = 0;
					else
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
						f = 1;
					}
				}
			}

			if(part[n][i][WIDTH].dy == 1)
			{

				if(part[n][i + 1][WIDTH].dx == 1)
				{
					if(f == 1)
						f = 0;
					else
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
						f = 1;
					}
				}
				else
				{
					if(f == 1)
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
					}
				}
			}
			if(part[n][i][WIDTH].dy == -1)
			{
				if(part[n][i + 1][WIDTH].dx == 1)
				{
					if(f == 1)
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
					}
				}
				else
				{
					if(f == 1)
						f = 0;
					else
					{
						a = part[n][i + 1][WIDTH + j].len;
						part[n][i + 1][WIDTH + j].len
						= part[n][i + 1][WIDTH - j].len;
						part[n][i + 1][WIDTH - j].len = a;
						f = 1;
					}
				}
			}
		}
		if(part[n][i][WIDTH].fin == 1)
			break;
	}
}

void change_xy(int n)
{
	for(int i = 0;;i++)
	{
		for(int j = -WIDTH;j <= WIDTH;j++)
		{
			if(i == 0)
			{
				part[n][0][WIDTH + j].x = part[n][0][WIDTH].x;
				part[n][0][WIDTH + j].y = part[n][0][WIDTH].y;
				if(part[n][0][WIDTH].dy == 0)
				{
					part[n][0][WIDTH + j].y += j;
				}
				if(part[n][0][WIDTH].dx == 0)
				{
					part[n][0][WIDTH + j].x += j;
				}
			}
			else
			{
				part[n][i][WIDTH + j].x
				= part[n][i - 1][WIDTH + j].x
				+ part[n][i - 1][WIDTH].dx
				* part[n][i - 1][WIDTH + j].len;

				part[n][i][WIDTH + j].y
				= part[n][i - 1][WIDTH + j].y
				+ part[n][i - 1][WIDTH].dy
				* part[n][i - 1][WIDTH + j].len;
			}
		}
		if(part[0][i][WIDTH].fin == 1)
			break;
	}
}

void draw_road(int n)
{
	for(int i = 0;;i++)
	{
		for(int j = -WIDTH;j <= WIDTH;j++)
		{
			for(int g = 0;g < part[n][i][WIDTH + j].len;g++)
			{
				if(part[n][i][WIDTH].fin == 1 && g == part[n][i][WIDTH + j].len - 1)
				{
					p[part[n][i][WIDTH + j].y
					+ part[n][i][WIDTH].dy * g]
					[2 * (part[n][i][WIDTH + j].x
					+ part[n][i][WIDTH].dx * g)] = 176;

					p[part[n][i][WIDTH + j].y
					+ part[n][i][WIDTH].dy * g]
					[2 * (part[n][i][WIDTH + j].x
					+ part[n][i][WIDTH].dx * g) + 1] = 10;
				}
				else
				{
					p[part[n][i][WIDTH + j].y
					+ part[n][i][WIDTH].dy * g]
					[2 * (part[n][i][WIDTH + j].x
					+ part[n][i][WIDTH].dx * g)] = 176;

					p[part[n][i][WIDTH + j].y
					+ part[n][i][WIDTH].dy * g]
					[2 * (part[n][i][WIDTH + j].x
					+ part[n][i][WIDTH].dx * g) + 1] = 6;
				}
			}
		}
		if(part[n][i][WIDTH].fin == 1)
			break;
	}
}
void roads()
{
	part[0][0][WIDTH].len = 20;
	part[0][0][WIDTH].dx = 0;
	part[0][0][WIDTH].dy = 1;
	part[0][0][WIDTH].fin = 0;
	part[0][0][WIDTH].x = 10;
	part[0][0][WIDTH].y = 10;

	part[0][1][WIDTH].len = 16;
	part[0][1][WIDTH].dx = 1;
	part[0][1][WIDTH].dy = 0;
	part[0][1][WIDTH].fin = 0;

	part[0][2][WIDTH].len = 8;
	part[0][2][WIDTH].dx = 0;
	part[0][2][WIDTH].dy = -1;
	part[0][2][WIDTH].fin = 0;

	part[0][3][WIDTH].len = 16;
	part[0][3][WIDTH].dx = 1;
	part[0][3][WIDTH].dy = 0;
	part[0][3][WIDTH].fin = 0;

	part[0][4][WIDTH].len = 16;
	part[0][4][WIDTH].dx = 0;
	part[0][4][WIDTH].dy = 1;
	part[0][4][WIDTH].fin = 1;

	part[1][0][WIDTH].len = 20;
	part[1][0][WIDTH].dx = 0;
	part[1][0][WIDTH].dy = 1;
	part[1][0][WIDTH].fin = 0;
	part[1][0][WIDTH].x = 10;
	part[1][0][WIDTH].y = 12;

	part[1][1][WIDTH].len = 30;
	part[1][1][WIDTH].dx = 1;
	part[1][1][WIDTH].dy = 0;
	part[1][1][WIDTH].fin = 0;

	part[1][2][WIDTH].len = 25;
	part[1][2][WIDTH].dx = 0;
	part[1][2][WIDTH].dy = -1;
	part[1][2][WIDTH].fin = 0;

	part[1][3][WIDTH].len = 30;
	part[1][3][WIDTH].dx = 1;
	part[1][3][WIDTH].dy = 0;
	part[1][3][WIDTH].fin = 0;

	part[1][4][WIDTH].len = 40;
	part[1][4][WIDTH].dx = 0;
	part[1][4][WIDTH].dy = 1;
	part[1][4][WIDTH].fin = 1;


	part[2][0][WIDTH].len = 20;
	part[2][0][WIDTH].dx = 0;
	part[2][0][WIDTH].dy = 1;
	part[2][0][WIDTH].fin = 0;
	part[2][0][WIDTH].x = 60;
	part[2][0][WIDTH].y = 10;

	part[2][1][WIDTH].len = 16;
	part[2][1][WIDTH].dx = -1;
	part[2][1][WIDTH].dy = 0;
	part[2][1][WIDTH].fin = 0;

	part[2][2][WIDTH].len = 8;
	part[2][2][WIDTH].dx = 0;
	part[2][2][WIDTH].dy = 1;
	part[2][2][WIDTH].fin = 0;

	part[2][3][WIDTH].len = 16;
	part[2][3][WIDTH].dx = -1;
	part[2][3][WIDTH].dy = 0;
	part[2][3][WIDTH].fin = 1;


	part[3][0][WIDTH].len = 20;
	part[3][0][WIDTH].dx = 0;
	part[3][0][WIDTH].dy = 1;
	part[3][0][WIDTH].fin = 0;
	part[3][0][WIDTH].x = 10;
	part[3][0][WIDTH].y = 10;

	part[3][1][WIDTH].len = 16;
	part[3][1][WIDTH].dx = 1;
	part[3][1][WIDTH].dy = 0;
	part[3][1][WIDTH].fin = 0;

	part[3][2][WIDTH].len = 8;
	part[3][2][WIDTH].dx = 0;
	part[3][2][WIDTH].dy = 1;
	part[3][2][WIDTH].fin = 1;

}
