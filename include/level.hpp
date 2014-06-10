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
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Animation.hpp>
#include <Thor/Math/Distributions.hpp>
#include <THOR/Shapes.hpp>
#include <starfield.hpp>
#include <enemy.hpp>
#include <player.hpp>
#include <killer.hpp>
#include <ammo.hpp>
#include <particle.hpp>
#include <opstruct.hpp>
#include <button.hpp>
#include <interpolate.hpp>
#include <StringHelpers.hpp>

namespace nasic
{
    void addFrames(thor::FrameAnimation& animation, int w, int h, int y, int xFirst, int xLast, float duration = 1.f);

    class level
    {
        public:
            level(sf::RenderWindow& window);
            ~level();

            void show(sf::RenderWindow& window);

            const sf::Uint32 levelState(){return m_levelstate;};

            enum levelstate
            {
                uninitialized,
                playing,
                paused,
                lost,
                won,
                exit
            };

        private:
            void initEnemies(sf::RenderWindow& window, int rows, int columns, float scale);
            void showIntro(sf::RenderWindow& window, sf::Time dt);
            bool introIsDone(){return m_introStatus;};
            void setIntroStatus(bool b){m_introStatus = b;};
            void showBossIntro(sf::RenderWindow& window, sf::Time dt);
            void showBonus(sf::RenderWindow& window);
            void createWave();
            void updateEnemies(sf::RenderWindow& window, sf::Time dt);
            void renderEnemies(sf::RenderWindow& window);
            void updateProjectiles(sf::Time dt);
            void renderProjectiles(sf::RenderWindow& window);
            void checkEnemyCollisions(sf::RenderWindow& window);
            void checkPlayerCollisions(sf::RenderWindow& window);
            void purgeEnemies();
            void pause();

        private:
            nasic::player hero;
            sf::Sprite m_life;
            sf::Uint32 m_numLives;
            std::vector<sf::Sprite> m_lives;
            std::vector<sf::Sprite>::iterator m_livesItr;
            sf::Uint32 m_score;
            sf::Uint32 m_timeBonusFactor;
            sf::Uint32 m_survivalBonus;
            sf::Uint32 m_accuracyBonus;
            sf::Uint32 m_hits;
            sf::Uint32 m_misses;
            sf::Text m_scoreLabel;
            sf::Text m_timeLabel;
            static sf::Uint32 m_levelstate;
            bool m_introStatus;
            std::list<nasic::enemy> m_enemies;
            nasic::enemy* m_enemy;
            sf::Time m_enemyMoveFrames;
            int m_wave;
            bool m_enemyInitStatus;
            bool enemyInitStatus(){return m_enemyInitStatus;};
            void setEnemyInitStatus(bool b){m_enemyInitStatus = b;};
            nasic::killer m_killer;
            nasic::particle m_particle;

            nasic::ammo* m_ammoPtr;

            std::list<nasic::ammo> m_playerAmmo;
            std::list<nasic::ammo>::iterator m_pAmmoIt;

            std::list<nasic::ammo> m_enemyAmmo;
            std::list<nasic::ammo> m_missileAmmo;
            std::list<nasic::ammo>::iterator m_missileIt;
            std::list<nasic::ammo>::iterator m_eAmmoIt;

            float m_winsizeX;
            float m_winsizeY;
            float m_scale;
            float m_scaleY;
            int m_dir;

            nasic::opstruct m_options;
            int m_musicVolume;
            int m_effectsVolume;
            int m_difficulty;

            sf::Font m_myfont;
            sf::Font m_hudFont;
            gui::button m_introMessage;
            gui::button m_bossMessage;
            sf::Text m_timeBonusMessage;
            sf::Text m_survivalBonusMessage;
            sf::Text m_accuracyBonusMessage;
            sf::Text m_pauseOverlay;

            sf::Sprite m_explosionSpr;

            sf::Sound m_explosion;
            sf::SoundBuffer m_hitBuff;
            sf::SoundBuffer m_explodBuff;

            sf::SoundBuffer m_shotBuff;
            sf::Sound m_shot;

            sf::SoundBuffer m_bgSndBuffer;
            sf::Sound m_bgSnd;

            sf::SoundBuffer m_pauseBuffer;
            sf::Sound m_pauseSnd;

            sf::Music m_music;

            thor::Animator<sf::Sprite, std::string> m_expAnim;
            thor::FrameAnimation m_expFrames;
    };
}

#endif // LEVEL_HPP
