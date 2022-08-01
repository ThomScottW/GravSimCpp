#include "Sim.hpp"


namespace
{
    double const PI = std::atan(1) * 4;
}


Sim::Sim(unsigned numParticles)
    : win{nullptr},
    winSurface{nullptr},
    ren{nullptr},
    running{true},
    mouseX{-1},
    mouseY{-1},
    ghostRad{5},
    showGhostParticle{false},
    fontSize{10},
    frozenP{nullptr},
    choosingOrbit{false}
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
    
    if (win)
    {
        SDL_DestroyWindow(win);
        win = nullptr;
    }

    TTF_Quit();
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
        SDL_WINDOW_SHOWN
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
        std::cerr << "Renderer Creation Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    // Init text.
    if (TTF_Init() < 0)
    {
        std::cerr << "Failed to initialize SDL_TTF: " << TTF_GetError() << std::endl;
    }

    return true;
}


void Sim::drawCirclePixels(double xc, double yc, double x, double y, bool filled)
{
    // I found that drawing the lines horizontally worked better.
    // There were less "gaps". And through testing it seems there's no need
    // to draw the points on the edge of the circle this way.

    if (filled)
    {
        // Lines between 2nd and 3rd octant.
        SDL_RenderDrawLine(ren, xc + x, yc - y, xc - x, yc - y);
        // Lines between 1st and 4th octant.
        SDL_RenderDrawLine(ren, xc + y, yc - x, xc - y, yc - x);
        // Lines between 8th and 5th octant.
        SDL_RenderDrawLine(ren, xc + y, yc + x, xc - y, yc + x);
        // Lines between 7th and 6th octant.
        SDL_RenderDrawLine(ren, xc + x, yc + y, xc - x, yc + y);
    }
    else
    {
        // Draws points on the edge of the circle.
        SDL_RenderDrawPoint(ren, xc + x, yc + y);
        SDL_RenderDrawPoint(ren, xc - x, yc + y);
        SDL_RenderDrawPoint(ren, xc + x, yc - y);
        SDL_RenderDrawPoint(ren, xc - x, yc - y);
        SDL_RenderDrawPoint(ren, xc + y, yc + x);
        SDL_RenderDrawPoint(ren, xc - y, yc + x);
        SDL_RenderDrawPoint(ren, xc + y, yc - x);
        SDL_RenderDrawPoint(ren, xc - y, yc - x);
    }
    // // Draws lines between the 1st and 5th octant.
    // SDL_RenderDrawLine(ren, xc + y, yc - x, xc - y, yc + x);
    // // Draws lines between the 2nd and 6th octant.
    // SDL_RenderDrawLine(ren, xc + x, yc - y, xc - x, yc + y);
    // // Draws lines between the 3rd and 7th octant.
    // SDL_RenderDrawLine(ren, xc - x, yc - y, xc + x, yc + y);
    // // Draws lines between the 4th and 8th octant.
    // SDL_RenderDrawLine(ren, xc - y, yc - x, xc + y, yc + x);  
}


void Sim::drawSDLCircle(double h, double k, double radius, bool filled, SDL_Color color)
{
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 255);

    double x = 0;
    double y = radius;
    double d = 3 - 2 * radius;

    drawCirclePixels(h, k, x, y, filled);

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

        drawCirclePixels(h, k, x, y, filled);
    }
}


void Sim::drawParticles()
{
    for (Particle* p : env.getParticles())
    {
        drawSDLCircle(p->x(), p->y(), p->getRadius(), true, p->getColor());
    }

    if (showGhostParticle || choosingOrbit)
    {
        drawSDLCircle(mouseX, mouseY, ghostRad, true, SDL_Color{100, 100, 100});
        addText(std::to_string((int)ghostRad), mouseX, mouseY - 30);
    }

    if (choosingOrbit)
    {
        // Draw a circle, centered at the orbitCenter particle, and that extends to
        // the mouse cursor.
        drawSDLCircle(
            (*orbitCenter).x(),
            (*orbitCenter).y(),
            std::hypot(mouseX - (*orbitCenter).x(), mouseY - (*orbitCenter).y()),
            false,
            SDL_Color{100, 100, 100}
        );
    }
}


