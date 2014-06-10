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

#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <opstruct.hpp>
#include <interpolate.hpp>
#include <starfield.hpp>

namespace nasic
{
    class gameOver
    {
        public:
            gameOver();
            ~gameOver();

            void show(sf::RenderWindow& window);

            sf::Uint32 getState(){return m_gameOverState;};

            enum gameOverState
            {
                uninitialized,
                playing,
                done,
            };

        private:
            sf::Uint32 m_gameOverState;
            nasic::opstruct m_options;
            float m_vol;
            float m_eff;
            float m_diff;
            std::string m_filename;
            sf::Uint32 m_initialVol;
            sf::Uint32 m_initialEff;
            sf::Uint32 m_initialDif;
            sf::SoundBuffer m_transitionBuff;
            sf::Sound m_transition;
    };
}

#endif // GAMEOVER_HPP
