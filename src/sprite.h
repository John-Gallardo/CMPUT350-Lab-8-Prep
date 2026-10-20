#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    // set mTexture, and mMinPointTex and mMaxPointTex based on the given texture's max texture coordinates
    // also we create vertex array: primitive used is triangles and we have 6 vertices
    Sprite(const sf::Texture& texture)
    : mTexture{&texture}, mVertexArray{sf::PrimitiveType::Triangles, 6} {
        // default: scale is 1 to 1, no offset
        mTransform = BasicTransform();
        mOrigin = sf::Vector2f{0.0f, 0.0f};  // origin by default?

        // NOTE: I assume minimum is always (0, 0) if not given
        mMinPointTex = sf::Vector2f(0.0f, 0.0f);

        // screenspace pixel coordinates transformed from ints to floats
        sf::Vector2u textureSize{texture.getSize()};
        mMaxPointTex = sf::Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));

        // NOTE: vertex position is [0, 0] to [mMaxPointTex.x, mMaxPointTex.y] by default
        for (int i{0}; i < 6; i++) {
            sf::Vector2f vertex{DEFAULT_VERTICES[i].x * mMaxPointTex.x, DEFAULT_VERTICES[i].y * mMaxPointTex.y};
            mVertexArray[i].position = vertex;
        }
    }

    // sets mTexture, mMinPointText, mMaxPointTex
    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint) 
    : mTexture{&texture}, mMinPointTex{texMinPoint}, mMaxPointTex{texMaxPoint} {
        // same code as above constructor w/o mMinPointTex and mMaxPointTex manually calculated
        mTransform = BasicTransform();
        mOrigin = sf::Vector2f{0.0f, 0.0f};
        for (int i{0}; i < 6; i++) {
            sf::Vector2f vertex{DEFAULT_VERTICES[i].x * mMaxPointTex.x, DEFAULT_VERTICES[i].y * mMaxPointTex.y};
            mVertexArray[i].position = vertex;
            mVertexArray[i].texCoords = vertex;
        }
    }

    // draw onto render target with our vertex array with the given target and state
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // reference: https://www.sfml-dev.org/tutorials/3.0/graphics/vertex-array/#texturing
        // specifically, setting texture & transform of an sf::RenderStates
        //NOTE: states.texture takes in a memory address & mTexture is already a pointer
    
        states.texture = mTexture;
        // using constructor with 9 values, (each for each value of the 3x3 matrix)
        states.transform = sf::Transform{
            mTransform.scale.x, 0, mTransform.translation.x,
            0, mTransform.scale.y, mTransform.translation.y,
            0, 0, 1
        };
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

        // set just mVertexArray. can't set others since this function is const
         for (int i{0}; i < 6; i++) {
            sf::Vector2f vertex{DEFAULT_VERTICES[i].x * mMaxPointTex.x, DEFAULT_VERTICES[i].y * mMaxPointTex.y};
            mVertexArray[i].position = vertex;
        }
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
