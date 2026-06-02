#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include "ConsoleControl.h"

int MAP_WIDTH = 0;
int MAP_HEIGHT = 0;

char** map;

struct Player {
    std::string name;
    int x = 0;
    int y = 0;
    int hp = 0;
    int maxHp = 0;
    int potions = 0;
    int bombs = 0;
    int sword = 0;
    int keys = 0;
};

struct Enemy {
    char type;
    int x = 0;
    int y = 0;
    int hp = 0;
    int maxHp = 0;
    int damage = 0;
    bool alive = false;
};

Player player;
std::vector<Enemy> enemies;

void CreateMap()
{

    ConsoleClear();

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            ConsoleXY(x * 2, y);

            if (x == player.x && y == player.y)
            {
                ConsoleSetColor(WHITE, BLACK);
                std::cout << "@ ";
                continue;
            }

            bool isEnemy = false;
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y)
                {
                    if (enemies[i].type == 'J') 
                    {
                        ConsoleSetColor(DARKRED, BLACK);
                    }
                    else
                    {
                        ConsoleSetColor(RED, BLACK);
                    }

                    std::cout << enemies[i].type << " ";
                    isEnemy = true;
                    break;
                }
            }
            if (isEnemy) continue;

            char c = map[y][x];
            if (c == '#')
            {
                ConsoleSetColor(DARKGREY, DARKGREY);
                std::cout << "  ";
            }
            else if (c == '.')
            {
                ConsoleSetColor(BLACK, BLACK);
                std::cout << "  ";
            }
            else if (c == 'P')
            {
                ConsoleSetColor(GREEN, BLACK);
                std::cout << "P ";
            }
            else if (c == 'K')
            { 
                ConsoleSetColor(YELLOW, BLACK);
                std::cout << "K ";
            }
            else if (c == 'L')
            {
                ConsoleSetColor(DARKRED, BLACK);
                std::cout << "L ";
            }
            else if (c == 'S')
            {
                ConsoleSetColor(CYAN, BLACK);
                std::cout << "S ";
            }
            else if (c == 'B')
            {
                ConsoleSetColor(MAGENTA, BLACK);
                std::cout << "B ";
            }
            else 
            {
                ConsoleSetColor(WHITE, BLACK);
                std::cout << c << " ";
            }
        }

        std::cout << std::endl;
    }
}

void PrintUI()
{

    ConsoleXY(0, MAP_HEIGHT + 2);
    ConsoleSetColor(WHITE, BLACK);

    std::cout << "=== " << player.name << " ===" << std::endl;
    std::cout << "HP: " << player.hp << "/" << player.maxHp << std::endl;
    std::cout << "Potions: " << player.potions << " | Bombs: " << player.bombs << " | Sword: " << player.sword << " | Keys: " << player.keys << std::endl;
}

bool CharacterCanMove(int x, int y)
{

    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return false;
    if (map[y][x] == '#') return false;
    return true;
}

void MoveEnemies()
{
    for (int i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i].alive || enemies[i].type == 'J') continue;

        int direction = rand() % 4;
        int directionX = enemies[i].x, directionY = enemies[i].y;

        if (direction == 0)
        {
            directionY--;
        }
        else if (direction == 1)
        {
            directionY++;
        }
        else if (direction == 2)
        {
            directionX--;
        }
        else if (direction == 3)
        {
            directionX++;
        }

        if (CharacterCanMove(directionX, directionY) && !(directionX == player.x && directionY == player.y))
        {
            enemies[i].x = directionX;
            enemies[i].y = directionY;
        }
    }
}

std::string GetEnemyName(char type)
{

    if (type == 'G')
    {
        return "Goblin";
    }
    if (type == 'O')
    {
        return "Orc";
    }
    if (type == 'T')
    {
        return "Troll";
    }
    if (type == 'J')
    {
        return "Boss";
    }

    return "Enemy";
}

