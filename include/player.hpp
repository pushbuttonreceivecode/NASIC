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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <THOR/Animation.hpp>
#include <killer.hpp>
#include <enemyWave.hpp>

namespace nasic
{
    //forward declaration of nasic::enemyWave
    //to avoid circular references (enemyWave includes player)
    class enemyWave;

    //forward declaration of nasic::killer
    //to avoid circular references (killer includes player)
    class killer;

    class player : public sf::Drawable, public sf::Transformable
    {
        public:
            player();
            player(sf::Uint32 type, const float& scale);
            ~player();

            void setPosition(float x, float y);
            sf::Vector2f getPosition(){return m_spr.getPosition();};
            void move(sf::Vector2f v, sf::Time dt);

            int getVel(){return m_data.speed;};

            sf::Uint32 getState();

            void setAmmo(sf::Uint32 ammo){m_ammo = ammo;};
            void initAmmo(bool shooting, float scale, sf::Sound& shot);

            void damage(int d);

            void heal(int h);

            void init(sf::RenderWindow& window);

            float getHealth(){return m_data.health;};

            sf::Vector2f getAABB();

            sf::Uint32 getType(){return m_type;};

            void update(sf::RenderWindow& window, sf::Time dt);

            void updateProjectiles(sf::Time dt);
            sf::Uint32 checkEnemyCollisions(sf::RenderWindow& window, std::list<nasic::enemy>& enemies, sf::Sprite& explosion, sf::Sound& explode, sf::SoundBuffer& buffer, sf::SoundBuffer& hitbuffer, thor::Animator<sf::Sprite, std::string>& explosionAnim, std::string animation, float scale);
            bool checkBossCollisions(nasic::killer& killer, sf::Sprite& explosion, sf::Sound& explode, sf::SoundBuffer& buffer, thor::Animator<sf::Sprite, std::string>& explosionAnim, std::string animation);

            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

            enum shipType
            {
                warrior,
                hunter,
                guard
            };

            enum shipState
            {
                normal,
                critical,
                invincible,
                dead
            };

            enum ammoType
            {
                gun,
                missle,
                nuke
            };

            enum hitList
            {
                miss = 0,
                agravu = 1,
                delsiriak = 2,
                gluorn = 3,
                rhiians = 4,
                hit = 5,
            };

        private:

            void updateState();

            void handleState(sf::RenderWindow& window);

        private:

            struct data
            {
                int health;
                int strength;
                int speed;
            };
            data m_data;
            data m_initialdata;

            sf::Clock deathClock;
            bool m_init;

            sf::Sprite m_spr;
            sf::Texture m_tex;

            sf::Uint32 m_state;
            sf::Uint32 m_type;
            sf::Uint32 m_ammo;

            nasic::ammo* m_ammoPtr;
            std::list<nasic::ammo> m_playerAmmo;
            std::list<nasic::ammo>::iterator m_pAmmoIt;
    };
}

#endif // PLAYER_HPP
