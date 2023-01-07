#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>

#include "include/digraph.h"
#include "include/dijkstra.h"
#include "include/dfs.h"
#include "include/TextCircle.h"
#include "include/directedLine.h"


/** display canvas being colored in by segments
int main() {
  // Create a window with a specified size and title
  sf::RenderWindow window(sf::VideoMode(800, 600), "Blank Canvas");

  // Calculate the dimensions of the border and segments
  int borderSize = window.getSize().x * 0.15; // 5% of the width of the window
  int numSegmentsX = 3; // 5 segments in the width
  int numSegmentsY = 3; // 5 segments in the height
  int segmentWidth = (window.getSize().x - borderSize * 2) / numSegmentsX;
  int segmentHeight = (window.getSize().y - borderSize * 2) / numSegmentsY;
  int borderThickness = 1; // Thickness of the white border around each segment

  // Create the border shapes
  sf::RectangleShape topBorder(sf::Vector2f(window.getSize().x, borderSize));
  topBorder.setPosition(0, 0);
  topBorder.setFillColor(sf::Color::White);

  sf::RectangleShape bottomBorder(sf::Vector2f(window.getSize().x, borderSize));
  bottomBorder.setPosition(0, window.getSize().y - borderSize);
  bottomBorder.setFillColor(sf::Color::White);

  sf::RectangleShape leftBorder(sf::Vector2f(borderSize, window.getSize().y));
  leftBorder.setPosition(0, 0);
  leftBorder.setFillColor(sf::Color::White);

  sf::RectangleShape rightBorder(sf::Vector2f(borderSize, window.getSize().y));
  rightBorder.setPosition(window.getSize().x - borderSize, 0);
  rightBorder.setFillColor(sf::Color::White);

  // Create the segment shapes
  sf::RectangleShape segments[numSegmentsX][numSegmentsY];
  sf::RectangleShape segmentBorders[numSegmentsX][numSegmentsY][4]; // Borders around each segment (top, bottom, left, right)

  for (int i = 0; i < numSegmentsX; i++) {
    for (int j = 0; j < numSegmentsY; j++) {
      segments[i][j] = sf::RectangleShape(sf::Vector2f(segmentWidth, segmentHeight));
      segments[i][j].setPosition(borderSize + i * segmentWidth, borderSize + j * segmentHeight);
      segments[i][j].setFillColor(sf::Color::Black);

      // Create the borders around the segment
      segmentBorders[i][j][0] = sf::RectangleShape(sf::Vector2f(segmentWidth, borderThickness));
      segmentBorders[i][j][0].setPosition(borderSize + i * segmentWidth, borderSize + j * segmentHeight);
      segmentBorders[i][j][0].setFillColor(sf::Color::White);

            segmentBorders[i][j][1] = sf::RectangleShape(sf::Vector2f(segmentWidth, borderThickness));
      segmentBorders[i][j][1].setPosition(borderSize + i * segmentWidth, borderSize + j * segmentHeight + segmentHeight - borderThickness);
      segmentBorders[i][j][1].setFillColor(sf::Color::White);

      segmentBorders[i][j][2] = sf::RectangleShape(sf::Vector2f(borderThickness, segmentHeight));
      segmentBorders[i][j][2].setPosition(borderSize + i * segmentWidth, borderSize + j * segmentHeight);
      segmentBorders[i][j][2].setFillColor(sf::Color::White);

      segmentBorders[i][j][3] = sf::RectangleShape(sf::Vector2f(borderThickness, segmentHeight));
      segmentBorders[i][j][3].setPosition(borderSize + i * segmentWidth + segmentWidth - borderThickness, borderSize + j * segmentHeight);
      segmentBorders[i][j][3].setFillColor(sf::Color::White);
    }
  }

  // Create a timer to keep track of the elapsed time
  sf::Clock clock;
  float elapsedTime = 0;

  // Run the main loop
  while (window.isOpen()) {
    // Handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Clear the window
    window.clear();

    // Update the elapsed time and draw the appropriate number of segments
    elapsedTime += clock.restart().asSeconds();

    // Calculate the number of segments to draw based on the elapsed time
    int numSegments = elapsedTime / 0.5; // Draw a new segment every 0.5 seconds
    numSegments = std::min(numSegments, numSegmentsX * numSegmentsY); // Maximum of 25 segments

    for (int i = 0; i < numSegmentsX; i++) {
      for (int j = 0; j < numSegmentsY; j++) {
        if (i * numSegmentsY + j < numSegments) {
          window.draw(segments[i][j]);

          // Draw the borders around the segment
          for (int k = 0; k < 4; k++) {
            window.draw(segmentBorders[i][j][k]);
          }
        }
      }
    }

    // Draw the borders
    window.draw(topBorder);
    window.draw(bottomBorder);
    window.draw(leftBorder);
    window.draw(rightBorder);

    // Draw your GUI elements here

    // Display the window contents on the screen
    window.display();
  }

  return 0;
}
*/






































