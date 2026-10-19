// maybe something here...
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // methods...

    // 1. constructor taking custom scale and translation.
    // default args are identity transformation -> translation = {0, 0} and scale = {1, 1}
    BasicTransform(sf::Vector2f scale = {1, 1}, sf::Vector2f translation = {0, 0})
        : scale{scale}, translation{translation} {}

    // 2. apply point giving result of applying transform to a point
    // NOTE: formula is from 'Scale' and then 'Translation' section.
    // i.e we apply scale and then translation.
    sf::Vector2f applyPoint(const sf::Vector2f &p) {
        return {
            (p.x * scale.x) + translation.x,
            (p.y * scale.y) + translation.y
        };
    }

    // 3. apply vector giving result of applying transform to a vector
    // NOTE: basically the same as point but we don't translate
    // "because vectors are 'points at infinity'."
    sf::Vector2f applyVector(const sf::Vector2f &p) {
        return {
            p.x * scale.x,
            p.y * scale.y
        };
    }

    // 4. exact comparison operator, self explanatory
    bool operator==(const BasicTransform &other) {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};
