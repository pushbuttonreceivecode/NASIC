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

#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <THOR/Math.hpp>

namespace nasic
{
    class starfield : public sf::Drawable
    {
        public:
            starfield(sf::RenderWindow& window, sf::Uint32 style, const float& scale);
            ~starfield();

            void update(sf::RenderWindow& window, sf::Time dt);
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

            enum starStyle
            {
                smallStars,
                allStars,
                starsAndPlanets
            };

        private:
            sf::CircleShape* m_star;
            std::vector<sf::CircleShape> m_stars;
            static sf::Uint32 m_style;
            float m_scale;
    };
}

#endif // STARFIELD_HPP
