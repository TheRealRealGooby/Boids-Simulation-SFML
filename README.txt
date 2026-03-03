# Boids Flocking Simulation

A 2D artificial life program simulating the flocking behavior of birds, built with C++ and the SFML (Simple and Fast Multimedia Library).

This project implements Craig Reynolds' classic 1986 Boids algorithm, demonstrating how complex, organic group behaviors can emerge from simple, localized rules. It serves as a foundational physics and movement engine for modeling crowd dynamics and emergent behavior.
# Core Mechanics

The simulation is driven by three fundamental steering behaviors calculated for each boid every frame:

    Separation: Collision avoidance. Boids steer away from each other if they enter a defined "protected range" to prevent overcrowding and visual glitching.

    Alignment: Velocity matching. Boids adjust their trajectory and speed to match the average heading of their nearby flockmates within their visual range.

    Cohesion: Centering. Boids are gently pulled towards the average center of mass of their local flockmates, keeping the group tightly knit.

Additionally, the boids feature a screen-edge avoidance system (turnf) that gracefully redirects them back into the simulation area when they get too close to the borders.
# Technical Details & Configuration

    Language: C++

    Graphics Library: SFML (Graphics, Window, System modules)

    Framerate: Capped at 60 FPS for stable physics calculations.

Tweaking the "DNA"

The behavior of the entire flock can be drastically altered by tweaking the static constant parameters located in Boid.h:

    avoidf, matchf, centerf: The strength multipliers for the three core rules.

    vrange & prange: The visual and protected radius (in pixels) for neighbor detection.

    maxspeed & minspeed: The speed limits enforced after all vector forces are applied.

# Future Scope: Crowd Dynamics & Evacuation

While currently functioning as a standard avian flocking simulation, this engine is designed to be modular. The base logic (spatial awareness, neighbor detection, and force-based movement) provides the groundwork for simulating human crowd dynamics.

Future iterations will adapt this algorithm into an emergency evacuation simulation, modifying the core boid rules to reflect:

    Panic states and localized bottlenecks.

    Static obstacle avoidance (walls, debris).

    Directional pull towards emergency exits instead of flock cohesion.
