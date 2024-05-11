#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <queue>
#include <fstream>
#include "avl.h"
using namespace std;
using namespace sf;

const int CELL_WIDTH = 30;

class Cell
{
public:
    int x, y;
    int pos;
    float size;
    float thickness;
    bool walls[4];
    bool visited;
    bool isPlayer;
    bool isPath;
    bool isEnemy;
    bool isEnemy2;
    bool isSword;
    bool isPotion;
    bool isKey;
    bool isLock;
    Cell()
    {
        x = 0;
        y = 0;
        pos = 0;
        size = 30.f;
        thickness = 4.f;
        walls[0] = true;
        walls[1] = true;
        walls[2] = true;
        walls[3] = true;
        visited = false;
        isPlayer = false;
        isPath = false;
        isEnemy = false;
        isEnemy2 = false;
        isSword = false;
        isPotion = false;
        isKey = false;
        isLock = false;
    }
    Cell(int x, int y)
    {
        this->x = x;
        this->y = y;
        pos = 0;
        size = 30.f;
        thickness = 4.f;
        walls[0] = true;
        walls[1] = true;
        walls[2] = true;
        walls[3] = true;
        visited = false;
        isPlayer = false;
        isPath = false;
        isEnemy = false;
        isEnemy2 = false;
        isSword = false;
        isPotion = false;
        isKey = false;
        isLock = false;
    }

    // draw the cell and its walls
    void draw(RenderWindow* window, Sprite& enemy, Texture& enemy_texture, Sprite& sword, Texture& sword_texture, int& score, AVLTree& inventory, Sprite player, Sprite potion, Texture potion_texture, Sprite key, Texture key_texture, Sprite lock, Texture lock_texture, Sprite enemy2, Texture enemy2_texture)
    {
        RectangleShape rect;

        if (isPlayer)
        {
            rect.setFillColor(Color(247, 23, 53)); // player ghost color
            rect.setSize(Vector2f(size, size));
            rect.setPosition(x, y);
            window->draw(rect);
        }
        else if (isPath)
        {
            rect.setFillColor(Color(255, 100, 150)); //shortest path color
            rect.setSize(Vector2f(size, size));
            rect.setPosition(x, y);
            window->draw(rect);
        }

        if (isEnemy)
        {
            enemy.setTexture(enemy_texture);
            enemy.setPosition(x + 4.f, y + 4.f);
            window->draw(enemy);
        }

        if (isEnemy2)
        {
            enemy2.setTexture(enemy2_texture);
            enemy2.setPosition(x + 4.f, y + 4.f);
            window->draw(enemy2);
        }

        if (isSword)
        {
            sword.setTexture(sword_texture);
            sword.setPosition(x, y);
            window->draw(sword);
        }

        if (isPotion)
        {
            potion.setTexture(potion_texture);
            potion.setPosition(x + 4.f, y + 4.f);
            window->draw(potion);
        }

        if (isKey)
        {
            key.setTexture(key_texture);
            key.setPosition(x + 4.f, y + 8.f);
            window->draw(key);
        }
        
        if (isLock)
        {
            lock.setTexture(lock_texture);
            lock.setPosition(x + 4.f, y + 4.f);
            window->draw(lock);
        }

        // check for collision
        if (player.getGlobalBounds().intersects(sword.getGlobalBounds()))
        {
            sword.setPosition(-100, -100);
            isSword = false;
            inventory.insert(1);
        }

        if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
        {
            enemy.setPosition(-100, -100);
            isEnemy = false;
            if (inventory.search(1))
            {
                inventory.remove(1);
                score += 50;
            }
            else
                score -= 50;
        }

        if (player.getGlobalBounds().intersects(enemy2.getGlobalBounds()))
        {
            enemy2.setPosition(-100, -100);
            isEnemy2 = false;
            if (inventory.search(1))
            {
                inventory.remove(1);
                score += 50;
            }
            else
                score -= 50;
        }

        if (player.getGlobalBounds().intersects(potion.getGlobalBounds()))
        {
            potion.setPosition(-100, -100);

            if (isPotion == true)
            {
                score += 50;
                isPotion = false;
                inventory.insert(2);
            }
        }

        if (player.getGlobalBounds().intersects(key.getGlobalBounds()))
        {
            key.setPosition(-100, -100);

            if (isKey == true)
            {
                isKey = false;
                inventory.insert(3);
            }
        }

        if (player.getGlobalBounds().intersects(lock.getGlobalBounds()))
        {
            if (isLock == true)
            {
                if (inventory.search(3))
                {
                    inventory.remove(3);
                    score += 50;
                    lock.setPosition(-100, -100);
                }
                isLock = false;
            }
        }

        rect.setFillColor(Color(0, 0, 0)); // wall color
        if (walls[0])
        {
            rect.setSize(Vector2f(size, thickness));
            rect.setPosition(x, y);
            window->draw(rect);
        }
        if (walls[1])
        {
            rect.setSize(Vector2f(thickness, size));
            rect.setPosition(x + size, y);
            window->draw(rect);
        }
        if (walls[2])
        {
            rect.setSize(Vector2f(size + thickness, thickness));
            rect.setPosition(x, y + size);
            window->draw(rect);
        }
        if (walls[3])
        {
            rect.setSize(Vector2f(thickness, size));
            rect.setPosition(x, y);
            window->draw(rect);
        }
    }
};

