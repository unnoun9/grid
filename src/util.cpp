#include "util.h"
#include "globals.h"

// just draws a line from p1 to p2 of given color
void util::drawline(vec2 p1, vec2 p2, sf::Color color)
{
    sf::Vertex line[] = {
        sf::Vertex(vec2(p1.x, p1.y), color),
        sf::Vertex(vec2(p2.x, p2.y), color)
    };
    window.draw(line, 2, sf::Lines);   
}

// just quits the main loop
void util::quit()
{
    running = false;
    window.close();
}