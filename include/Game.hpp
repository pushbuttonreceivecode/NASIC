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

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <intro.hpp>
#include <menu.hpp>
#include <options.hpp>
#include <info.hpp>
#include <level.hpp>
#include <youwin.hpp>
#include <youlose.hpp>

namespace nasic
{
    class Game
    {
        public:
            Game();
            ~Game();

            static void init();

        private:
            static void run();

            static void intro();
            static void menu();
            static void options();
            static void info();
            static void level();
            static void win();
            static void lose();

            static bool isQuitting();

            enum state
            {
                s_uninitialized,
                s_intro,
                s_menu,
                s_info,
                s_options,
                s_level,
                s_win,
                s_lose,
                s_quit
            };

        private:
            static sf::Uint32 m_state;
            static sf::RenderWindow m_window;
    };
}

#endif // GAME_HPP
