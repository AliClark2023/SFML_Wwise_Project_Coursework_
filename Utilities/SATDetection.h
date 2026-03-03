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
 */

class sat_detection
{
    public:
    // transforms from local points into world
    static std::vector <sf::Vector2f> get_transformed_points(const sf::CircleShape& shape);
    // projects polygon points onto axis for overlapping detection
    static void project_onto_axis(const std::vector<sf::Vector2f>& pts, const sf::Vector2f& axis, float& min, float& max);
    // returns if theres an overlap between point A & B when its projected onto an axis
    static bool projection_overlap(float min_a, float max_a, float min_b, float max_b);
    
    /*
     * Performs Separate axis theorem to detect if there's a collision between two shapes 
     * and determines which orientation the collision came from using minimal translation vector MTV
     */
    static bool sat_collision(const sf::CircleShape& shape_a, const sf::CircleShape& shape_b, sf::Vector2f& mtv);
};
