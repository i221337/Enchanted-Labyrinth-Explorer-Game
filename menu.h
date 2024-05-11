#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime> 
#include"maze.h"
using namespace sf;
using namespace std;


//==============Constant number of  Menu and stars  ==========
const int Max_item = 4;
const int numStars = 100;


//============= Menu Class ==============================
// inialization  of variables and funtion calling

class Menu
{
private:
    int menuSelected;
    Font font;
    Text menuOption[Max_item];
    Texture texture;
    Sprite backgroundImage;
    Texture bg_texture;
    Clock colorClock;

    Vector2f starPositions[numStars];
    Color starColors[numStars];

    void updateStarColors();
public:
    ///Menu();
    Menu(float width, float height, bool isLevelMenu);

    void draw(RenderWindow& window);
    void MoveUp();
    void MoveDown();
    int getMenuSelected() { return menuSelected; }
};


//============== Initialization of menu  options and stars  ==============
Menu::Menu(float width, float height, bool isLevelMenu)
{
    if (!font.loadFromFile("img/mine1bold.otf"))
    {
        cout << "Failed to load font file!" << endl;
    }
    if (!bg_texture.loadFromFile("img/titlescreen.png"))
    {
        cout << "Failed to load backgound file!" << endl;
    }
    backgroundImage.setTexture(bg_texture);
    // Initialize stars and their colors
    /*for (int i = 0; i < numStars; ++i)
    {
        int x = rand() % 800;
        int y = rand() % 120;
        starPositions[i] = Vector2f(x, y);

        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        starColors[i] = Color(r, g, b);
    }*/

    menuSelected = 0;

    // Initialize menu options differently based on whether it's a level menu or not
    if (isLevelMenu) {
        menuOption[0].setFont(font);
        menuOption[0].setFillColor(Color::White);
        menuOption[0].setString("Level 1");
        menuOption[0].setCharacterSize(25);
        menuOption[0].setPosition(280.f, 295.f);

        menuOption[1].setFont(font);
        menuOption[1].setFillColor(Color::Blue);
        menuOption[1].setString("Level 2");
        menuOption[1].setCharacterSize(25);
        menuOption[1].setPosition(280.f, 345.f);

        menuOption[2].setFont(font);
        menuOption[2].setFillColor(Color::Blue);
        menuOption[2].setString("Level 3");
        menuOption[2].setCharacterSize(25);
        menuOption[2].setPosition(280.f, 397.f);

        menuOption[3].setFont(font);
        menuOption[3].setFillColor(Color::Blue);
        menuOption[3].setString("Back");
        menuOption[3].setCharacterSize(25);
        menuOption[3].setPosition(304.f, 447.f);
    }
    else {
        // Initialize menu options for the main menu
        menuOption[0].setFont(font);
        menuOption[0].setFillColor(Color::White);
        menuOption[0].setString("Start New Game");
        menuOption[0].setCharacterSize(25);
        menuOption[0].setPosition(220.f, 295.f);

        menuOption[1].setFont(font);
        menuOption[1].setFillColor(Color::Blue);
        menuOption[1].setString("High Scores");
        menuOption[1].setCharacterSize(25);
        menuOption[1].setPosition(247.f, 345.f);

        menuOption[2].setFont(font);
        menuOption[2].setFillColor(Color::Blue);
        menuOption[2].setString("Help");
        menuOption[2].setCharacterSize(25);
        menuOption[2].setPosition(300.f, 397.f);

        menuOption[3].setFont(font);
        menuOption[3].setFillColor(Color::Blue);
        menuOption[3].setString("Quit");
        menuOption[3].setCharacterSize(25);
        menuOption[3].setPosition(300.f, 447.f);
    }
}

//==============     Printing Menu and Stars      ===============
void Menu::draw(RenderWindow& window)
{
    //updateStarColors();
    window.draw(backgroundImage);
    // Draw menu options
    for (int i = 0; i < Max_item; i++)
    {
        window.draw(menuOption[i]);
    }

    // Draw stars
    for (int i = 0; i < numStars; i++)
    {
        Text star("*", font, 30);
        star.setPosition(starPositions[i]);
        star.setFillColor(starColors[i]);
        window.draw(star);
    }
}

// ============= generate random star by setting clock ===============
void Menu::updateStarColors()
{
    if (colorClock.getElapsedTime().asSeconds() >= 1)
    {
        colorClock.restart();
        for (int i = 0; i < numStars; i++)
        {

            // Generate new random color for each star
            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;
            starColors[i] = Color(r, g, b);
        }
    }
}


// ============= key board setup to select menu option for move up ===
void Menu::MoveUp()
{
    if (menuSelected > 0)
    {
        menuOption[menuSelected].setFillColor(Color::Blue);
        menuSelected--;
        menuOption[menuSelected].setFillColor(Color::White);
    }
    else
    {
        menuOption[menuSelected].setFillColor(Color::Blue);
        menuSelected = Max_item - 1;
        menuOption[menuSelected].setFillColor(Color::White);
    }
}


// ============= key board setup to select menu option for move down ===
void Menu::MoveDown()
{
    if (menuSelected < Max_item - 1)
    {
        menuOption[menuSelected].setFillColor(Color::Blue);
        menuSelected++;
        menuOption[menuSelected].setFillColor(Color::White);
    }
    else
    {
        menuOption[menuSelected].setFillColor(Color::Blue);
        menuSelected = 0;
        menuOption[menuSelected].setFillColor(Color::White);
    }
}

