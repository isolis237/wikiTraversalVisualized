#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class TextCircle : public sf::Drawable, public sf::Transformable
{
public:
    TextCircle(const std::string& text, float radius, const sf::Font& font)
        : original_radius(radius)
    {
        // Set up the circle
        circle.setRadius(radius);
        circle.setFillColor(sf::Color::Cyan);
        circle.setOrigin(radius, radius);

        scaled_radius = original_radius * 12.0f;

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
    TextCircle() : TextCircle("", 0.0f, sf::Font()){}

    void algoUpdate() {
        circle.setOrigin(scaled_radius, scaled_radius);
        circle.setRadius(scaled_radius);
        circle.setFillColor(sf::Color::Blue);

        this->text.setCharacterSize(this->text.getCharacterSize() * 2);
        this->text.setScale(2.0f, 2.0f);
        //this->text.setScale(scaled_radius / 10, scaled_radius / 10);
    }

    void update(const float& delta_time, const sf::Vector2f mouse_pos)
{
    // Check if the mouse is over the circle
    if (circle.getGlobalBounds().contains(mouse_pos))
    {
        // Set the origin of the circle to its center
        circle.setOrigin(scaled_radius, scaled_radius);

        // Increase the radius of the circle and set its fill color
        circle.setRadius(scaled_radius);
        circle.setFillColor(sf::Color::Blue);

        // Set the origin of the text to its center
        this->text.setOrigin(this->text.getLocalBounds().width / 2.0f, this->text.getLocalBounds().height / 2.0f);

        // Scale up the text and set its character size
        this->text.setCharacterSize(scaled_radius);
        this->text.setScale(2.0f * (scaled_radius) / this->text.getLocalBounds().width, 1.0f);
    }
    else
    {
        // Reset the radius of the circle and set its position back to its original position
        circle.setRadius(original_radius);
        circle.setOrigin(original_radius, original_radius);

        // Reset the text to its original size and position
        this->text.setCharacterSize(original_radius);
        this->text.setScale(1.0f, 1.0f);
        this->text.setOrigin(this->text.getLocalBounds().width / 2.0f, this->text.getLocalBounds().height / 2.0f);
        circle.setFillColor(sf::Color::Red);
    }
}

void update() {
    circle.setRadius(scaled_radius);
    circle.setFillColor(sf::Color::Green);
    this->text.setCharacterSize(scaled_radius);
    this->text.setScale(2.0f * (scaled_radius) / this->text.getLocalBounds().width, 1.0f);
}


    void addNeighbor(TextCircle* neighbor) {
        neighbors.push_back(neighbor);
    }

    vector<TextCircle*> getNeighbors() {
        return neighbors;
    }

    void setPosition(float x, float y)
    {
        // Set the position of the circle and the text
        circle.setPosition(x, y);
        text.setPosition(x, y);
        this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2);
    }

    sf::Vector2f getPosition() const
    {
        // Return the position of the circle
        return circle.getPosition();
    }

    std::string getText() {
        return text.getString();
    }

    void setColor(sf::Color color) {
        circle.setFillColor(color);
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
    float scaled_radius = original_radius * 12.0f;
    vector<TextCircle*> neighbors;
};

