#ifndef MODULE_HH
#define MODULE_HH
#include <SFML/Graphics.hpp>

class Module : public sf::Drawable, public sf::Transformable {

private:
  sf::Vector2f mod_size;
  float width, height, displayx, displayy;
  float thickness;
  float rad2deg;
  sf::RectangleShape module, sub_module, sub_module1;

  // Photons
  sf::VertexArray lines;
  std::vector<sf::VertexArray> path;

  // Text
  int reflections;
  float initial_angle;
  sf::Text text1,text2;
  sf::Font font;
  std::vector<sf::Text> textvec;
  std::vector<sf::Text>::iterator tit;
  
public:
  Module(sf::Vector2f, float, float);
  ~Module() {};
  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void InitializePhoton();
  int GetReflections() { return path.size()-2; }
  float GetTheta() { return initial_angle; }
  void MakeSpecs();
};
#endif
