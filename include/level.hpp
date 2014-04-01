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

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <THOR/Animation.hpp>
#include <starfield.hpp>
#include <enemy.hpp>
#include <player.hpp>
#include <ammo.hpp>
#include <opstruct.hpp>

namespace nasic
{
    void addFrames(thor::FrameAnimation& animation, int w, int h, int y, int xFirst, int xLast, float duration = 1.f);

    class level
    {
        public:
            level();
            ~level();

            void show(sf::RenderWindow& window);

            const sf::Uint32 levelState(){return m_levelstate;};

            enum levelstate
            {
                uninitialized,
                playing,
                paused,
                exit
            };

        private:
            void initEnemies(sf::RenderWindow& window, int rows, int columns, float scale);
            void createWave();
            void updateEnemies(sf::RenderWindow& window, sf::Time dt);
            void renderEnemies(sf::RenderWindow& window);
            void updateProjectiles(sf::Time dt);
            void renderProjectiles(sf::RenderWindow& window);
            void checkEnemyCollisions(sf::RenderWindow& window);
            void cleanup();

        private:
            static sf::Uint32 m_levelstate;
            std::list<nasic::enemy> m_enemies;
            nasic::enemy* m_enemy;

            std::list<nasic::ammo> m_playerAmmo;
            std::list<nasic::ammo>::iterator m_pAmmoIt;

            std::list<nasic::ammo> m_enemyAmmo;
            std::list<nasic::ammo>::iterator m_eAmmoIt;

            float m_winsizeX;
            float m_winsizeY;

            nasic::opstruct m_options;
            int m_musicVolume;
            int m_effectsVolume;
            int m_difficulty;

            sf::Sound m_explosion;
            sf::Sprite m_explosionSpr;

            sf::SoundBuffer m_hitBuff;
            sf::SoundBuffer m_explodBuff;

            thor::Animator<sf::Sprite, std::string> m_expAnim;
            thor::FrameAnimation m_expFrames;
    };
}

#endif // LEVEL_HPP
