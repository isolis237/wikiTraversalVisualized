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


const string startNode = "Banana";
const string endNode = "Wikipedia";


const float screenW = 2150;
const float screenH = 1100;

const string data_path = "/Users/Ivan/Documents/GitHub/wikiTraversalVisualized/src/links.tsv";
const string font_path = "/Users/Ivan/Documents/GitHub/wikiTraversalVisualized/src/lato/Lato-Regular.ttf";

const unsigned max_node = 5000;
const float node_size = 15000.0f / max_node; 

int main()
{

  // Create the window
  sf::RenderWindow window(sf::VideoMode(screenW, screenH), "WikiTraversals");
  sf::RenderTexture render_texture;
  render_texture.create(screenW,screenH);

  //initialize random number generators
  std::random_device rd;
  std::minstd_rand rng(rd());
  std::uniform_real_distribution<float> xdist(5, screenW - 25);
  std::uniform_real_distribution<float> ydist(5, screenH - 20);

  //Create graph from data
  std::cout << std::endl << std::endl << std::endl; 
  std::cout << "************************************************************************************" << std::endl;
  std::cout << "Creating graph from wikilinks." << std::endl << std::endl;
  Digraph wikigraph(data_path);
  std::cout << "Vertex Count: " << wikigraph.vertex_count_ << std::endl;
  std::cout << "Edge Count: " << wikigraph.edge_count_ << std::endl;
  std::cout << "************************************************************************************" << std::endl << std::endl;

  //initialize timers 
  sf::Time delta_time;
  sf::Clock clock;

  //initialize data maps
  std::unordered_map<std::string, Parsing::AdjacencyList*> &adj_map = wikigraph.get_adj_map();
  std::unordered_map<std::string, TextCircle> node_map;
  std::vector<DirectedLine> lines;

  // Set up the font
  sf::Font font;
  if (!font.loadFromFile(font_path))
  {
      // Error loading font
      return 1;
  }

TextCircle temp;
unsigned node_count = 0;

//create a textCircle object for each node in the graph
for (auto it = adj_map.begin(); it != adj_map.end(); ++it) {
  auto it_node = node_map.find(it->first);
  if (it_node == node_map.end()) {
    temp = TextCircle(it->first, node_size, font);
    node_map[it->first] = temp;
    float randx = xdist(rng);
    float randy = ydist(rng);
    node_map[it->first].setPosition(randx,randy);
  }

  //iterate over child nodes of current node
  for(auto edge: it->second->get_edges()) {
    auto it_child = node_map.find(edge->get_destination()->get_name());
    if (it_child == node_map.end()) {
      node_map[edge->get_destination()->get_name()] = TextCircle(edge->get_destination()->get_name(), node_size, font);
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

//find dfs and dijkstra path between two nodes
vector<string> dfs_path = DFS(&wikigraph).get_path(startNode, endNode);
vector<string> dijkstra_path = Dijkstra(&wikigraph).get_path(startNode, endNode);

std::cout << "DFS path length: ";
std::cout << dfs_path.size() << std::endl;

std::cout << "Dijkstra path length: ";
std::cout << dijkstra_path.size() << std::endl << std::endl;
for (string name : dijkstra_path) {
  std::cout << node_map[name].getText() << std::endl;
}
std::cout << std::endl;

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

        sf::Time delta_time = clock.restart();

        render_texture.clear(sf::Color::White);

        //Draw each node and directed line
        for (const auto& line : lines) {
          render_texture.draw(line);
        }
        for (auto& node: node_map) {
            node.second.update(delta_time.asSeconds(), sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
            render_texture.draw(node.second);
        }

        //create line that will be used to highlight path from parent to each child node
        sf::VertexArray path = sf::VertexArray(sf::LineStrip, 2);

        //update all nodes within the dijkstra path
        for (unsigned i = 0; i < dijkstra_path.size() - 1; i++) {
          path[0].position = node_map[dijkstra_path[i]].getPosition();
          path[0].color = sf::Color::Magenta;
          node_map[dijkstra_path[i]].algoUpdate();
          path[1].position = node_map[dijkstra_path[i + 1]].getPosition();
          path[1].color = sf::Color::Magenta;
          node_map[dijkstra_path[i+1]].algoUpdate();
          render_texture.draw(path);
          render_texture.draw(node_map[dijkstra_path[i]]);
        }
        //update color of the first and last node in the path
        node_map[dijkstra_path.front()].setColor(sf::Color::Green);
        node_map[dijkstra_path.back()].algoUpdate();
        node_map[dijkstra_path.back()].setColor(sf::Color::Red);

        //draw updates to render_texture
        render_texture.draw(node_map[dijkstra_path.front()]);
        render_texture.draw(node_map[dijkstra_path.back()]);

        //display the updates
        render_texture.display();
        sf::Sprite sprite(render_texture.getTexture());
        window.draw(sprite);
        window.display();
    }
    // Display the window
    window.display();
}



    return 0;
}