// =============Main function of menu for render window & draw and calling maze class=== 
void drawmenu(RenderWindow& window, Menu& menu)
{
    // Clear the window
    window.clear();
    Music music;
    if (music.openFromFile("img/minecraft_audio.ogg"))
    {
        music.play();
        music.setLoop(1);
    }
    else
    {
        cout << "Failed to load music file!" << endl;
    }
    Sprite instructions;    // Help sprite
    Texture ins_texture;
    ins_texture.loadFromFile("img/instructions.png");
    instructions.setTexture(ins_texture);

    Sprite highpng;
    Texture highscores_texture;
    if (!highscores_texture.loadFromFile("img/highscores.png"))
    {
        cout << "Failed to load font file!" << endl;
    }
    highpng.setTexture(highscores_texture);
    Font highScoresFont;
    if (!highScoresFont.loadFromFile("img/mine1bold.otf"))
    {
        cout << "Failed to load font file!" << endl;
    }

    while (window.isOpen()) {
        //mouse position in window
        //cout << "Mouse pos: " << Mouse::getPosition(window).x << " " << Mouse::getPosition(window).y << endl;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyReleased) {
                if (event.key.code == Keyboard::Escape)
                    window.close();
                else if (event.key.code == Keyboard::Up)
                    menu.MoveUp();
                else if (event.key.code == Keyboard::Down)
                    menu.MoveDown();
                else if (event.key.code == Keyboard::Return) {
                    switch (menu.getMenuSelected()) {
                    case 0:
                    {
                        // If "Start New Game" is selected, open a new window for level selection
                        RenderWindow levelWindow(VideoMode(660, 660), "Select Level");
                        Menu levelMenu(window.getSize().x, window.getSize().y, 1); // Create a menu for level selection
                        levelWindow.clear();
                        while (levelWindow.isOpen()) {
                            Event levelEvent;
                            while (levelWindow.pollEvent(levelEvent)) {
                                if (levelEvent.type == Event::Closed)
                                    levelWindow.close();
                                else if (levelEvent.type == Event::KeyReleased)
                                {
                                    if (levelEvent.key.code == Keyboard::Up)
                                        levelMenu.MoveUp(); // Move up in level selection menu
                                    else if (levelEvent.key.code == Keyboard::Down)
                                        levelMenu.MoveDown(); // Move down in level selection menu
                                    else if (levelEvent.key.code == Keyboard::Return) {
                                        // Draw maze based on the selected level
                                        switch (levelMenu.getMenuSelected()) {
                                        case 0:
                                            music.pause();
                                            drawmaze(10); // Draw maze for level 1
                                            break;
                                        case 1:
                                            music.pause();
                                            drawmaze(15); // Draw maze for level 2
                                            break;
                                        case 2:
                                            music.pause();
                                            drawmaze(20); // Draw maze for level 3
                                            break;
                                        case 3:
                                            music.pause();
                                            levelWindow.close(); // Draw maze for level 4
                                            break;
                                        }
                                    }
                                }
                            }
                            levelMenu.draw(levelWindow); // Draw level selection menu
                            levelWindow.display();
                        }
                        break;
                    }
                    case 1:
                    {
                        //highscores
                        int highscores[5];
                        string players[5];
                        fstream inputf("img/highscores.txt", ios::in);
                        for (int i = 0; i < 5; i++)
                        {
                            getline(inputf, players[i], ':');
                            inputf >> highscores[i];
                            inputf.ignore();
                        }
                        inputf.close();
                        RenderWindow highScoresWindow(VideoMode(660, 660), "High Scores");
                        Text highScoresText;
                        highScoresText.setFont(highScoresFont);
                        highScoresText.setFillColor(Color::Black);
                        highScoresText.setCharacterSize(20);
                        highScoresText.setPosition(385.f, 153.f);
                        string highScoresString;
                        for (int i = 0; i < 5; i++)
                        {
                            highScoresString += players[i] + ": " + to_string(highscores[i]) + "\n";
                        }
                        highScoresText.setString(highScoresString);
                        //window for diaplaying highscores
                        while (highScoresWindow.isOpen())
                        {
                            Event hsEvent;
                            while (highScoresWindow.pollEvent(hsEvent))
                            {
                                if (hsEvent.type == Event::Closed)
                                    highScoresWindow.close();
                                if (hsEvent.type == Event::KeyReleased)
                                {
                                    if (hsEvent.key.code == Keyboard::Escape)
                                        highScoresWindow.close();
                                }
                            }
                            highScoresWindow.clear();
                            highScoresWindow.draw(highpng);
                            highScoresWindow.draw(highScoresText);
                            highScoresWindow.display();
                        }
                        break;
                    }
                    case 2:
                    {
                        //help menu
                        RenderWindow ins(VideoMode(660, 660), "help");
                        while (ins.isOpen())
                        {
                            Event insEvent;
                            while (ins.pollEvent(insEvent))
                            {
                                if (insEvent.type == Event::Closed)
                                    ins.close();
                                if (insEvent.type == Event::KeyReleased)
                                {
                                    if (insEvent.key.code == Keyboard::Escape)
                                        ins.close();
                                }
                            }
                            ins.clear();
                            ins.draw(instructions);
                            ins.display();
                        }
                        break;
                    }
                    case 3:
                        window.close();
                        break;
                    }
                }
            }
        }
        menu.draw(window);
        window.display();
    }
}

