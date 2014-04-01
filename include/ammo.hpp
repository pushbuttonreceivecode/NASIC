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

#ifndef AMMO_HPP
#define AMMO_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

namespace nasic
{
    class ammo : public sf::Sprite
    {
        public:
            ammo();
            ammo(sf::Uint32 type, sf::Vector2f initPos, const float& scale);
            ~ammo();

            void fire(sf::Time dt);

            enum ammoType
            {
                warrior = 0,
                hunter = 1,
                guard = 2,
                special_warrior = 3,
                special_hunter = 4,
                special_guard = 5,
                rhiians = 6,
                agravu = 7,
                delsiriak = 8,
                gluorn = 9
            };

        private:

        private:
            float m_velY;
            sf::Uint32 m_type;
            sf::Texture m_tex;

            float m_scale;
    };
}

#endif // AMMO_HPP
