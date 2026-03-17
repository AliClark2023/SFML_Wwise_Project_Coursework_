#include "SATDetection.h"

// transforms from local points into world
std::vector <sf::Vector2f> sat_detection::get_transformed_points(const sf::Shape& shape)
{
    std::vector <sf::Vector2f> points(shape.getPointCount());
    for (unsigned int i = 0; i < points.size(); i++)
    {
        points[i] = shape.getTransform().transformPoint(shape.getPoint(i));
    }
    return points;
}


sf::Vector2f sat_detection::calculate_centre(const std::vector<sf::Vector2f>& pts)
{
    sf::Vector2f c(0.f, 0.f);
    if (pts.empty()) return c;
    for (const auto& p : pts) { c.x += p.x; c.y += p.y; }
    c.x /= static_cast<float>(pts.size());
    c.y /= static_cast<float>(pts.size());
    return c;
}

// projects polygon points onto axis for overlapping detection
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
// returns if theres an overlap between point A & B when its projected onto an axis
bool sat_detection::projection_overlap(const float min_a, const float max_a, const float min_b, const float max_b)
{
    return !(max_a < min_b || max_b < min_a);
}

/*
   * Performs Separate axis theorem to detect if there's a collision between two shapes 
   * and determines which orientation the collision came from using minimal translation vector MTV
   */
// collision assumes shape A will be the object to move out of shape Bs way
// works with convex shapes only
bool sat_detection::sat_collision(const sf::Shape& shape_a, const sf::Shape& shape_b, sf::Vector2f& mtv)
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
        
        float overlap = std::min(max_a, max_b) - std::max(min_a, min_b);
        //float overlap = std::min(min_a, min_b) - std::max(max_a, max_b);
        if (overlap < smallest_overlap)
        {
            smallest_overlap = overlap;
            smallest_axis = axis;
        }
    }
    
    
    // calculating direction of movement needed for collision response
    sf::Vector2f centre_a = calculate_centre(points_a);
    sf::Vector2f centre_b = calculate_centre(points_b);
    sf::Vector2f axis = centre_a - centre_b;
    if (axis.dot(smallest_axis) < 0.0f)
    {
        smallest_axis = -smallest_axis;
    }
    mtv = smallest_axis * smallest_overlap;
    return true;
}

