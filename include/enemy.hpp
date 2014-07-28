//////////////////////////////////////
//************************************
//Not Another Space Invaders Clone
//Author: Mark Richards
//Date: 2/1/2014
//License: zlib/png
//************************************
//////////////////////////////////////

/*
Copyright (c) 2014 Mark Richards

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <THOR/Animation.hpp>

namespace nasic
{
    class enemy : public sf::Sprite
    {
        public:
            enemy();
            enemy(sf::Uint32 type, const float& scale);
            ~enemy();

            sf::Uint32 getState();
            sf::Uint32 getType(){return m_type;};

            sf::Vector2f getAABB();

            void setId(sf::Vector2u id){m_id = id;};
            sf::Vector2u getId(){return m_id;};

            void setInitialPosition(sf::Vector2f pos){m_initialPos = pos;};
            sf::Vector2f getInitialPosition(){return m_initialPos;};

            void damage(int d);
            int getHealth(){return m_data.health;};

            void animate(sf::Time dt);
            void update(sf::Time dt);

            enum enemyType
            {
                Delsiriak,
                Agravu,
                Gluorn,
                Rhiians
            };

            enum enemyState
            {
                normal,
                pissed,
                dead
            };

        private:
            void addFrames(thor::FrameAnimation& animation, int w, int h, int y, int xFirst, int xLast, float duration = 1.f);
            void updateState();

            void handleState();

        private:
            struct data
            {
                int health;
                int strength;
            };
            data m_data;

            sf::Uint32 m_type;
            sf::Uint32 m_state;
            sf::Vector2u m_id;
            sf::Vector2f m_initialPos;

            sf::Vector2f m_aabb;

            sf::Texture m_tex;
            thor::Animator<sf::Sprite, std::string> m_animator;
            thor::FrameAnimation m_frames;

            float m_scale;
    };
}

#endif // ENEMY_HPP
