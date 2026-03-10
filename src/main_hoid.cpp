#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "Hoid.h"

int main() 
{
    // Create a screen (window) to see the simulation on
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Hoids Simulation - Test SFML");
    window.setFramerateLimit(60);
    std::srand(std::time(0));

    /* Convert screen limits from unsigned int to float to 
    use in calling the constructor and the update function */
    sf::Vector2f windowSizeFloat(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

    // Creating a flock of hoids
    std::vector<Hoid> flock;
    int noHoids=1000;

    bool simulation = false; // Has simulation started or is still in prep
    std::vector<sf::RectangleShape> obstacles; // Obstacle vector
    std::vector<sf::RectangleShape> exits; // Exists vector
    std::vector<sf::CircleShape> threats; // Threats vector

    // As long as the window is open run the simulation
    while(window.isOpen()) 
    {
        sf::Event event;
        window.setKeyRepeatEnabled(false); // Cannot hold key pressed

        // Prep map with threats, obstacles and exits
        while(window.pollEvent(event)) 
        {
            if(event.type == sf::Event::Closed)
                window.close();
            // If Enter key pressed spawn hoid flock and start simulation
            if(!simulation && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                for(int i = 0; i < noHoids; i++) 
                {
                    Hoid newHoid(windowSizeFloat);
                    newHoid.spawn(flock, obstacles, windowSizeFloat, threats);
                    flock.push_back(newHoid);
                }
                simulation = true;
            }
            // If T key pressed create threat on mouse position and add to threats vector
            if(!simulation && event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::T)
                {
                    sf::CircleShape t(15);
                    t.setOrigin(15, 15);
                    t.setPosition(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
                    t.setFillColor(sf::Color::Red);
                    window.draw(t);
                    threats.push_back(t);
                }
            if(!simulation && event.type == sf::Event::MouseButtonPressed)
            {
                // If left click create obstacle on mouse position and add to obstacles vector
                if(!simulation && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::RectangleShape o;
                    o.setSize(sf::Vector2f(75, 75));
                    o.setOrigin(37.5, 37.5);
                    o.setPosition(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
                    o.setFillColor(sf::Color::Yellow);
                    window. draw(o);
                    obstacles.push_back(o);
                }
                // If right click create exit on nearest screen edge and add to exits vector
                if(!simulation && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                {
                    sf::RectangleShape e;
                    e.setSize(sf::Vector2f(10, 50));
                    e.setOrigin(5, 25);
                    e.setFillColor(sf::Color::Green);
                    // Calculate nearest screen edge to mouse cursor
                    float x = sf::Mouse::getPosition(window).x, y = sf::Mouse::getPosition(window).y;
                    float dx = window.getSize().x-sf::Mouse::getPosition(window).x, dy = window.getSize().y-sf::Mouse::getPosition(window).y;
                    float minid = x;
                    if(minid > y) minid = y;
                    if(minid > dx) minid = dx;
                    if(minid > dy) minid =dy;
                    // Position exit to nearest cursor
                    if(minid == x)
                        e.setPosition(0, y);
                    else if(minid == y){
                            e.setPosition(x, 0);
                            e.setRotation(90);}
                        else if(minid == dx)
                            e.setPosition(window.getSize().x, y);
                            else{
                                e.setPosition(x, window.getSize().y);
                                e.setRotation(90);}
                    window. draw(e);
                    exits.push_back(e);
                }
            }
        }

        // Start simulation
        if(simulation)
        {
            for(auto& b : flock)
                b.update(windowSizeFloat, flock, exits, threats, obstacles);
            flock.erase(std::remove_if(flock.begin(),flock.end(), [&](Hoid& h){
                for(auto& e : exits)
                    if(h.getShape().getGlobalBounds().intersects(e.getGlobalBounds())) return true;
                return false;}), flock.end());
            window.clear();
            for(auto& o : obstacles)
                window.draw(o);
            for(auto& t : threats)
                window.draw(t);
            for(auto& e : exits)
                window.draw(e);
            for(auto& b : flock)
                b.draw(window);
        }
        window.display();
    }

    return 0;
}