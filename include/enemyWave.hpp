#ifndef ENEMYWAVE_HPP
#define ENEMYWAVE_HPP

#include <enemy.hpp>
#include <ammo.hpp>
#include <player.hpp>
#include <interpolate.hpp>

namespace nasic
{
    //forward declaration of nasic::player
    //to avoid circular references (player includes enemyWave)
    class player;

    class enemyWave : public sf::Drawable, public sf::Transformable
    {
    public:
        enemyWave();
        ~enemyWave();

        void init(sf::RenderWindow& window, int rows, int columns, float scale);
        bool isEmpty(){return m_enemies.size() <= 0;};
        bool initStatus(){return m_enemyInitStatus;};
        void setInitStatus(bool b){m_enemyInitStatus = b;};
        void updateEnemies(sf::Time dt, float scale);
        void initProjectiles(sf::Time dt, float scale);

        sf::Uint32 checkPlayerCollisions(sf::RenderWindow& window, nasic::player& hero, float scale, sf::Sprite& explosion, sf::Sound& explode, sf::SoundBuffer& buffer, thor::Animator<sf::Sprite, std::string>& explosionAnim, std::string animation);
        void debugClearEnemies(){m_enemies.clear();};
        void purgeEnemies();

        void animate(sf::Time dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        enum hitList
        {
            miss = 0,
            agravu = 1,
            delsiriak = 2,
            gluorn = 3,
            rhiians = 4,
        };

    private:
        void updateProjectiles(sf::Time dt);

    public:
        std::list<nasic::enemy> m_enemies;//to give player class access for collision detection

    private:

        nasic::enemy* m_enemy;

        float m_offset;
        int m_dir;
        bool m_enemyInitStatus;

        nasic::ammo* m_ammoPtr;
        std::list<nasic::ammo> m_enemyAmmo;
        std::list<nasic::ammo>::iterator m_eAmmoIt;

        sf::Time m_enemyMoveFrames;
        sf::Time m_enemyFireFrames;
    };
}

#endif // ENEMYWAVE_HPP