int main()
{

  sf::RenderTexture render_texture;
  render_texture.create(2300,1100);

    std::random_device rd;
    std::minstd_rand rng(rd());

    std::uniform_real_distribution<float> xdist(5, 2250);
    std::uniform_real_distribution<float> ydist(5, 1100);

    sf::Time delta_time;


    //Create graph from data
    std::cout << std::endl << std::endl << std::endl; 
    std::cout << "************************************************************************************" << std::endl;
    std::cout << "Creating graph from wikilinks." << std::endl << std::endl;
    Digraph wikigraph("/Users/Ivan/Documents/GitHub/WikiTraversalVisualized/src/links.tsv");
    std::cout << "Vertex Count: " << wikigraph.vertex_count_ << std::endl;
    std::cout << "Edge Count: " << wikigraph.edge_count_ << std::endl;
    std::cout << "************************************************************************************" << std::endl << std::endl;

    std::vector<TextCircle> nodes;
    

    // Set up the font
  sf::Font font;
  if (!font.loadFromFile("/Users/Ivan/Documents/GitHub/WikiTraversalVisualized/src/lato/Lato-Black.ttf"))
  {
      // Error loading font
      return 1;
  }

  // Create an unordered map to store the nodes
  std::unordered_map<std::string, TextCircle> node_map;

  // Get the map of nodes in the graph
  std::unordered_map<std::string, Parsing::AdjacencyList*> &adj_map = wikigraph.get_adj_map();

 // Create a vector of DirectedLine objects
std::vector<DirectedLine> lines;
TextCircle temp;

unsigned node_count = 0;
unsigned max_node = 10;
for (auto it = adj_map.begin(); it != adj_map.end(); ++it) {
  auto it_node = node_map.find(it->first);
  if (it_node == node_map.end()) {
    temp = TextCircle(it->first, 3.5f, font);
    node_map[it->first] = temp;
    float randx = xdist(rng);
    float randy = ydist(rng);
    node_map[it->first].setPosition(randx,randy);
  }

  //iterate over child nodes of current node
  for(auto edge: it->second->get_edges()) {
    auto it_child = node_map.find(edge->get_destination()->get_name());
    if (it_child == node_map.end()) {
      node_map[edge->get_destination()->get_name()] = TextCircle(edge->get_destination()->get_name(), 5.0f, font);
      // Set the position of the TextCircle object to a random position
      float randx = xdist(rng);
      float randy = ydist(rng);
      node_map[edge->get_destination()->get_name()].setPosition(randx, randy);
    }
    // Create a DirectedLine object that connects the parent node to the child node
    lines.emplace_back(node_map[it->first].getPosition(), node_map[edge->get_destination()->get_name()].getPosition());
  }

  node_count++;
  if (node_count > max_node) {
    break;
  }
}




    // Create the window
    sf::RenderWindow window(sf::VideoMode(2300, 1100), "My Window");

    sf::Clock clock;

    // Run the main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        sf::Time delta_time = clock.restart();

        for (auto& node: node_map) {
            node.second.update(delta_time.asSeconds(), sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        }

        render_texture.clear(sf::Color::White);

        // Draw the shapes
        for (auto& line: lines){
          line.update();
        }

        for (const auto& line : lines) {
          render_texture.draw(line);
        }

        for (auto& node : node_map)
        {
          render_texture.draw(node.second);
        }

        // Display the window
        render_texture.display();

        sf::Sprite sprite(render_texture.getTexture());
        window.draw(sprite);
        window.display();
    }

    return 0;
}