#include "Boid.h" /* Makefile will search the "include" folder by default
                    (-I flag) so the complete path isn't necessary */ 
#include <cmath>
#include <cstdlib>
#include <ctime>

// Constructor
Boid::Boid(sf::Vector2f windowSize) {
    int h = windowSize.y;
    int w = windowSize.x;


    // Triangle shape of the boid
    boid.setPointCount(3);
    boid.setPoint(0, sf::Vector2f(0,0));
    boid.setPoint(1, sf::Vector2f(0,4));
    boid.setPoint(2, sf::Vector2f(8,2));
    boid.setFillColor(sf::Color::Yellow);
    boid.setOutlineColor(sf::Color::White);
    boid.setOutlineThickness(1);
    boid.setOrigin(0, 2);
    boid.setFillColor(sf::Color::Yellow);

    // Random starting rotation & position
    boid.setRotation(static_cast<float>(std::rand() % 360));
    boid.setPosition({static_cast<float>(std::rand() % (w-10)), static_cast<float>(std::rand() % (h-10))});
    velocity = sf::Vector2(std::cos(boid.getRotation()*3.14159265f / 180.f), std::sin(boid.getRotation()*3.14159265f / 180.f));

}

// Separation rule
sf::Vector2f Boid::separation(std::vector<Boid>& flock)
{
    float dist;
    sf::Vector2f close(0.f, 0.f);
    for(const Boid& b : flock)
    {
        // Calculate the distance between curent boid and neighbouring boid
        dist = (boid.getPosition().x-b.boid.getPosition().x)*(boid.getPosition().x-b.boid.getPosition().x) + (boid.getPosition().y-b.boid.getPosition().y)*(boid.getPosition().y-b.boid.getPosition().y);
        // If neighbouring boid in protected range change close vector (will be added to velocity)
        if(dist < prange*prange && &b!=this)
        {
            dist=std::sqrt(dist);
            // Normalising close vector to have constant separation force for all boids
            close.x += (boid.getPosition().x - b.boid.getPosition().x)/dist;
            close.y += (boid.getPosition().y - b.boid.getPosition().y)/dist;
        }
    }
    close *= avoidf;
    return close;
}

// Alignemnt rule
sf::Vector2f Boid::alignment(std::vector<Boid>& flock)
{
    float dist;
    sf::Vector2 avgv(0.f, 0.f); // Average velocity of neighbours
    int n = 0; // Number of neighbours
    for(const Boid& b : flock)
    {
        dist = (boid.getPosition().x-b.boid.getPosition().x)*(boid.getPosition().x-b.boid.getPosition().x) + (boid.getPosition().y-b.boid.getPosition().y)*(boid.getPosition().y-b.boid.getPosition().y);
        // If other boid in current boid visual range and is not current boid then other boid is neighbour
        // Calculate average velocity
        if(dist <= vrange*vrange && &b!=this)
        {
            avgv.x += b.velocity.x;
            avgv.y += b.velocity.y;
            n++;
        }
    }
    // Calculate average velocity considering current boid position and matching factor
    if(n)
    {
        avgv.x = (avgv.x/n - velocity.x)*matchf;
        avgv.y = (avgv.y/n - velocity.y)*matchf;
    }
    return avgv;
}

// Cohesion rule
sf::Vector2f Boid::cohesion(std::vector<Boid>& flock)
{
    float dist;
    sf::Vector2 avgp(0.f, 0.f); // Average position of neighbours
    int n = 0; // Number of neighbours
    for(const Boid& b : flock)
    {
        dist = (boid.getPosition().x-b.boid.getPosition().x)*(boid.getPosition().x-b.boid.getPosition().x) + (boid.getPosition().y-b.boid.getPosition().y)*(boid.getPosition().y-b.boid.getPosition().y);
        // Calculate average position
        if(dist <= vrange*vrange && &b!=this)
        {
            avgp.x += b.boid.getPosition().x;
            avgp.y += b.boid.getPosition().y;
            n++;
        }
    }
    // Calculate average position considering current boid position and centering factor
    if(n)
    {
        avgp.x = (avgp.x/n - boid.getPosition().x)*centerf;
        avgp.y = (avgp.y/n - boid.getPosition().y)*centerf;
    }
    return avgp;
}

// Movement logic
void Boid::update(sf::Vector2f windowSize, std::vector<Boid>& flock) {
    int h = windowSize.y;
    int w = windowSize.x;
    sf::Vector2f pos = boid.getPosition();  

    velocity.x += separation(flock).x + alignment(flock).x + cohesion(flock).x;
    velocity.y += separation(flock).y + alignment(flock).y + cohesion(flock).y;

    // Limiting boid's movement to edges of screen
    if(pos.x >= w-50)
    {
        velocity.x -= turnf;
    }
    if(pos.x <= 50)
    {
        velocity.x += turnf;
    }
    if(pos.y >= h-50)
    {
        velocity.y -= turnf;
    }
    if(pos.y <= 50)
    {
        velocity.y += turnf;
    }

    float speed = std::sqrt(velocity.x*velocity.x + velocity.y*velocity.y);

    // Limit speed to minimum speed and maximum speed
    if(speed > maxspeed)
        velocity = velocity/speed*maxspeed;

    if(speed < minspeed)
        velocity = velocity/speed*minspeed;

    // Obtaining new trajectory angle
    float angleRad = std::atan2(velocity.y, velocity.x);
    
    // Conversion from radians to degrees
    float angleDeg = angleRad * 180.f / 3.14159265f;
    
    // Updating boid's rotation to new trajectory
    boid.setRotation(angleDeg);

    // Updating boid's trajectory to new velocity
    boid.move(velocity);
}

// Drawing the boid
void Boid::draw(sf::RenderWindow& window) {
    window.draw(boid);
}