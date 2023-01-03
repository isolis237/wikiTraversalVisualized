#include <SFML/Graphics.hpp>
#include <iostream>

class TextCircle : public sf::Drawable, public sf::Transformable
{
public:
    TextCircle(const std::string& text, float radius, const sf::Font& font)
        : original_radius(radius)
    {
        // Set up the circle
        circle.setRadius(radius);
        circle.setFillColor(sf::Color::Red);
        circle.setOrigin(radius, radius);

        // Set up the text
        this->text.setString(text);
        this->text.setFont(font);
        this->text.setCharacterSize(static_cast<unsigned int>(radius));
        this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2);
        this->text.setFillColor(sf::Color::Black);

        // Scale down the text if it is too large to fit within the circle
        if (this->text.getLocalBounds().width > 2.0f * radius)
        {
            this->text.setScale(2.0f * radius / this->text.getLocalBounds().width, 1.0f);
        }
    }

    void update(const float& delta_time, const sf::Vector2f mouse_pos)
    {

        // Check if the mouse is over the circle
        if (circle.getGlobalBounds().contains(mouse_pos))
        {
            // Increase the radius of the circle
            circle.setRadius(original_radius * 12.0f);
            this->text.setCharacterSize(original_radius * 10.0f);
            this->text.setOrigin(circle.getOrigin());
            circle.setFillColor(sf::Color::Blue);
        }
        else
        {
            // Reset the radius of the circle
            circle.setRadius(original_radius);
            this->text.setCharacterSize(original_radius);
            circle.setFillColor(sf::Color::Red);
        }
    }

    void setPosition(float x, float y)
    {
        // Set the position of the circle and the text
        circle.setPosition(x, y);
        text.setPosition(x, y);
    }

    sf::Vector2f getPosition() const
    {
        // Return the position of the circle
        return circle.getPosition();
    }

    std::string getText() {
        return text.getString();
    }



private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Draw the circle and the text
        target.draw(circle, states);
        target.draw(text, states);
    }

    sf::CircleShape circle;
    sf::Text text;
    float original_radius;
};

