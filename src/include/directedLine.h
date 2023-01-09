#include <SFML/Graphics.hpp>

class DirectedLine : public sf::Drawable
{
public:
    DirectedLine(sf::Vector2f start, sf::Vector2f end)
    {
        // Create the line with the start and end points and set its color to blue
        m_line = sf::VertexArray(sf::LinesStrip, 2);
        m_line[0].position = start;
        m_line[1].position = end;
        m_line[0].color = sf::Color::Blue;
        m_line[1].color = sf::Color::Red;
    }

    void algoUpdate()
    {
        m_line[0].color = sf::Color::Green;
        m_line[1].color = sf::Color::Green;
    }

    void update() {
        m_line[0].color = sf::Color::Blue;
        m_line[1].color = sf::Color::Red;
    }

private:
    sf::VertexArray m_line;

    // Implement the draw function inherited from sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Draw the line
        target.draw(m_line, states);
    }
};