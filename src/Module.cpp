#include "../include/Module.hh"

#include <iostream>
#include <cmath>
#include <stdio.h>     
#include <stdlib.h>    
#include <time.h>  

Module::Module( sf::Vector2f pos, float x, float y ) {
  displayx = x;
  displayy = y;
  rad2deg = 180.0 / 3.141592;
  // Main "Module" 
  width = 300.0;
  height = 50.0;
  thickness = 5.0;

  sf::Vector2f mod_size(width, height );
  module.setSize( mod_size );
  module.setOrigin( width/2.0, height/2.0 );
  module.setFillColor( sf::Color::Black );
  module.setOutlineThickness( -thickness );
  module.setOutlineColor( sf::Color::Green );
  module.setPosition( pos.x, pos.y );

  // Erase Edges
  sf::Vector2f subtract_size( 2*thickness, height - 2*thickness );
  sub_module.setSize( subtract_size );
  sub_module.setOrigin( subtract_size.x/2.0, subtract_size.y/2.0 );
  sub_module.setFillColor( sf::Color::Black );
  sub_module.setPosition( pos.x - width/2.0, pos.y );

  sub_module1.setSize( subtract_size );
  sub_module1.setOrigin( subtract_size.x/2.0, subtract_size.y/2.0 );
  sub_module1.setFillColor( sf::Color::Black );
  sub_module1.setPosition( pos.x + width/2.0, pos.y );

  // Trace the Photon
  lines = sf::VertexArray(sf::LinesStrip,2);
}

void Module::draw( sf::RenderTarget& target, sf::RenderStates ) const {
  target.draw( module );
  target.draw( sub_module );
  target.draw( sub_module1 );
  std::vector<sf::VertexArray>::const_iterator cit;
  for(cit=path.begin(); cit != path.end(); cit++) {
    target.draw(*cit);
  }
}

void Module::initialize_photon() {
  // Lets generate a random coordinate on LHS of the screen
  srand (time(NULL));
  float leftsideofmodule = module.getPosition().x - width/2.0 - 20.0;
  int tempX = 200.0;
  int tempY = rand() % int(displayy);
  sf::Vector2f initial(tempX, tempY/1.7);

  // Define the initial point randomly
  lines[0].position = initial;
  lines[0].color = sf::Color::White;

  // Find the second point randomly
  tempY = rand() % int(height-thickness); //0-45 
  sf::Vector2f TL_module( module.getPosition().x - width/2.0, module.getPosition().y - height/2.0 + thickness + tempY);
  sf::Vector2f dist = TL_module - initial;
  float dist_mag = sqrt( pow(dist.x,2) + pow(dist.y,2) );
  sf::Vector2f dist_hat = dist / dist_mag;
  // Calculate the angle of dist wrt to x_hat
  float theta = acos(dist_hat.x);

  // Extend the vector from the left face to somewhere within the module
  float l = tempY / tan(theta);
  sf::Vector2f correction( l, height - tempY - 2*thickness);
  sf::Vector2f final = TL_module + correction;
  lines[1].position = final;
  lines[1].color = sf::Color::White;
  path.push_back( lines );

  // Iteratively propagate through the module
  // Keep track of a length variable
  float propagated = l;
  sf::Vector2f current = final;
  sf::Vector2f next(0.0,0.0);
  int upordown = 0;
  float next_mag = 0.0;
  while( propagated <= width ) {   
    lines[0].position = current;
    lines[0].color = sf::Color::White;
    next_mag = (height - 2*thickness) / sin(theta);
  
    propagated += next_mag * cos(theta);

    float nextX = 0, nextY = 0;
    nextX = next_mag*cos(theta);
    if( upordown % 2 == 0 ){
      nextY = -next_mag*sin(theta);
    } else {
      nextY = next_mag*sin(theta);
    }
    upordown++;
    sf::Vector2f next_temp(nextX, nextY);
    next = current + next_temp;
    lines[1].position = next;
    lines[1].color = sf::Color::White;
    path.push_back( lines );

    current = next;
    sf::Vector2f reset(0.0,0.0);
    next_temp = reset;
  }
}
