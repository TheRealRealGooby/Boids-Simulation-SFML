#ifndef HOID_H 
#define HOID_H
#include <vector>

#include <SFML/Graphics.hpp>

class Hoid {
private:
    sf::CircleShape hoid;
    sf::Vector2f velocity;
    sf::Vector2f human_error; // Gives 'personality' to hoid's velocity
    float personal_speed; // Gives 'personality' to hoid's speed
    float speed; // Hoid speed
    static constexpr float breakf = 0.6f; // Breaking factor used after exiting 
    static constexpr float exitf = 2.f; // Exit factor used in thowards_exit rule
    static constexpr float threatf = -0.03f; // Threat factor used in avoid_threat rule
    static constexpr float base_max_speed = 2.5f; // Maximum speed if hoid not in danger zone
    static constexpr float panic_max_speed = 5.5f; // Maximum speed if hoid in danger zone
    sf::Vector2f separation(std::vector<Hoid>& flock); // Separation rule (hoids can touch but cannot pass through one another)
    sf::Vector2f towards_exit(std::vector<sf::RectangleShape>& exits); // Move thowards nearest exit rule
    sf::Vector2f avoid_threat(std::vector<sf::CircleShape>& threats); // Avoid threats rule
    void collision(std::vector<sf::RectangleShape>& obstacles); // Cannot pass through obstacles, other hoids or screen edges rule 

public:
    Hoid(sf::Vector2f windowSize);
    sf::CircleShape& getShape() { return hoid; } // Relevant for intersection verification between hoid and exit
    // Hoid spawnning function called when simulation starts
    void spawn(std::vector<Hoid>& flock, std::vector<sf::RectangleShape>& obstacles, sf::Vector2f& windowSize, std::vector<sf::CircleShape>& threats); 
    // Update logic unifies all movement rules in one function
    void update(sf::Vector2f& windowSize, std::vector<Hoid>& flock, std::vector<sf::RectangleShape>& exits, std::vector<sf::CircleShape>& threats, std::vector<sf::RectangleShape>& obstacles);
    void draw(sf::RenderWindow& window);  
};

#endif