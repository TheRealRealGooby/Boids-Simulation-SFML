Boids and Dumb Hoids: Flock and Crowd Evacuation Simulators

Compilation and Execution
Prerequisites

To compile and execute these simulations, the following dependencies must be installed on the host system:

    g++ Compiler: Required for compiling C++ source code.

    SFML (Simple and Fast Multimedia Library): Required for window creation, graphics rendering, and system events. The Makefile explicitly links -lsfml-graphics, -lsfml-window, and -lsfml-system.

Build Instructions

The project includes a Makefile configured to automate the build process for both simulation architectures. Open a terminal in the repository root and execute the following commands:

    Compile Both Simulations:
    Execute the standard make or make all command. This will generate both the simulare_boid and simulare_hoid executables.

    Compile Boids Only:
    Execute make simulare_boid.

    Compile Hoids Only:
    Execute make simulare_hoid.

Execution

Once compiled, run the generated binaries directly from the terminal:

    To start the avian flocking simulation: ./simulare_boid

    To start the humanoid evacuation simulation: ./simulare_hoid

Cleanup

To reset the directory state and remove all generated object files (.o), dependency files (.d), and executables, run the following command: make clean

INTORDUCTION

This project implements two distinct entity-based movement simulations. The first is an implementation of the elegant Boids algorithm, conceptualized by Craig Reynolds in 1986. It mathematically simulates the organic flight patterns of a bird flock (boids, standing for "bird-oid objects") governed by three fundamental rules: Separation, Alignment, and Cohesion.

The second component is an evacuation simulation modeling a crowd of "dumb hoids" (humanoid objects) attempting to escape a room. The environment features multiple exits, physical obstacles, and active threats. The reasoning behind the "dumb" classification is detailed in the drawbacks section below.

THE BOIDS ALGORITHM

The simulation initializes with each boid assigned a random spatial coordinate and trajectory vector. Rather than following a centralized command, the boids organically group into increasingly larger and synchronized flocks by strictly adhering to three localized conceptual rules:

    Separation: Entities steer to avoid crowding local flockmates. This creates a spatial buffer, preventing mid-air collisions and maintaining physical realism.

    Alignment: Entities steer towards the average heading of local flockmates. This synchronizes their movement, allowing the group to travel in a unified direction.

    Cohesion: Entities steer to move toward the average position of local flockmates. This creates a center of mass attraction, keeping the flock tightly knit and preventing individuals from drifting away.

THE DUMB HOIDS ALGORITHM

Unlike the continuous flight of boids, the hoids simulation requires an initial environment setup phase. Users can generate a custom layout using the following inputs:

    Right Click: Spawns Exits (represented by green rectangles).

    Left Click: Spawns Obstacles (represented by yellow squares).

    Press 'T': Spawns Threats (represented by red circles).

Pressing the Enter key terminates the setup phase and triggers the simulation. Hoids spawn into the environment and immediately begin calculating their trajectories based on four primary rules:

    Separation: A milder implementation of the boids' separation rule. It prevents humanoids from completely overlapping while allowing them to form dense, high-pressure crowds.

    Attraction: Hoids continuously calculate the distance to all exits and apply a steering force toward the nearest one.

    Threat Avoidance: Hoids calculate repulsive vectors to steer away from the defined threat zones.

    Obstacle Collision: Hoids interact with physical barriers, zeroing out their velocity on intersecting axes to prevent phasing through walls.

!!! DRAWBACKS (Why they are "Dumb Hoids")

The current evacuation model is highly mechanical and presents significant opportunities for optimization. The behavioral limitations justify the "dumb hoids" designation:

    Unnatural Physics: The obstacle avoidance and collision responses are rigid. They lack predictive pathfinding or smooth, organic cornering.

    The Local Minima Bug: The most critical flaw is the lack of high-level cognitive routing. If a hoid's nearest targeted exit is entirely obstructed by a physical obstacle or a threat zone, the entity will endlessly attempt to reach it. They do not possess the logic to abandon a blocked door and reroute to a secondary, accessible exit. They remain permanently trapped in a mathematical loop of attraction and repulsion.

CONCLUSION

The evacuation simulation can be significantly improved by implementing cognitive decision-making nodes, short-term memory, and more unpredictable, human-like responses to panic. However, the core architectural concept remains valid: steering a massive crowd of independent entities using a localized, predefined set of rules—as pioneered by the 1986 Boids algorithm—is as applicable to modeling human crowd dynamics as it is to avian flocks.
