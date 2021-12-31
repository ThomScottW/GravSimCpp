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

    Sim(unsigned numParticles);

    // Destructor.
    ~Sim();

    // Runs an SDL game loop.
    int run();


private:
    // Return true if SDL video elements are initialized successfully.
    bool Init();

    // Handle any SDL_Event.
    void handleEvent(SDL_Event* Event);

    void drawSDLCircle(std::vector<double> pos, double radius);

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
    
};



#endif