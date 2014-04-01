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

#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <THOR/Shapes.hpp>
#include <starfield.hpp>
#include <hexgrid.hpp>
#include <opstruct.hpp>

namespace nasic
{
    class menu
    {
        public:
            menu();
            ~menu();
            void show(sf::RenderWindow& window);
            const sf::Uint32 menuState(){return m_choice;};

            enum choice
            {
                s_uninitialized,
                s_undecided,
                s_play,
                s_info,
                s_options,
                s_quit
            };

        private:
            static choice m_choice;
            nasic::opstruct m_options;
            std::string m_filename;
            sf::Uint32 m_initialVol;
            sf::Uint32 m_initialEff;
            sf::Uint32 m_initialDif;

            float m_scale;
            float m_winsizeX;
            float m_winsizeY;
    };
}

#endif // MENU_HPP
