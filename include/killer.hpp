#ifndef KILLER_HPP
#define KILLER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ammo.hpp>

namespace nasic
{
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

            void updateEyeColor(unsigned int g, unsigned int a);

            void animateMandibles(float angle);

            sf::Vector2f leftCannonPosition(){return m_leftCannon.getPosition();};
            sf::Vector2f rightCannonPosition(){return m_rightCannon.getPosition();};
            sf::Vector2f leftEyeLaserPosition(){return m_eyeGlow.getPosition();};
            sf::Vector2f rightEyeLaserPosition(){return sf::Vector2f(m_eyeGlow.getPosition().x + m_eyeGlow.getGlobalBounds().width, m_eyeGlow.getPosition().y);};
            sf::Vector2f leftGunPosition(){return m_leftEarring.getPosition();};
            sf::Vector2f rightGunPosition(){return m_rightEarring.getPosition();};

            sf::Vector2f getTargetPosition(){return sf::Vector2f(m_eyeGlow.getPosition().x, m_eyeGlow.getPosition().y);};
            sf::Vector2f getTargetAABB(){return sf::Vector2f(m_eyeGlow.getSize().x, m_eyeGlow.getSize().y);};

            void damage(int d){m_health -= d;};

            int getHealth(){return m_health;};

            void updateState();

            sf::Uint32 getState(){return m_killerState;};

            sf::Vector2f getPosition() const;

            void move(float offsetX, float offsetY);

            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        public:
            enum killerState
            {
                normal,
                pissed,
                dead
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
    };
}

#endif // KILLER_HPP
