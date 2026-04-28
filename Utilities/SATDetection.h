#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include <SFML/System/Vector2.hpp>

/*
 * helper suite to calculate and detect collisions between convex polygons using 
 * Seperate axis theorem
 * Uses circle shape to define custom polygons (less setup than convex shape)
 * derived from: https://www.youtube.com/watch?v=IkdGcvKXNJw
 * & Co-pilot
 * Produces more overhead than bounding boxes but allows for any type of shape to be created or rotated
 */

class sat_detection
{
    public:
    static std::vector <sf::Vector2f> get_transformed_points(const sf::Shape& shape);
    static void project_onto_axis(const std::vector<sf::Vector2f>& pts, const sf::Vector2f& axis, float& min, float& max);
    static bool projection_overlap(float min_a, float max_a, float min_b, float max_b);
    
    static sf::Vector2f calculate_centre(const std::vector<sf::Vector2f>& pts);
  
    static bool sat_collision(const sf::Shape& shape_a, const sf::Shape& shape_b, sf::Vector2f& mtv);
};
