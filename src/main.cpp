//    ************************************************************
//    *                   Reflection Simulation                  *
//    *                       Jan  2016                          *
//    ************************************************************
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include "../include/Module.hh"

const float gDisplayx = 1000;
const float gDisplayy = 400;
int main() {
  //GAME SETUP
  sf::ContextSettings setting;
  setting.antialiasingLevel = 10;
  sf::RenderWindow window(sf::VideoMode(gDisplayx,gDisplayy), "Reflection Simulation", sf::Style::Default, setting);
  window.setFramerateLimit(60);


  //////////////////////////////////////////////////////
  //                   Initialize                     //
  //////////////////////////////////////////////////////
  Module module( sf::Vector2f(gDisplayx/2.0, gDisplayy/2.0), gDisplayx, gDisplayy );
  module.InitializePhoton();
  module.MakeSpecs();
  sf::Clock clock;

  while( window.isOpen() ) {
    sf::Event event;
    while( window.pollEvent(event) ) {
      if( event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ) {
	window.close();
      }
    }
    window.clear();

    // DRAWINGS
    window.draw( module );
      
    // UPDATING
    window.display();   

  }
  return 0;
}
