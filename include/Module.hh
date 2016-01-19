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
  sf::VertexArray lines;
  std::vector<sf::VertexArray> path;

public:
  Module(sf::Vector2f, float, float);
  ~Module() {};
  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void initialize_photon();
};
#endif
