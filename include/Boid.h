#ifndef BOID_H  // "Include Guard": prevents the file from being loaded a second time
#define BOID_H
#include <vector>

#include <SFML/Graphics.hpp>

class Boid {
private:
    sf::ConvexShape boid;
    sf::Vector2f velocity;

    // static => one variable for whole flock (not one for each boid)
    // constexpr => true constant
    static constexpr float avoidf = 0.1f; // Avoid factor used in separation
    static constexpr float matchf = 0.05f; // Matching factor used in alignment
    static constexpr float centerf = 0.002f; // Centering factor used in cohesion
    static constexpr float vrange = 70.0f; // Visual range
    static constexpr float prange = 30.0f; // Protected range
    static constexpr float turnf = 0.3f; // Turn factor used in smooth avoidance of screen edges
    static constexpr float maxspeed = 5.f; // Maximum speed
    static constexpr float minspeed = 3.f; // Minimum speed

    sf::Vector2f separation (std::vector<Boid>& flock); // Separation rule
    sf::Vector2f alignment (std::vector<Boid>& flock);  // Alignment rule
    sf::Vector2f cohesion (std::vector<Boid>& flock); // Cohesion rule
public:
    Boid(sf::Vector2f windowSize);

    // Methods (boid's behaviour)
    void update(sf::Vector2f windowSize, std::vector<Boid>& flock);
    void draw(sf::RenderWindow& window);  
};

#endif
