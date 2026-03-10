#include "Hoid.h" /* Makefile will search the "include" folder by default
                    (-I flag) so the complete path isn't necessary */ 
#include <cmath>
#include <cstdlib>
#include <ctime>

// Constructor
Hoid::Hoid(sf::Vector2f windowSize) 
{
    hoid.setRadius(4);
    hoid.setFillColor(sf::Color::Blue);
    hoid.setOutlineColor(sf::Color::White);
    hoid.setOutlineThickness(1);
    hoid.setOrigin(0, 2);
}

// Spawn function (cannot spawn hoids over other hoids or obstacles)
void Hoid::spawn(std::vector<Hoid>& flock, std::vector<sf::RectangleShape>& obstacles, sf::Vector2f& windowSize, std::vector<sf::CircleShape>& threats)
{
    bool correctly_spawned = false;
    bool safe_position;
    int height = windowSize.y;
    int width = windowSize.x;
    int distance;
    while (!correctly_spawned)
    {
        // Generate random position
        hoid.setPosition({static_cast<float>(std::rand() % (width-15)), static_cast<float>(std::rand() % (height-15))});
        safe_position = true;
        for(auto& h : flock)
        {
            // Check if current hoid intersects other hoid
            distance = (hoid.getPosition().x - h.hoid.getPosition().x)*(hoid.getPosition().x - h.hoid.getPosition().x)+(hoid.getPosition().y - h.hoid.getPosition().y)*(hoid.getPosition().y - h.hoid.getPosition().y);
            if(distance <= 64)
            {
                safe_position = false;
                break;
            }
        }
        // If current hoid does not intersect other hoid check if it intersects obstacles
        if(safe_position)
            for(auto& o : obstacles)
                if (hoid.getGlobalBounds().intersects(o.getGlobalBounds()))
                {
                    safe_position = false;
                    break;
                }
        // If current hoid does not intersect obstacles check if it intersects threats
        if(safe_position)
            for(auto& t : threats)
                if (hoid.getGlobalBounds().intersects(t.getGlobalBounds()))
                {
                    safe_position = false;
                    break;
                }
        if(safe_position) correctly_spawned = true;
    }
}

// Separation rule is milder for hoids than for boids
sf::Vector2f Hoid::separation(std::vector<Hoid>& flock)
{
    float dist;
    sf::Vector2f close(0.f, 0.f);
    for(const Hoid& h : flock)
    {
        // Calculate the distance between curent hoid and neighbouring hoid
        dist = (hoid.getPosition().x-h.hoid.getPosition().x)*(hoid.getPosition().x-h.hoid.getPosition().x) + (hoid.getPosition().y-h.hoid.getPosition().y)*(hoid.getPosition().y-h.hoid.getPosition().y);
        // If neighbouring hoid in protected range change close vector (will be added to velocity)
        if(dist <= 400 && &h!=this)
        {
            dist=std::sqrt(dist);
            // Normalising close vector to have constant separation force for all hoids
            close.x += (hoid.getPosition().x - h.hoid.getPosition().x)/dist;
            close.y += (hoid.getPosition().y - h.hoid.getPosition().y)/dist;
        }
    }
    close *= 1.5f;
    return close;
}

// Move towards exit rule implementation
sf::Vector2f Hoid::towards_exit(std::vector<sf::RectangleShape>& exits)
{
    sf::Vector2f direction (100000.f, 100000.f);
    float dist, norm;
    // Calculate closest exit and get direction towards it
    for(auto& e : exits)
    {
        dist = (hoid.getPosition().x-e.getPosition().x)*(hoid.getPosition().x-e.getPosition().x) + (hoid.getPosition().y-e.getPosition().y)*(hoid.getPosition().y-e.getPosition().y);
        if(dist < direction.x*direction.x + direction.y*direction.y)
            direction = e.getPosition() - hoid.getPosition(); 
    }
    // Normalize direction vector and apply exit factor to it
    norm = std::sqrt(direction.x* direction.x + direction.y* direction.y);
    direction /= norm;
    direction *= exitf;
    return direction;
}

// Avoid threats rule implementation
sf::Vector2f Hoid::avoid_threat(std::vector<sf::CircleShape>& threats)
{
    sf::Vector2f direction, total(0.f, 0.f);
    float dist;
    for(auto& t : threats)
    {
        dist = std::sqrt((hoid.getPosition().x-t.getPosition().x)*(hoid.getPosition().x-t.getPosition().x) + (hoid.getPosition().y-t.getPosition().y)*(hoid.getPosition().y-t.getPosition().y));
        if(dist < 175) // If hoid in danger zone calculate new direction for hoid away from threat
        {
            direction = t.getPosition() - hoid.getPosition(); 
            direction /= dist;
            total += direction*(175 - dist)*threatf; // The closer it is to threat the greater the speed
        }
    }
    return total;
}
// Collision rule implementation
void Hoid::collision(std::vector<sf::RectangleShape>& obstacles)
{
    sf::FloatRect hbounds = hoid.getGlobalBounds(); // Hoid bounds used to check if boid will intersect with obstacle
    sf::FloatRect obounds;
    sf::FloatRect nextX, nextY;
    for (auto& o : obstacles)
    {
        // If on next step hoid intersects obstacle, x/y component of speed becomes 0
        obounds = o.getGlobalBounds();
        nextX = hbounds;
        nextX.left += velocity.x;
        if(nextX.intersects(obounds)) velocity.x = 0;
        nextY = hbounds;
        nextY.top += velocity.y;
        if(nextY.intersects(obounds)) velocity.y = 0;
    }
}

// Movement logic
void Hoid::update(sf::Vector2f& windowSize, std::vector<Hoid>& flock, std::vector<sf::RectangleShape>& exits, std::vector<sf::CircleShape>& threats, std::vector<sf::RectangleShape>& obstacles) {
    sf::Vector2f exit_force = towards_exit(exits);
    sf::Vector2f threat_force = avoid_threat(threats);
    sf::Vector2f separation_force = separation(flock);
    // Set hoid's personality
    float random_x = ((std::rand() % 100) / 100.f) * 0.4f - 0.2f;
    float random_y = ((std::rand() % 100) / 100.f) * 0.4f - 0.2f;
    personal_speed = ((std::rand() % 100) / 100.f) * 0.1f + 0.7f;
    human_error = {random_x, random_y};
    // Calculate new velocity and speed
    velocity += human_error + threat_force + exit_force + separation_force;
    speed = std::sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
    // Hoid maximum speed is greater if hoid in a danger zone
    if(threat_force.x*threat_force.x + threat_force.y*threat_force.y != 0.f)
    {
        if(speed > panic_max_speed)
            velocity = velocity/speed*panic_max_speed;
    }
    else
        if(speed > base_max_speed)
            velocity *= breakf;
    // Screen edges are hard walls
    if(hoid.getPosition().x + velocity.x <= 0.f || hoid.getPosition().x + velocity.x >= windowSize.x) 
        velocity.x = 0.f;

    if(hoid.getPosition().y + velocity.y <= 0.f || hoid.getPosition().y + velocity.y >= windowSize.y) 
        velocity.y = 0.f;
    // Add personal speed and collision rule to velocity
    collision(obstacles);
    velocity *= personal_speed;
    hoid.move(velocity);
}

// Drawing the hoid
void Hoid::draw(sf::RenderWindow& window) {
    window.draw(hoid);
}