//  reset the maze with player reached at end
void resetMaze(Cell* maze, int size)
{
    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            maze[i].walls[j] = true;
        }
        maze[i].visited = false;
        maze[i].isPlayer = false;
        maze[i].isPath = false;
        maze[i].isEnemy = false;
        maze[i].isEnemy2 = false;
        maze[i].isSword = false;
        maze[i].isPotion = false;
        maze[i].isKey = false;
        maze[i].isLock = false;
    }
}

//  remove walls between two adjacent cells
void removeWallsBetween(Cell* maze, Cell* current, Cell* chosen, int size)
{
    if (current->pos - size == chosen->pos)
    {
        current->walls[0] = false;
        chosen->walls[2] = false;
    }
    else if (current->pos + 1 == chosen->pos)
    {
        current->walls[1] = false;
        chosen->walls[3] = false;
    }
    else if (current->pos + size == chosen->pos)
    {
        current->walls[2] = false;
        chosen->walls[0] = false;
    }
    else if (current->pos - 1 == chosen->pos)
    {
        current->walls[3] = false;
        chosen->walls[1] = false;
    }
}

//  generate the maze using depth-first search 
void makeMaze(Cell* maze, int size)
{
    srand(time(0));
    resetMaze(maze, size);
    stack<Cell> stack;
    maze[0].visited = true;
    stack.push(maze[0]);

    while (!stack.empty())
    {
        Cell current = stack.top();
        stack.pop();
        int pos = current.pos;
        int neighbours[4];
        int count = 0;

        // find unvisited neighbours
        if ((pos) % (size) != 0 && pos > 0)
        {
            Cell left = maze[pos - 1];
            if (!left.visited)
            {
                neighbours[count++] = pos - 1;
            }
        }
        if ((pos + 1) % (size) != 0 && pos < size * size)
        {
            Cell right = maze[pos + 1];
            if (!right.visited)
            {
                neighbours[count++] = pos + 1;
            }
        }
        if ((pos + size) < size * size)
        {
            Cell bottom = maze[pos + size];
            if (!bottom.visited)
            {
                neighbours[count++] = pos + size;
            }
        }
        if ((pos - size) > 0)
        {
            Cell top = maze[pos - size];
            if (!top.visited)
            {
                neighbours[count++] = pos - size;
            }
        }

        if (count > 0)
        {
            int randneighbourpos = rand() % count;
            Cell* chosen = &maze[neighbours[randneighbourpos]];

            stack.push(current);
            removeWallsBetween(maze, &maze[current.pos], chosen, size);

            chosen->visited = true;
            stack.push(*chosen);
        }
    }
}

