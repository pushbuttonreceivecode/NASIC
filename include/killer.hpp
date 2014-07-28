#ifndef KILLER_HPP
#define KILLER_HPP

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <particle.hpp>
#include <interpolate.hpp>
#include <ammo.hpp>

namespace nasic
{
    //forward declaration of nasic::player
    //to avoid circular references (player includes killer)
    class player;

    class killer : public sf::Drawable, public sf::Transformable
    {
        public:
            killer();

            ~killer();

            void init(sf::RenderWindow& window, float scale);

            sf::Vector2f getAABB()
            {
                m_dimensions = sf::Vector2f(m_helmet.getGlobalBounds().width, m_helmet.getGlobalBounds().height);
                return m_dimensions;
            };

            void updateEyeColor(sf::Time dt);
            void initParticles(int disturbance);
            void updateParticles(sf::Time dt, float scale);

            void animateMandibles(sf::Time dt, float scale);

            sf::Vector2f leftCannonPosition(){return m_leftCannon.getPosition();};
            sf::Vector2f rightCannonPosition(){return m_rightCannon.getPosition();};
            sf::Vector2f leftEyeLaserPosition(){return sf::Vector2f(m_eyeGlow.getPosition().x + m_eyeGlow.getGlobalBounds().width/5.f, m_eyeGlow.getPosition().y);};
            sf::Vector2f rightEyeLaserPosition(){return sf::Vector2f(m_eyeGlow.getPosition().x + m_eyeGlow.getGlobalBounds().width - m_eyeGlow.getGlobalBounds().width/5.f, m_eyeGlow.getPosition().y);};
            sf::Vector2f leftGunPosition(){return m_leftEarring.getPosition();};
            sf::Vector2f rightGunPosition(){return m_rightEarring.getPosition();};

            void fireAmmo(sf::Time dt, float scaleX, float scaleY);
            sf::Uint32 checkCollisions(sf::RenderWindow& window, nasic::player& hero, float scaleX, float scaleY);

            sf::Vector2f getTargetPosition(){return sf::Vector2f(m_eyeGlow.getPosition().x, m_eyeGlow.getPosition().y);};
            sf::Vector2f getTargetAABB(){return sf::Vector2f(m_eyeGlow.getSize().x, m_eyeGlow.getSize().y);};

            void damage(int d){m_health -= d;};

            int getHealth(){return m_health;};

            void speak(sf::Time dt);

            void updateState();
            void toggleState(int s);//for debug purposes

            sf::Uint32 getState(){return m_killerState;};

            sf::Vector2f getPosition() const;

            void move(float offsetX, float offsetY);
            void updateMotion(sf::Time dt, float scaleX, float scaleY);

            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        public:
            enum killerState
            {
                normal,
                pissed,
                dead
            };

            enum collisionType
            {
                miss = 0,
                gun = 1,
                cannon = 2,
                missile = 3,
            };

        private:

            sf::Texture face;
            sf::Texture helmet;
            sf::Texture leftEar;
            sf::Texture rightEar;
            sf::Texture leftMandible;
            sf::Texture rightMandible;
            sf::Texture leftEarring;
            sf::Texture rightEarring;
            sf::Texture leftTooth;
            sf::Texture rightTooth;
            sf::Texture particleTexture;
            nasic::particle m_particle;
            std::list<nasic::particle> particles;
            std::list<nasic::particle>::iterator particleIt;

            sf::Sprite m_face;
            sf::Sprite m_helmet;
            sf::Sprite m_leftEar;
            sf::Sprite m_rightEar;
            sf::Sprite m_leftCannon;
            sf::Sprite m_rightCannon;
            sf::Sprite m_leftTooth;
            sf::Sprite m_rightTooth;
            sf::Sprite m_leftMandible;
            sf::Sprite m_rightMandible;
            sf::Sprite m_leftEarring;
            sf::Sprite m_rightEarring;
            sf::RectangleShape m_eyeGlow;

            sf::Uint32 m_killerState;
            int m_health;
            sf::Vector2f m_dimensions;
            sf::Time m_eyeColorFrames;
            int m_eyeColorSwitch;
            sf::Time m_motionFrames;
            int m_direction;
            sf::Time m_mandibleFrames;
            int m_mandibleRotationDirection;

            nasic::ammo* m_ammoPtr;
            std::list<nasic::ammo> m_enemyAmmo;
            std::list<nasic::ammo> m_missileAmmo;
            std::list<nasic::ammo>::iterator m_missileIt;
            std::list<nasic::ammo>::iterator m_eAmmoIt;
            sf::Time m_bossMandibleFrames;
            sf::Time m_bossCannonFrames;
            sf::Time m_bossMissileFrames;
            sf::Time m_bossGunFrames;
            sf::Time m_bossBurstFrames;
            sf::Time m_missileAmmoFrames;
            sf::Time m_missileBurstFrames;
            sf::Time m_particleFrames;

            sf::Sound m_vocals;
            sf::SoundBuffer m_laughBuffer;
            sf::SoundBuffer m_tauntBuffer;
            sf::SoundBuffer m_deadBuffer;
            sf::Time m_speechFrames;
            int m_speechSwitch;
            bool m_playSwitch;
            unsigned int m_speechCounter;
    };
}

#endif // KILLER_HPP
