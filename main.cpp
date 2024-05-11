#include <SFML/Graphics.hpp>
#include "Menu.h" 

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(660, 660), "Menu");
    Menu menu(window.getSize().x, window.getSize().y, 0);
    drawmenu(window, menu);
    return 0;
}