// using Dijkstra's algorithm
stack<int> findShortestPath(Cell* maze, int size, int startIdx, int endIdx) {
    int* dist = new int[size * size];
    int* prev = new int[size * size];
    auto comp = [&](int lhs, int rhs) 
    {
        return dist[lhs] > dist[rhs];
    };
    priority_queue<int, deque<int>, decltype(comp)> pq(comp);
    for (int i = 0; i < size * size; ++i) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[startIdx] = 0;
    pq.push(startIdx);

    while (!pq.empty()) {
        int currentIdx = pq.top();
        pq.pop();
        Cell& current = maze[currentIdx];
        if (currentIdx == endIdx) {
            break;
        }
        int neighbors[4] = { -1, -1, -1, -1 };
        if (!current.walls[0] && currentIdx - size >= 0) {
            neighbors[0] = currentIdx - size;
        }
        if (!current.walls[1] && (currentIdx + 1) % size != 0) {
            neighbors[1] = currentIdx + 1;
        }
        if (!current.walls[2] && currentIdx + size < size * size) {
            neighbors[2] = currentIdx + size;
        }
        if (!current.walls[3] && currentIdx % size != 0) {
            neighbors[3] = currentIdx - 1;
        }

        for (int nextIdx : neighbors) {
            if (nextIdx != -1) {
                int alt = dist[currentIdx] + 1;
                if (alt < dist[nextIdx]) {
                    dist[nextIdx] = alt;
                    prev[nextIdx] = currentIdx;
                    pq.push(nextIdx);
                }
            }
        }
    }

    int crawl = endIdx;
    stack<int> shortestPath;

    while (prev[crawl] != -1) {
        shortestPath.push(crawl);
        crawl = prev[crawl];
    }
    shortestPath.push(startIdx);

    delete[] dist;
    delete[] prev;

    return shortestPath;
}

void colorShortestPath(Cell* maze, stack<int> shortestPath)
{
    int count = 0;
    while (!shortestPath.empty() && count < 16)
    {
        int cellIdx = shortestPath.top();
        shortestPath.pop();
        maze[cellIdx].isPath = true;
        count++;
    }
}

void generateSpritesAtRandom(Cell* maze, stack<int> path, int playerPos, int size) {
    int pathLength = path.size();
    int* pathIndices = new int[pathLength + 1];
    int count = 0;
    while (!path.empty()) {
        int cellIdx = path.top();
        path.pop();
        pathIndices[count++] = cellIdx;
    }
    srand(time(0));

    int swordIdx = 0;
    int enemyIdx1 = 0;
    int enemyIdx2 = 0;
    int potionIdx = 0;
    int keyIdx = 0;
    int lockIdx = 0;
    lockIdx = pathIndices[1];
    do
    {
        swordIdx = pathIndices[rand() % pathLength];
    } while (swordIdx == 0 || swordIdx == pathLength - 1 || swordIdx == playerPos);

    int r1 = rand() % (size * size - 1);
    stack<int> p3 = findShortestPath(maze, size, r1, swordIdx);
    int p3Length = p3.size();
    int* p3Indices = new int[p3Length + 1];
    int c3 = 0;
    while (!p3.empty()) {
        int cellId = p3.top();
        p3.pop();
        p3Indices[c3++] = cellId;
    }

    do
    {
        enemyIdx1 = p3Indices[rand() % p3Length];
    } while (enemyIdx1 == 0 || enemyIdx1 == p3Length - 1 || enemyIdx1 == playerPos || enemyIdx1 == swordIdx);

    do
    {
        enemyIdx2 = p3Indices[rand() % p3Length];
    } while (enemyIdx2 == 0 || enemyIdx2 == p3Length - 1 || enemyIdx2 == playerPos || enemyIdx2 == swordIdx || enemyIdx2 == enemyIdx1);

    do
    {
        potionIdx = pathIndices[rand() % pathLength];
    } while (potionIdx == 0 || potionIdx == pathLength - 1 || potionIdx == playerPos || potionIdx == swordIdx || potionIdx == enemyIdx1 || potionIdx == enemyIdx2);

    int endIdx = 0;
    int maxDiLock = 5;
    int minDiKey = 10;
    int diToLock = abs(endIdx - lockIdx);
    int diToKey = abs(endIdx - keyIdx);

    while (diToLock > maxDiLock || diToKey < minDiKey || keyIdx == potionIdx || keyIdx == enemyIdx1 || keyIdx == enemyIdx2 || keyIdx == lockIdx || keyIdx == swordIdx) {
        keyIdx = pathIndices[rand() % pathLength];
        lockIdx = pathIndices[rand() % pathLength];
        diToLock = abs(endIdx - lockIdx);
        diToKey = abs(endIdx - keyIdx);
    }

    int r = rand() % (size * size - 1);
    stack<int> p2 = findShortestPath(maze, size, r, keyIdx);
    int p2Length = p2.size();
    int* p2Indices = new int[p2Length + 1];
    int c2 = 0;
    while (!p2.empty()) {
        int cellId = p2.top();
        p2.pop();
        p2Indices[c2++] = cellId;
    }

    do
    {
        lockIdx = p2Indices[rand() % p2Length];
    } while (lockIdx == 0 || lockIdx == p2Length - 1 || lockIdx == playerPos || lockIdx == swordIdx || lockIdx == enemyIdx1 || lockIdx == potionIdx || lockIdx == keyIdx || lockIdx == enemyIdx2);

    maze[swordIdx].isSword = true;
    maze[enemyIdx1].isEnemy = true;
    maze[enemyIdx2].isEnemy2 = true;
    maze[potionIdx].isPotion = true;
    maze[keyIdx].isKey = true;
    maze[lockIdx].isLock = true;

    delete[] pathIndices;
    delete[] p2Indices;
    delete[] p3Indices; 
}


