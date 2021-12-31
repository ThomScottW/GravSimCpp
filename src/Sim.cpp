#include "Sim.hpp"


Sim::Sim()
    : win{nullptr}, winSurface{nullptr}, ren{nullptr}, running{true}
{
    env = Environment();
}

Sim::Sim(unsigned numParticles)
    : win{nullptr}, winSurface{nullptr}, ren{nullptr}, running{true}
{
    env = Environment(numParticles);
}


Sim::~Sim()
{
    if (ren)
    {
        SDL_DestroyRenderer(ren);
        ren = nullptr;
    }
    
    if (ren)
    {
        SDL_DestroyWindow(win);
        win = nullptr;
    }

    SDL_Quit();
}


bool Sim::Init()
{
    // SDL_Init will return -1 on error.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Video Initialization Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window.
    win = SDL_CreateWindow(
        "Gravity Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        env.dimensions().at(0),
        env.dimensions().at(1),
        SDL_WINDOW_SHOWN // Not sure what this one does? Can use | to add more flags.
    );

    if (nullptr == win)
    {
        std::cout << "Window Creation Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Init Surface.
    winSurface = SDL_GetWindowSurface(win);

    // Create renderer.
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    if (nullptr == ren)
    {
        std::cout << "Renderer Creation Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    return true;
}


void Sim::drawSDLCircle(std::vector<double> pos, double radius)
{
    // Set the color of the circles.
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    // Circle Equation: (x-h)^2 + (y-k)^2 = r^2
    double xTerm;
    double yTerm;
    double h = pos.at(0);  // Center x.
    double k = pos.at(1);  // Center y.


    for (double x = h - radius; x < h + radius; ++x)
    {
        for (double y = k - radius; y < k + radius; ++y)
        {
            xTerm = (x - h) * (x - h);
            yTerm = (y - k) * (y - k);

            // Only draw a point if it is within the circle.
            if (xTerm + yTerm <= radius * radius)
            {
                SDL_RenderDrawPoint(ren, x, y);
            } 
        }
    }
}


void Sim::drawParticles()
{
    for (Particle p : env.getParticles())
    {
        drawSDLCircle(p.coordinates(), p.getRadius());
    }
}


void Sim::handleEvent(SDL_Event* Event){
    return;
}


void Sim::drawScreen()
{
    // SDL_RenderClear fills the entire screen with the color set by
    /// SDL_SetRenderDrawColor.
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    // Draw the particles on top of that color.
    drawParticles();

    // Then present the completed frame.
    SDL_RenderPresent(ren);
}


int Sim::run()
{
    if (Init() == false)
    {
        return 1;
    }

    SDL_Event Event;

    while (running)
    {
        while (SDL_PollEvent(&Event) != 0)
        {
            handleEvent(&Event);

            if (Event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        env.update();

        drawScreen();

        // SDL_Delay(100);
    }

    return 0;
}