void Sim::addText(std::string text, int x, int y)
{
    Text t{text, x, y};
    texts.push_back(t);
}


// Draw text to the screen.
void Sim::drawText(const char* text, int x, int y) const
{
    TTF_Font* font = TTF_OpenFont("/home/scotttw/Projects/GravSim/grav.ttf", 20);

    if (!font)
    {
        std::cout << font << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, {255, 255, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;

    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);

    SDL_RenderCopy(ren, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}


void Sim::drawScreen()
{
    // SDL_RenderClear fills the entire screen with the color set by
    /// SDL_SetRenderDrawColor.
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    // Draw the particles on top of that color.
    drawParticles();

    // Draw text.
    for (auto it = texts.begin(); it != texts.end();)
    {
        // The SDL function to render text requires a const char*.
        // The .c_str() method converts from std::string to const char*.
        drawText((*it).t.c_str(), (*it).x, (*it).y);
        it = texts.erase(it);
    }

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
            if (Event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (Event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (Event.button.button == SDL_BUTTON_LEFT && !showGhostParticle)
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
                    env.placeParticle(new Particle(ghostRad, mouseX, mouseY, newMot));
                }
            }
            else if (Event.type == SDL_MOUSEMOTION)
            {
                // Get the x and y coordinates of the mouse.
                mouseX = Event.button.x;
                mouseY = Event.button.y;
            }
            else if (Event.type == SDL_MOUSEWHEEL)
            {
                if (Event.wheel.y > 0)
                {
                    ++ghostRad;
                }
                else if (Event.wheel.y < 0)
                {
                    ghostRad = ghostRad > 1 ? ghostRad - 1 : 1;
                }
            }
            else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_SPACE)
            {
                // Search if there is a particle near the mouse cursor.
                if ( (frozenP = env.findParticle(mouseX, mouseY)) != nullptr )
                {
                    if ( (*frozenP).isFrozen() )
                    {
                        (*frozenP).unFreeze();
                    }
                    else
                    {
                        (*frozenP).freeze();
                    }
                }
            }
            else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_o && !choosingOrbit)
            {
                // See if we clicked on a particle.
                if ( (orbitCenter = env.findParticle(mouseX, mouseY)) != nullptr )
                {
                    std::cout << "Orbit chosen" << std::endl;
                    choosingOrbit = true;
                }
            }
            else if (Event.type == SDL_KEYUP && Event.key.keysym.sym == SDLK_o && choosingOrbit)
            {
                std::cout << "Stopped choosing orbit" << std::endl;
                choosingOrbit = false;

                double ocX = (*orbitCenter).x();
                double ocY = (*orbitCenter).y();

                // Find the angle between the mouse cursor and the orbitCenter.
                double orbitAngle = std::atan2(ocY - mouseY, ocX - mouseX);

                // Subtract 90 degrees.
                orbitAngle = std::fmod(orbitAngle - 0.5 * M_PI, 2 * M_PI);

                // Distance between mouse and orbitCenter.
                double distBetweenBodies = std::hypot(mouseX - ocX, mouseY - ocY);

                std::cout << "Choosing particle with radius " << ghostRad << " meters and mass " << Particle::calcMass(ghostRad, 1) << "kg" << std::endl;

                // Calculate the necessary velocity.
                double orbitalVelocity = std::sqrt( 
                    (GRAVITATIONAL_CONSTANT * (*orbitCenter).getMass()) / distBetweenBodies );

                // Create a MotionVector for the new particle.
                MotionVector<double> newMot = MotionVector<double>(
                    orbitalVelocity * std::cos(orbitAngle),
                    orbitalVelocity * std::sin(orbitAngle)
                );

                // Place the particle.
                env.placeParticle(new Particle(
                    ghostRad,
                    mouseX, mouseY,
                    newMot + (*orbitCenter).getVelocity()
                ));
            }
            else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_a)
            {
                // Place an attacker, false sets gravity to be off.
                env.placeParticle(new Attacker(mouseX, mouseY));
            }
        }

        env.update();

        drawScreen();

        SDL_Delay(16);
    }
    return 0;
}