// handle player movement
void handleMove(Event event, Cell* maze, int* currentPos, int size, AVLTree& inventory)
{
    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A)
    {
        if (!maze[*currentPos].walls[3] && !maze[*currentPos - 1].walls[1])
        {
            if (maze[*currentPos - 1].isLock != true)
            {
                *currentPos = *currentPos - 1;
                maze[*currentPos].isPlayer = true;
            }
            else if (inventory.search(3))
            {
                *currentPos = *currentPos - 1;
                maze[*currentPos].isPlayer = true;
            }
        }
    }
    else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D)
    {
        if (!maze[*currentPos].walls[1] && !maze[*currentPos + 1].walls[3])
        {
            if (maze[*currentPos + 1].isLock != true)
            {
                *currentPos = *currentPos + 1;
                maze[*currentPos].isPlayer = true;
            }
            else if (inventory.search(3))
            {
                *currentPos = *currentPos + 1;
                maze[*currentPos].isPlayer = true;
            }
        }
    }
    else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
    {
        if ((*currentPos - size) < 0)
        {
            return;
        }
        if (!maze[*currentPos].walls[0] && !maze[*currentPos - size].walls[2])
        {
            if (maze[*currentPos - size].isLock != true)
            {
                *currentPos = *currentPos - size;
                maze[*currentPos].isPlayer = true;
            }
            else if (inventory.search(3))
            {
                *currentPos = *currentPos - size;
                maze[*currentPos].isPlayer = true;
            }
        }
    }
    else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
    {
        if ((*currentPos + size) > size * size)
        {
            return;
        }
        if (!maze[*currentPos].walls[2] && !maze[*currentPos + size].walls[0])
        {
            if (maze[*currentPos + size].isLock != true)
            {
                *currentPos = *currentPos + size;
                maze[*currentPos].isPlayer = true;
            }
            else if (inventory.search(3))
            {
                *currentPos = *currentPos + size;
                maze[*currentPos].isPlayer = true;
            }
        }
    }
    else if (event.key.code == Keyboard::H)
    {
        int curr = *currentPos;
        colorShortestPath(maze, findShortestPath(maze, size, curr, size * size - 1));
    }
}

