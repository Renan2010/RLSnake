#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <ctime>
// Variables
int SIZE = 30;
int vx = SIZE * SIZE;
int vy = SIZE * 15;
int verticalLine = vy / SIZE;
int horizontalLine = vx / SIZE;
int direction = 1;
int num = 3;
// Game logic
struct Fruit
{
    int x, y;
}apple;


struct snake
{
    int x , y;
} Snake[100];

void gameAction()
{
    for(int i = num; i > 0; --i)
    {
        Snake[i].x = Snake[i - 1].x;
        Snake[i].y = Snake[i - 1].y;
    }
    if(direction == 0)
    {
        Snake[0].y += 1;
    } else if(direction == 1)
    {
        Snake[0].x -= 1;
    } else if(direction == 2)
    {
        Snake[0].x += 1;
    } else if(direction == 3)
    {
        Snake[0].y -= 1;
    }
    if((Snake[0].x == apple.x) && (Snake[0].y == apple.y))
    {
        num++;
        apple.x = rand() % 20;
        apple.y = rand() % 15;
    }    


}   

// SFML Code
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    // SFML window
    sf::RenderWindow window(sf::VideoMode(vx, vy), "RLSnake Game SFML C++", sf::Style::Titlebar | sf::Style::Close);
    sf::RectangleShape block;
    sf::RectangleShape blockapple;
    sf::RectangleShape blocksnake;
    sf::Clock clock;
    float chrono = 0;
    float delay = 0.1;
    // Block(head) color
    block.setSize(sf::Vector2f(SIZE , SIZE));
    block.setFillColor(sf::Color::Black);
    block.setOutlineThickness(1);
    block.setOutlineColor(sf::Color::White);
    // Snake Block color
    blocksnake.setSize(sf::Vector2f(SIZE , SIZE));
    blocksnake.setFillColor(sf::Color::Green);
    blocksnake.setOutlineThickness(1);
    blocksnake.setOutlineColor(sf::Color::White);

    // Apple
    apple.x = rand() & horizontalLine;
    apple.y = rand() & verticalLine;
    // Apple color
    blockapple.setSize(sf::Vector2f(SIZE , SIZE));
    blockapple.setFillColor(sf::Color::Red);
    blockapple.setOutlineThickness(1);
    blockapple.setOutlineColor(sf::Color::White);


    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        chrono += time;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            window.clear();    
            for(int i = 0; i < verticalLine; i++)
            {
                for (int j = 0; j < horizontalLine; j++)
                {
                    block.setPosition(j * SIZE, i * SIZE);
                    window.draw(block);
                }   
        
            }
            
            
            
        }
        // Time 
        if(chrono > delay)
        {
            chrono = 0;
            gameAction();
            
        }
        window.clear();
        // Keyboard config
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != 2)
            {
                direction = 1;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != 1)
            {
                direction = 2;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != 0)
            {
                direction = 3;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != 3)
            {
                direction = 0;
            }

        for(int i = 0; i < num; i++)
        {
            blocksnake.setPosition(Snake[i].x * SIZE,Snake[i].y * SIZE);
            window.draw(blocksnake);
        }
        blockapple.setPosition(apple.x * SIZE, apple.y * SIZE);
        window.draw(blockapple);
        window.display();
    
    }
}