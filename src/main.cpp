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



const float screenW = 1450;
const float screenH = 750;

const string data_path = "/Users/Ivan/Documents/GitHub/wikiTraversalVisualized/src/links.tsv";
const string font_path = "/Users/Ivan/Documents/GitHub/wikiTraversalVisualized/src/lato/Lato-Regular.ttf";



int main()
{

  sf::RenderTexture render_texture;
  render_texture.create(screenW,screenH);

    std::random_device rd;
    std::minstd_rand rng(rd());

    std::uniform_real_distribution<float> xdist(5, screenW - 25);
    std::uniform_real_distribution<float> ydist(5, screenH - 25);

    sf::Time delta_time;


    //Create graph from data
    std::cout << std::endl << std::endl << std::endl; 
    std::cout << "************************************************************************************" << std::endl;
    std::cout << "Creating graph from wikilinks." << std::endl << std::endl;
    Digraph wikigraph(data_path);
    std::cout << "Vertex Count: " << wikigraph.vertex_count_ << std::endl;
    std::cout << "Edge Count: " << wikigraph.edge_count_ << std::endl;
    std::cout << "************************************************************************************" << std::endl << std::endl;

    std::vector<TextCircle> nodes;
    

    // Set up the font
  sf::Font font;
  if (!font.loadFromFile(font_path))
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
unsigned max_node = 5;
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
    lines.back().setParent(&node_map[it->first]);
    lines.back().setChild(&node_map[edge->get_destination()->get_name()]);
    node_map[it->first].addNeighbor(&node_map[edge->get_destination()->get_name()]);
  }

  node_count++;
  if (node_count > max_node) {
    break;
  }
}

  for (auto line : lines) {
    std::cout << line.getParent()->getText();
    std::cout << ", ";
    std::cout << line.getChild()->getText() << std::endl;
  }

/**
  TextCircle* testNode = &node_map["Zara_Yaqob"];
  vector<TextCircle*> neighbors = testNode->getNeighbors();

  for (auto neighbor : neighbors) {
    std::cout << neighbor->getText() << std::endl;
  }*/


    // Create the window
    sf::RenderWindow window(sf::VideoMode(screenW, screenH), "My Window");

    window.setFramerateLimit(30);

    sf::Clock clock;
    sf::Time elapsed_time = sf::Time::Zero;

    // Set the time delay between each node draw
    const sf::Time draw_delay = sf::seconds(5.0f);

    sf::Time timer = sf::Time::Zero;

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
        elapsed_time += delta_time;
        timer += delta_time;

        for (auto& node: node_map) {
            node.second.update(delta_time.asSeconds(), sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        }

        render_texture.clear(sf::Color::White);

        //Draw each node and directed line
        for (const auto& line : lines) {
          render_texture.draw(line);
        }

        for (auto& node : node_map)
        {
          render_texture.draw(node.second);
        }

        //find neighbors of node of interest
        vector<TextCircle* > parentNodeNeighbors = node_map["Wikispecies"].getNeighbors();

        //create line that will be used to highlight path from parent to each child node
        sf::VertexArray path = sf::VertexArray(sf::LineStrip, 2);
        path[0].position = node_map["Wikispecies"].getPosition();
        path[0].color = sf::Color::Magenta;
        //iterate through each neighbor and update it & draw updated node to canvas
        for (auto &node : parentNodeNeighbors) {
          elapsed_time += delta_time;
          if (elapsed_time > draw_delay) {
            node->algoUpdate();
            path[1].position = node->getPosition();
            path[1].color = sf::Color::Magenta;
            render_texture.draw(path);
            render_texture.draw(*node);
            render_texture.display();
            sf::Sprite sprite(render_texture.getTexture());
            window.draw(sprite);
            window.display();
            elapsed_time = sf::Time::Zero;
          }
        }

      /**
        // Display the window
        render_texture.display();

        sf::Sprite sprite(render_texture.getTexture());
        window.draw(sprite);
        window.display();
        */
    }

    return 0;
}