void file_handling(int score)
{
    //file handling
    int* highscores = new int[5];
    int* temp = new int[6];
    char* ptr[5];
    for (int i = 0; i < 5; i++)
    {
        ptr[i] = new char[20];
    }
    fstream inputf("img/highscores.txt", ios::in);
    int index = 0;
    while (!inputf.eof())
    {
        if (index == 5)
        {
            break;
        }
        inputf.getline(ptr[index], 20, ': ');
        inputf >> highscores[index];
        inputf.ignore();
        index++;
    }
    inputf.close();
    // total score;
    temp[0] = score;
    for (int i = 1; i < 6; i++)
    {
        temp[i] = highscores[i - 1];
    }
    for (int i = 0; i < 6 - 1; i++)
    {
        for (int j = 0; j < 6 - i - 1; j++)
        {
            if (temp[j] < temp[j + 1])
            {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    delete[]highscores;
    highscores = new int[5];
    for (int i = 0; i < 5; i++)
    {
        highscores[i] = temp[i];
    }
    fstream writef("img/highscores.txt", ios::out);

    for (int i = 0; i < 5; i++)
    {
        writef << ptr[i] << " " << highscores[i] << endl;
    }
    delete[]temp;
    writef.close();
}

int drawmaze(const int SIZE)
{
    Sprite bg;
    Texture bg_texture; 
    if (!bg_texture.loadFromFile("img/bg2.png"))
    {
        cout << "Failed to load font file!" << endl;
    }
    bg.setTexture(bg_texture);

    Music music, oof;
    if (music.openFromFile("img/Battle.ogg"))
    {
        music.play();
        music.setLoop(1);
    }
    else
    {
        cout << "Failed to load music file!" << endl;
    }
    if (!oof.openFromFile("img/oof.ogg"))
    {
        cout << "Failed to oof music file!" << endl;
    }

    Sprite endpng;
    Texture end_texture;
    if (!end_texture.loadFromFile("img/end.png"))
    {
        cout << "Failed to load font file!" << endl;
    }
    endpng.setTexture(end_texture);

    srand(time(0));
    AVLTree inventory;
    int score = 0;
    Sprite sword, dummy_sword;
    Texture sword_texture;
    if (!sword_texture.loadFromFile("img/sword.png"))
    {
        cout << "sword_texture not loaded\n";
    }
    sword.setTexture(sword_texture);
    dummy_sword.setTexture(sword_texture);

    Sprite enemy;
    Texture enemy_texture;
    if (!enemy_texture.loadFromFile("img/enemy.jpg"))
    {
        cout << "enemy_texture not loaded\n";
    }
    enemy.setTexture(enemy_texture);

    Sprite enemy2;
    Texture enemy2_texture;
    if (!enemy2_texture.loadFromFile("img/creeper.png"))
    {
        cout << "enemy2_texture not loaded\n";
    }
    enemy2.setTexture(enemy2_texture);

    Sprite potion, dummy_potion;
    Texture potion_texture;
    if (!potion_texture.loadFromFile("img/potion.png"))
    {
        cout << "potion_texture not loaded\n";
    }
    potion.setTexture(potion_texture);
    dummy_potion.setTexture(potion_texture);

    Sprite key, dummy_key;
    Texture key_texture;
    if (!key_texture.loadFromFile("img/key.png"))
    {
        cout << "key_texture not loaded\n";
    }
    key.setTexture(key_texture);
    dummy_key.setTexture(key_texture);
    
    Sprite lock;
    Texture lock_texture;
    if (!lock_texture.loadFromFile("img/lock.png"))
    {
        cout << "lock_texture not loaded\n";
    }
    lock.setTexture(lock_texture);

    int currentPos = 0;
    RenderWindow window(VideoMode(CELL_WIDTH * SIZE + 60 + 150, CELL_WIDTH * SIZE + 60), "RANDOM MAZE");
    Cell* maze = new Cell[SIZE * SIZE];
    window.setFramerateLimit(30);

    // Initialize maze cells
    for (int i = 30, k = 0; i < CELL_WIDTH * SIZE + 30; i += CELL_WIDTH)
    {
        for (int j = 30; j < CELL_WIDTH * SIZE + 30; j += CELL_WIDTH, k++)
        {
            maze[k].y = i;
            maze[k].x = j;
            maze[k].size = CELL_WIDTH;
            maze[k].pos = k;
        }
    }

    // Generate maze
    makeMaze(maze, SIZE);
    maze[currentPos].isPlayer = true;

    // for player
    Sprite currentPosRect;
    Texture player_texture;
    if (!player_texture.loadFromFile("img/player.jpg"))
    {
        cout << "player_texture not loaded\n";
    }
    currentPosRect.setTexture(player_texture);

    //for diamond(end cell)
    Sprite finishRect;
    Texture diamond_texture;
    if (!diamond_texture.loadFromFile("img/diamond.png"))
    {
        cout << "diamond_texture not loaded\n";
    }
    finishRect.setTexture(diamond_texture);

    int r = rand() % (SIZE * SIZE);
    generateSpritesAtRandom(maze, findShortestPath(maze, SIZE, r, currentPos), currentPos, SIZE);


    while (window.isOpen())
    {
        //mouse position in window
        //cout << "Mouse pos: " << Mouse::getPosition(window).x << " " << Mouse::getPosition(window).y << endl;
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                if (event.key.code == Keyboard::Escape)
                {
                    file_handling(score);
                    window.close();
                }
                else 
                {
                    handleMove(event, maze, &currentPos, SIZE, inventory);
                }
                break;
            default:
                break;
            }
        }

        // check if player reached the finish
        if (currentPos == (SIZE * SIZE - 1))
        {
            score += 100;
            makeMaze(maze, SIZE);
            currentPos = 0;
            maze[currentPos].isPlayer = true;
            int r = (rand() % (SIZE * SIZE - 1)) - 1;
            generateSpritesAtRandom(maze, findShortestPath(maze, SIZE, r, currentPos), currentPos, SIZE);
        }

        // Background color 
        //window.clear(Color(13, 300, 330));
        window.draw(bg);
        // Draw maze cells
        for (int i = 0; i < SIZE * SIZE; i++)
        {
            maze[i].draw(&window, enemy, enemy_texture, sword, sword_texture, score, inventory, currentPosRect, potion, potion_texture, key, key_texture, lock, lock_texture, enemy, enemy2_texture);
            window.draw(dummy_sword);
            window.draw(dummy_potion);
            window.draw(dummy_key);
        }

        //player's current position
        currentPosRect.setPosition(maze[currentPos].x + 4.f, maze[currentPos].y + 4.f);
        window.draw(currentPosRect);

        //  finish position
        finishRect.setPosition(maze[SIZE * SIZE - 1].x + 4.f, maze[SIZE * SIZE - 1].y + 4.f);
        window.draw(finishRect);

        Font font;
        if (!font.loadFromFile("img/mine1bold.otf"))
        {
            cerr << "Error loading font" << endl;
        }
        Text text;
        text.setFont(font);

        // Display total_score
        text.setString("Score: " + to_string(score));
        text.setPosition(CELL_WIDTH* SIZE + 60, 30);
        text.setCharacterSize(20);
        window.draw(text);

        // Display INVENTORY
        text.setString("Inventory");
        text.setCharacterSize(20);
        text.setFillColor(Color::White);
        text.setPosition(CELL_WIDTH * SIZE + 60, 60);
        window.draw(text);

        dummy_sword.setPosition(CELL_WIDTH * SIZE + 60, 90);
        text.setString("x " + to_string(inventory.getTotal(1)/2));
        text.setPosition(CELL_WIDTH * SIZE + 60 + 40, 93);
        window.draw(text);
    
        dummy_potion.setPosition(CELL_WIDTH* SIZE + 60, 133);
        text.setString("x " + to_string(inventory.getTotal(2)));
        text.setPosition(CELL_WIDTH* SIZE + 60 + 40, 133);
        window.draw(text);

        dummy_key.setPosition(CELL_WIDTH * SIZE + 65, 180);
        text.setString("x " + to_string(inventory.getTotal(3)));
        text.setPosition(CELL_WIDTH * SIZE + 60 + 40, 174);
        window.draw(text);

        window.display();

        if (score < 0) 
        {
            oof.play();
            music.pause();
            RenderWindow end(VideoMode(660, 660), "help");
            while (end.isOpen())
            {
                //mouse position in window
                //cout << "Mouse pos: " << Mouse::getPosition(end).x << " " << Mouse::getPosition(end).y << endl;
                Event endEvent;
                while (end.pollEvent(endEvent))
                {
                    if (endEvent.type == Event::Closed)
                        end.close();
                    if (endEvent.type == Event::KeyReleased)
                    {
                        if (endEvent.key.code == Keyboard::Escape)
                            end.close();
                    }
                }
                end.clear();
                end.draw(endpng);
                if (score == -50)
                    text.setString("               You Died\n\nPlayer was shot by skeleton\n\n               Score: " + to_string(score + 50));
                else
                    text.setString("               You Died\n\nPlayer was blown by creeper\n\n               Score: " + to_string(score + 150));
                text.setPosition(160, 276);
                text.setCharacterSize(20);
                end.draw(text);
                end.display();
            }
            window.close();
        }
    }

    delete[]maze;
    return 0;
}

