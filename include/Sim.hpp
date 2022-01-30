#ifndef SIM_HPP
#define SIM_HPP


#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include "Particle.hpp"
#include "Environment.hpp"


class Sim
{
public:
    // Constructor.
    Sim();

    Sim(unsigned numParticles=10);

    // Destructor.
    ~Sim();

    // Runs an SDL game loop.
    int run();


private:
    // Return true if SDL video elements are initialized successfully.
    bool Init();

    // Handle any SDL_Event.
    void handleEvent(SDL_Event* Event);

    // Draw an SDL circle at the given x and y coordinates, with a 
    // specified radius and opacity.
    void drawSDLCircle(double x, double y, double radius, int r=255, int g=255, int b=255);

    // Given points xc and yc for the center of a circle, and points x y
    // on the edge of the circle, draw the corresponding points in all 8 octants,
    // and lines between them.
    void drawOctantLines(double xc, double yc, double x, double y);

    // Draw all the particles to the screen.
    void drawParticles();

    // Draw the screen.
    void drawScreen();

    // Generate a random Particle.
    Particle genParticle();
    
    SDL_Window* win;
    SDL_Surface* winSurface;
    SDL_Renderer* ren;
    Environment env;

    bool running;

    // For ghost particles.
    double ghostX;
    double ghostY;
    double ghostRad;
    bool showGhostParticle;
};


#endif