int main()
{

    srand(static_cast<unsigned int>(time(0)));
    ConsoleClear();

    std::ifstream file("map.txt");

    if (!file.is_open())
    {
        std::cout << "Error: map.txt can't be opened" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    file.close();

    MAP_HEIGHT = lines.size();
    MAP_WIDTH = lines.empty() ? 0 : lines[0].length();

    map = new char* [MAP_HEIGHT];
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        map[i] = new char[MAP_WIDTH + 1];
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            map[y][x] = lines[y][x];
            if (map[y][x] == '@')
            {
                player.x = x;
                player.y = y;
                map[y][x] = '.';
            }
        }
        map[y][MAP_WIDTH] = '\0';
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            char c = map[y][x];
            if (c == 'G' || c == 'O' || c == 'T' || c == 'J')
            {
                int life = 0, damage = 0;
                if (c == 'G')
                {
                    life = 35;
                    damage = 12;
                }
                else if (c == 'O')
                {
                    life = 65;
                    damage = 22;
                }
                else if (c == 'T')
                {
                    life = 85;
                    damage = 28;
                }
                else if (c == 'J')
                {
                    life = 160;
                    damage = 35;
                }

                enemies.push_back({ c, x, y, life, life, damage, true });
                map[y][x] = '.';
            }
        }
    }

    std::cout << "HELLO WELCOME TO Practica UF2 Dungeon\nWhat's your name?" << std::endl;
    std::cout << "NAME: ";
    std::cin >> player.name;
    std::cin.ignore();

    player.hp = player.maxHp = 100;
    player.potions = 0;
    player.bombs = 0;
    player.sword = 0;
    player.keys = 0;

    std::cout << "\nPress any key to start..." << std::endl;
    while (ConsoleInKey() == 0) {}

    bool inCombat = false;
    std::string input;
    int EnemieInCombat = -1;

    while (player.hp > 0)
    {
        CreateMap();
        PrintUI();

        int key = ConsoleInKey();

        if (!inCombat)
        {
            int newX = player.x;
            int newY = player.y;
            bool moved = false;

            if (key == KB_UP)
            {
                newY--;
                moved = true;
            }
            if (key == KB_DOWN)
            {
                newY++;
                moved = true;
            }
            if (key == KB_LEFT)
            {
                newX--;
                moved = true;
            }
            if (key == KB_RIGHT)
            {
                newX++;
                moved = true;
            }

            bool enemieFound = false;
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].alive && enemies[i].x == newX && enemies[i].y == newY)
                {
                    EnemieInCombat = i;
                    enemieFound = true;
                    break;
                }
            }

            if (enemieFound)
            {
                inCombat = true;
                std::cout << "\n>>> YOUR GOING TO FIGHT AGAINST " << GetEnemyName(enemies[EnemieInCombat].type) << "! <<<\n";
                continue;
            }

            if (moved && CharacterCanMove(newX, newY))
            {
                if (map[newY][newX] == 'L')
                {
                    if (player.keys > 0)
                    {
                        player.keys--;
                        map[newY][newX] = '.';
                    }
                    else
                    {
                        continue;
                    }
                }

                player.x = newX;
                player.y = newY;

                char cell = map[player.y][player.x];
                if (cell == 'P')
                {
                    player.potions++;
                    map[player.y][player.x] = '.';
                }
                else if (cell == 'B')
                {
                    player.bombs++;
                    map[player.y][player.x] = '.';
                }
                else if (cell == 'K')
                {
                    player.keys++;
                    map[player.y][player.x] = '.';
                }
                else if (cell == 'S')
                {
                    player.sword = 1;
                    map[player.y][player.x] = '.';
                }

                MoveEnemies();
            }
        }
        else
        {
            ConsoleClear();

            Enemy& actualEnemie = enemies[EnemieInCombat];

            std::cout << "[" << player.name << "] HP:[" << player.hp << "/" << player.maxHp << "]" << std::endl;
            std::cout << "[" << GetEnemyName(actualEnemie.type) << "] HP:[" << actualEnemie.hp << "/" << actualEnemie.maxHp << "]" << std::endl;
            std::cout << std::endl;
            std::cout << "What will you do?" << std::endl;
            std::cout << "attack | inventory | status | help" << std::endl;

            std::getline(std::cin, input);
            for (int i = 0; i < input.length(); i++)
            {
                input[i] = tolower(input[i]);
            }

            if (input == "attack")
            {
                int damage = (player.sword > 0) ? 28 : 8;

                if (rand() % 10 < 9)
                {
                    actualEnemie.hp -= damage;
                    std::cout << "You hit for " << damage << " damage!\n";
                }
                else
                {
                    std::cout << "You missed!\n";
                }
            }
            else if (input == "inventory")
            {
                std::cout << "potion or bomb? ";
                std::string item;
                std::getline(std::cin, item);
                for (int i = 0; i < item.length(); i++)
                {
                    item[i] = tolower(item[i]);
                }

                if (item == "potion" && player.potions > 0)
                {
                    player.hp += 40;
                    if (player.hp > player.maxHp)
                    {
                        player.hp = player.maxHp;
                    }
                    player.potions--;
                    std::cout << "You used a potion!\n";
                }
                else if (item == "bomb" && player.bombs > 0)
                {
                    actualEnemie.hp -= 100;
                    player.bombs--;
                    std::cout << "You used a bomb!\n";
                }
            }
            else if (input == "status")
            {
                PrintUI();
                ConsoleWait(3000);
            }
            else if (input == "help")
            {
                std::cout << "attack - Attack the enemy\n";
                std::cout << "inventory - To acces and use the objects\n";
                std::cout << "status - Show your stats/objects\n";
                std::cout << "help - Show this commands menu\n";
                ConsoleWait(3000);
            }

            if (actualEnemie.hp > 0)
            {
                if (rand() % 10 < 3)
                {
                    player.hp -= actualEnemie.damage;
                    std::cout << "The enemy attacks you!\n";
                }
                else
                {
                    std::cout << "The enemy missed!\n";
                }
            }

            if (actualEnemie.hp <= 0)
            {
                std::cout << "\nYou defeated the " << GetEnemyName(actualEnemie.type) << "!\n";
                map[actualEnemie.y][actualEnemie.x] = '.';
                actualEnemie.alive = false;
                inCombat = false;

                if (actualEnemie.type == 'J')
                {
                    ConsoleClear();
                    std::cout << "====================================\n";
                    std::cout << "            YOU WIN!\n";
                    std::cout << "====================================\n";
                    ConsoleWait(3000);
                    break;
                }
            }
        }

        ConsoleWait(100);
    }

    ConsoleClear();
    std::cout << "---YOUR GAME IS OVER---" << std::endl;
    std::cout << "-------YOU DIED--------" << std::endl;
    while (ConsoleInKey() == 0) {}

    return 0;
}