#include "SATDetection.h"

std::vector <sf::Vector2f> sat_detection::get_transformed_points(const sf::CircleShape& shape)
{
    std::vector <sf::Vector2f> points(shape.getPointCount());
    for (unsigned int i = 0; i < points.size(); i++)
    {
        points[i] = shape.getTransform().transformPoint(shape.getPoint(i));
    }
    return points;
}

void sat_detection::project_onto_axis(const std::vector<sf::Vector2f>& pts, const sf::Vector2f& axis, float& min,
    float& max)
{
    min = max = (pts[0].x * axis.x + pts[0].y * axis.y);
    for (size_t i = 1; i < pts.size(); i++)
    {
        float p = pts[i].x * axis.x + pts[i].y * axis.y;
        if (p < min) min = p;
        if (p > max) max = p;
    }

}

bool sat_detection::projection_overlap(const float min_a, const float max_a, const float min_b, const float max_b)
{
    return !(min_a < min_b || max_b < max_a);
}

bool sat_detection::sat_collision(const sf::CircleShape& shape_a, const sf::CircleShape& shape_b, sf::Vector2f& mtv)
{
    std::vector <sf::Vector2f> points_a = get_transformed_points(shape_a);
    std::vector <sf::Vector2f> points_b = get_transformed_points(shape_b);
    
    std::vector <sf::Vector2f> axes;
    
    
    float smallest_overlap = std::numeric_limits<float>::infinity();
    sf::Vector2f smallest_axis;

    
    // Get axes from both polygons
    auto add_axes = [&](const std::vector<sf::Vector2f>& pts)
    {
        for (size_t i = 0; i < pts.size(); i++)
        {
            const sf::Vector2f p1 = pts[i];
            const sf::Vector2f p2 = pts[(i + 1) % pts.size()];
            const sf::Vector2f edge = p2 - p1;

            // normal (perpendicular)
            sf::Vector2f axis(edge.y, -edge.x);

            // normalize
            float len = std::sqrt(axis.x*axis.x + axis.y*axis.y);
            if (len != 0) axis /= len;

            axes.push_back(axis);
        }
    };
    
    add_axes(points_a);
    add_axes(points_b);
    
    // testing all axes
    for (sf::Vector2f& axis : axes)
    {
        float min_a, max_a, min_b, max_b;
        project_onto_axis(points_a, axis, min_a, max_a);
        project_onto_axis(points_b, axis, min_b, max_b);
        
        // separating axis found, no collision 
        if (!projection_overlap(min_a, max_a, min_b, max_b)) return false;
        
        float overlap = std::min(max_a, max_b) - std::min(min_a, min_b);
        if (overlap < smallest_overlap)
        {
            smallest_overlap = overlap;
            smallest_axis = axis;
        }
    }
    
    mtv = smallest_axis * smallest_overlap;
    return true;
}

