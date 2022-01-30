#include "Sim.hpp"


Sim::Sim(unsigned numParticles)
    : win{nullptr},
    winSurface{nullptr},
    ren{nullptr},
    running{true},
    ghostX{-1},
    ghostY{-1},
    ghostRad{5},
    showGhostParticle{false}
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


void Sim::drawOctantLines(double xc, double yc, double x, double y)
{
    // I found that drawing the lines horizontally worked better.
    // There were less "gaps". And through testing it seems there's no need
    // to draw the points on the edge of the circle this way.

    // Lines between 2nd and 3rd octant.
    SDL_RenderDrawLine(ren, xc + x, yc - y, xc - x, yc - y);
    // Lines between 1st and 4th octant.
    SDL_RenderDrawLine(ren, xc + y, yc - x, xc - y, yc - x);
    // Lines between 8th and 5th octant.
    SDL_RenderDrawLine(ren, xc + y, yc + x, xc - y, yc + x);
    // Lines between 7th and 6th octant.
    SDL_RenderDrawLine(ren, xc + x, yc + y, xc - x, yc + y);

    // Draws points on the edge of the circle.
    // SDL_RenderDrawPoint(ren, xc + x, yc + y);
    // SDL_RenderDrawPoint(ren, xc - x, yc + y);
    // SDL_RenderDrawPoint(ren, xc + x, yc - y);
    // SDL_RenderDrawPoint(ren, xc - x, yc - y);
    // SDL_RenderDrawPoint(ren, xc + y, yc + x);
    // SDL_RenderDrawPoint(ren, xc - y, yc + x);
    // SDL_RenderDrawPoint(ren, xc + y, yc - x);
    // SDL_RenderDrawPoint(ren, xc - y, yc - x);
    
    // // Draws lines between the 1st and 5th octant.
    // SDL_RenderDrawLine(ren, xc + y, yc - x, xc - y, yc + x);
    // // Draws lines between the 2nd and 6th octant.
    // SDL_RenderDrawLine(ren, xc + x, yc - y, xc - x, yc + y);
    // // Draws lines between the 3rd and 7th octant.
    // SDL_RenderDrawLine(ren, xc - x, yc - y, xc + x, yc + y);
    // // Draws lines between the 4th and 8th octant.
    // SDL_RenderDrawLine(ren, xc - y, yc - x, xc + y, yc + x);  
}


void Sim::drawSDLCircle(double h, double k, double radius, int r, int g, int b)
{
    SDL_SetRenderDrawColor(ren, r, g, b, 255);

    double x = 0;
    double y = radius;
    double d = 3 - 2 * radius;

    drawOctantLines(h, k, x, y);

    // This is bresenham's circle drawing algorithm. We draw the 2nd octant
    // of the circle. The other octants can be inferred from this one.
    while (y > x)
    {
        // Decide whether y will stay the same, or decrease.
        if (d <= 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            --y;
        }
        ++x;

        drawOctantLines(h, k, x, y);
    }
}


void Sim::drawParticles()
{
    for (Particle p : env.getParticles())
    {
        drawSDLCircle(p.x(), p.y(), p.getRadius());
    }

    if (showGhostParticle)
    {
        drawSDLCircle(ghostX, ghostY, ghostRad, 100, 100, 100);
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
            else if (Event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (Event.button.button == SDL_BUTTON_LEFT)
                {
                    showGhostParticle = true;
                }
            }
            else if (Event.type == SDL_MOUSEBUTTONUP)
            {
                if (Event.button.button == SDL_BUTTON_LEFT)
                {
                    showGhostParticle = false;
                }
                else if (Event.button.button == SDL_BUTTON_RIGHT && showGhostParticle)
                {
                    MotionVector<double> newMot = MotionVector<double>(0, 0);
                    env.placeParticle(Particle(ghostRad, ghostX, ghostY, std::pow(ghostRad, 2), newMot));
                }
            }
            else if (Event.type == SDL_MOUSEMOTION)
            {
                // Get the x and y coordinates of the mouse.
                ghostX = Event.button.x;
                ghostY = Event.button.y;
            }
            else if (Event.type == SDL_MOUSEWHEEL)
            {
                if (Event.wheel.y > 0)
                {
                    ++ghostRad;
                }
                else if (Event.wheel.y < 0)
                {
                    --ghostRad;
                }
                
            }
            
        }

        env.update();

        drawScreen();

        // SDL_Delay(100);
    }
    return 0;
}
