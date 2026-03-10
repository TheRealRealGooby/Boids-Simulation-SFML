#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Boid.h"

int main() {
    // Create a screen (window) to see the simulation on
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Boids Simulation - Test SFML");
    window.setFramerateLimit(60);
    std::srand(std::time(0));

    /* Convert screen limits from unsigned int to float to 
    use in calling the constructor and the update function */
    sf::Vector2f windowSizeFloat(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

    // Creating a flock of boids
    std::vector<Boid> flock;
    int noBoids=300;
    for (int i = 0; i < noBoids; ++i) 
    {
        Boid newBoid(windowSizeFloat);
        flock.push_back(newBoid);
    }

    // As long as the window is open run the simulation
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

    // The flock animation
        for (auto& b : flock)
            b.update(windowSizeFloat, flock);
        window.clear();
        for (auto& b : flock)
            b.draw(window);
        window.display();
    }

    return 0;
}