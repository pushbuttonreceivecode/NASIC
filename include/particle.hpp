#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>
#include <interpolate.hpp>

namespace nasic
{
    class particle : public sf::Sprite
    {
        public:
            particle();
            ~particle();

            bool isAlive(){return m_alive;};

            void decAlpha(sf::Time dt)
            {
                if(m_frames.asSeconds() > 1.f)
                    m_frames = sf::Time::Zero;
                else
                    m_frames += dt;

                if(m_frames.asSeconds() < 1.f)
                    this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 255-(unsigned int)interpolate::sineEaseIn(m_frames.asSeconds(),0.f,255.f,1.f)));

                else
                {
                    this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 0));
                    m_alive = false;
                }
            };

        private:
            sf::Time m_frames;
            bool m_alive;
    };
}

#endif // PARTICLE_HPP
