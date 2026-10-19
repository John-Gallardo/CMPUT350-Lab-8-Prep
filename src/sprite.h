#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// used for coordinate transformations
constexpr int g_windowWidth{256};
constexpr int g_windowHeight{256};

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    // set mTexture, and mMinPointTex and mMaxPointTex based on the given texture's max texture coordinates
    // also we create vertex array: primitive used is triangles and we have 6 vertices
    Sprite(const sf::Texture& texture)
    : mTexture{&texture}, mVertexArray{sf::PrimitiveType::Triangles, 6} {
        // NOTE: I assume minimum is always (0, 0) if not given
        mMinPointTex = sf::Vector2f(0.0f, 0.0f);

        // NOTE: need to change if window size changes since getSize() returns the size of the texture in PIXELS.
        sf::Vector2u textureSize{texture.getSize()};
        mMaxPointTex = sf::Vector2f(static_cast<float>(textureSize.x) / g_windowWidth, static_cast<float>(textureSize.y) / g_windowHeight);

        // NOTE: mVertexArray by default covers the entire screen in screenspace coordinates I assume?
        for (int i{0}; i < 6; i++) {
            auto vertex{DEFAULT_VERTICES[i]};
            // TODO: need to set mVertexArray
        }


        // TODO: probably need to set mOrigin and mTransform too
    }

    // sets mTexture, mMinPointText, mMaxPointTex
    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint) 
    : mTexture{&texture}, mMinPointTex{texMinPoint}, mMaxPointTex{texMaxPoint} {
        // TODO: incomplete! probably need to set rest of member variables
    }

    // draw onto render target with our vertex array with the given target and state
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // It may help you to write this, which is an array of 6 vertices:
    //   - the first 3 correspond to the vertices of the top-left triangle of a "default rectangle"
    //     ranging from [0, 1] in both dimensions.
    //   - the next 3 correspond to the vertices of the bottom-right triangle. of the default
    //     rectangle.
    // NOTE: wound CCW, I don't think it matters though
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {{
        {1.0f, 0.0f},  // top right
        {0.0f, 0.0f},  // top left
        {0.0f, 1.0f},  // bottom left
        {0.0f, 1.0f},  // bottom left
        {1.0f, 1.0f},  // bottom right
        {1.0f, 0.0f}   // top right
    }};

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {
        // Edge Case: no texture
        if (mTexture == nullptr) {
            return; 
        }

        // TODO: I guess just needs to set mVertexArray? can't set the others since function is marked const
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif
