#include "../include/Module.hh"

#include <iostream>
#include <cmath>
#include <stdio.h>     
#include <stdlib.h>    
#include <time.h> 
#include <string>
#include <sstream> 

Module::Module( sf::Vector2f pos, float x, float y ) {
  displayx = x;
  displayy = y;
  rad2deg = 180.0 / 3.141592;
  // Main "Module" 
  width = 800.0;
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
  reflections = 0;
  initial_angle = 0.0;

  // Text
  if( !font.loadFromFile("fonts/arial.ttf")) {
    std::cerr << "ERROR: Font did not load properly." << std::endl;
  }
}

void Module::draw( sf::RenderTarget& target, sf::RenderStates ) const {
  target.draw( module );
  target.draw( sub_module );
  target.draw( sub_module1 );

  std::vector<sf::VertexArray>::const_iterator cit;
  for(cit=path.begin(); cit != path.end(); cit++) {
    target.draw(*cit);
  }
  std::vector<sf::Text>::const_iterator tit1;
  for( tit1=textvec.begin(); tit1!=textvec.end(); tit1++) {
    target.draw(*tit1);
  }
}

void Module::InitializePhoton() {
  // Lets generate a random coordinate on LHS of the screen
  srand (time(NULL));
  int tempX = rand() % int(module.getPosition().x - width/2.0);
  int tempY = rand() % int(displayy);
  sf::Vector2f initial(tempX, tempY);

  // Define the initial point randomly
  lines[0].position = initial;
  lines[0].color = sf::Color::White;

  // Find the second point, assume that is passes through center of LH Face
  sf::Vector2f CL_module( module.getPosition().x - width/2.0, module.getPosition().y );
  sf::Vector2f dist = CL_module - initial;
  float dist_mag = sqrt( pow(dist.x,2) + pow(dist.y,2) );
  sf::Vector2f dist_hat = dist / dist_mag;

  // Calculate the angle of dist wrt to x_hat
  float theta = acos(dist_hat.x);
  initial_angle = theta*(180.0 / 3.141592);

  // Extend the vector from the left face to somewhere within the module
  float lengthx = (height/2.0) / tan(theta);
  tempX = lengthx;
  tempY = height/2.0 - thickness;
  sf::Vector2f correction(tempX,tempY);
  // If dist_hat.y < 0, then the reflections will be inverted, so take this into account
  bool invert = false;
  if( dist_hat.y < 0 ) {
    tempX = lengthx;
    tempY = -height/2.0 + thickness;
    sf::Vector2f temp(tempX,tempY);
    correction = temp;
    invert = true;
  }
  
  sf::Vector2f first_track = CL_module + correction;
  lines[1].position = first_track;
  lines[1].color = sf::Color::White;
  path.push_back( lines );

  // Iteratively propagate through the module
  // Keep track of a length variable, if the photon_track.dot(x_hat) exceeds
  // length of module, then stop the iteration:
  float propagated = lengthx;
  sf::Vector2f current = first_track;
  sf::Vector2f next(0.0,0.0);
  int upordown = 0;
  float next_mag = 0.0;
  float nextX, nextY;
  while( propagated <= width ) {   
    lines[0].position = current;
    lines[0].color = sf::Color::White;
    // Find the magnitude of the next vector
    next_mag = (height - 2*thickness) / sin(theta);
  
    // Keep track of total photon_track.dot(x_hat)
    propagated += next_mag * cos(theta);

    // Temporary coordinates of final vertexarray point
    nextX = next_mag*cos(theta);
    nextY = 0.0;

    // Choose to go up or down in y_hat
    if( !invert ) {
      if( upordown % 2 == 0 ){
	nextY = -next_mag*sin(theta);
      } else {
	nextY = next_mag*sin(theta);
      }
    }
    else{
      if( upordown % 2 == 0 ){
	nextY = next_mag*sin(theta);
      } else {
	nextY = -next_mag*sin(theta);
      }
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

  // Extend the last photon track, and make it the same length
  // as the initial photon track
  lines[0].position = next;
  lines[0].color = sf::Color::White;
  float length_of_initial = 0.1*lengthx / cos(theta);
  sf::Vector2f next_temp(length_of_initial*nextX, length_of_initial*nextY);
  lines[1].position = next + next_temp;
  lines[1].color = sf::Color::White;
  path.push_back( lines );
}

void Module::MakeSpecs() {
  float theta = GetTheta();
  int nbounce = GetReflections();
  std::ostringstream temp,temp1;
  temp << theta;
  std::string thetastring = temp.str();
  temp1 << nbounce;
  std::string nbouncestring = temp1.str();

  // Add in Text Properties
  text1.setFont(font);
  text1.setString(thetastring);
  text1.setCharacterSize(40);
  text1.setColor(sf::Color::White);
  textvec.push_back( text1 );
  text2.setFont(font);
  text2.setString(nbouncestring);
  text2.setCharacterSize(40);
  text2.setColor(sf::Color::White);
  textvec.push_back( text2 );
  
  float offset = -100.0;
  for( tit=textvec.begin(); tit!=textvec.end(); tit++ ) {

    sf::FloatRect recttemp = (*tit).getLocalBounds();
    (*tit).setPosition( (displayx-recttemp.width)/2.0 + offset , 0.5*recttemp.height);
    offset = 100.0;
  }
}
