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

#include <player.hpp>

nasic::player::player()
{
    m_init = false;
}

nasic::player::player(sf::Uint32 type, const float& scale)
{
    using nasic::player;
    m_type = type;

    switch(m_type)
    {
    case shipType::warrior:
    {
        m_data.health = 100;
        m_data.speed = 400;
        m_data.strength = 20;

        m_initialdata = m_data;

        if(!m_tex.loadFromFile("img/warrior.png"))
        {
            std::cerr<<"Could not load warriors.png"<<std::endl;
        }

        //pass the texture to the sprite
        m_spr.setTexture(m_tex);
    }
    break;

    case shipType::hunter:
    {
        m_data.health = 150;
        m_data.speed = 300;
        m_data.strength = 30;

        m_initialdata = m_data;

        if(!m_tex.loadFromFile("img/hunter.png"))
        {
            std::cerr<<"Could not load hunter.png"<<std::endl;
        }

        //pass the texture to the sprite
        m_spr.setTexture(m_tex);
    }
    break;

    case shipType::guard:
    {
        m_data.health = 200;
        m_data.speed = 200;
        m_data.strength = 50;

        m_initialdata = m_data;

        if(!m_tex.loadFromFile("img/guard.png"))
        {
            std::cerr<<"Could not load guard.png"<<std::endl;
        }
        //pass the texture to the sprite
        m_spr.setTexture(m_tex);
    }
    break;

    default:
        break;
    }
    m_spr.setScale(scale, scale);
    //std::cout<<"Player scaled to: "<<m_spr.getScale().x<<","<<m_spr.getScale().y<<std::endl;
}

nasic::player::~player()
{

}

void nasic::player::setPosition(float x, float y)
{
    using nasic::player;
    m_spr.setPosition(x,y);
}

void nasic::player::move(sf::Vector2f v, sf::Time dt)
{
    m_spr.move(dt.asSeconds()*v);
}

sf::Uint32 nasic::player::getState()
{
    using nasic::player;

    //return state for updates outside of the player class
    return m_state;
}

void nasic::player::damage(int d)
{
    m_data.health -= d;
}

void nasic::player::heal(int h)
{
    m_data.health += h;
}

sf::Vector2f nasic::player::getAABB()
{
    using nasic::player;
    sf::Vector2f val = sf::Vector2f(0.f,0.f);
    switch(m_type)
    {
    case shipType::warrior:
    {
        val = sf::Vector2f(46.f,46.f);
    }
    break;

    case shipType::hunter:
    {
        val = sf::Vector2f(23.f,23.f);
    }
    break;

    case shipType::guard:
    {
        val = sf::Vector2f(24.f,27.f);
    }
    break;

    default:
        break;
    }

    return val;
}

void nasic::player::initAmmo(bool shooting, float scale, sf::Sound& shot)
{
    if(shooting && m_playerAmmo.size() < 2)
    {
        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::warrior, sf::Vector2f(m_spr.getPosition().x + m_spr.getGlobalBounds().width/2.f, m_spr.getPosition().y), scale);
        m_playerAmmo.push_back(*m_ammoPtr);
        //std::cout<<m_playerAmmo.size()<<std::endl;

        shot.play();
    }
}

void nasic::player::updateState()
{
    using nasic::player;
    switch(m_type)
    {
    case shipType::warrior:
    {
        if(m_data.health >= m_initialdata.health * .3f)
            m_state = shipState::normal;
        else if(0 < m_data.health && m_data.health < m_initialdata.health * .3f)
            m_state = shipState::critical;
        else if(m_data.health <= 0)
            m_state = shipState::dead;
    }
    break;

    case shipType::hunter:
    {
        if(m_data.health >= m_initialdata.health * .3f)
            m_state = shipState::normal;
        else if(0 < m_data.health && m_data.health < m_initialdata.health * .3f)
            m_state = shipState::critical;
        else if(m_data.health <= 0)
            m_state = shipState::dead;
    }
    break;

    case shipType::guard:
    {
        if(m_data.health >= m_initialdata.health * .3f)
            m_state = shipState::normal;
        else if(0 < m_data.health && m_data.health < m_initialdata.health * .3f)
            m_state = shipState::critical;
        else if(m_data.health <= 0)
            m_state = shipState::dead;
    }
    break;

    default:
        break;
    }
}

void nasic::player::handleState(sf::RenderWindow& window)
{
    using nasic::player;
    switch(m_state)
    {
    case shipState::normal:
    {
        m_data.speed = m_data.speed;
        m_spr.setColor(sf::Color(255,255,255,255));
    }
    break;

    case shipState::critical:
    {
        m_spr.setColor(sf::Color(255,0,0,255));
        m_data.speed = m_initialdata.speed*0.75f;
    }
    break;

    case shipState::dead:
    {
        if(!m_init)
            init(window);
    }
    break;

    default:
        break;
    }
}

void nasic::player::init(sf::RenderWindow& window)
{
    m_data.speed = m_data.speed;
    m_spr.setColor(sf::Color(255,255,255,255));
    m_spr.setPosition(window.getSize().x/2.f, window.getSize().y/1.25f);
    m_state = shipState::normal;
    m_init = true;
}

void nasic::player::update(sf::RenderWindow& window, sf::Time dt)
{
    using nasic::player;
    updateState();
    handleState(window);
}

void nasic::player::updateProjectiles(sf::Time dt)
{
    for(m_pAmmoIt = m_playerAmmo.begin(); m_pAmmoIt != m_playerAmmo.end(); ++m_pAmmoIt)
    {
        m_pAmmoIt->fire(dt);
    }
}

sf::Uint32 nasic::player::checkEnemyCollisions(sf::RenderWindow& window, std::list<nasic::enemy>& enemies, sf::Sprite& explosion, sf::Sound& explode, sf::SoundBuffer& buffer, sf::SoundBuffer& hitbuffer, thor::Animator<sf::Sprite, std::string>& explosionAnim, std::string animation, float scale)
{
    sf::Uint32 tempHit;

    std::list<nasic::ammo>::iterator ammoIt;
    for(ammoIt = m_playerAmmo.begin(); ammoIt != m_playerAmmo.end(); ++ammoIt)
    {
        if((*ammoIt).getPosition().y < 0.f)//erase the ammo if it leaves the screen
        {
            ammoIt = m_playerAmmo.erase(ammoIt);

            //std::cout<<m_playerAmmo.size()<<std::endl;
            tempHit = nasic::player::hitList::miss;
        }
    }

    std::list<nasic::enemy>::iterator enemIt;
    for(enemIt = enemies.begin(); enemIt != enemies.end(); ++enemIt)
    {
        std::list<nasic::ammo>::iterator pAmmoIt;
        for(pAmmoIt = m_playerAmmo.begin(); pAmmoIt != m_playerAmmo.end(); ++pAmmoIt)
        {
            if(pAmmoIt->getPosition().y >= enemIt->getPosition().y
                    && pAmmoIt->getPosition().y <= enemIt->getPosition().y + scale*enemIt->getAABB().y
                    && pAmmoIt->getPosition().x >= enemIt->getPosition().x
                    && pAmmoIt->getPosition().x <= enemIt->getPosition().x + scale*enemIt->getAABB().x)
            {
                //erase the bullet from the list
                pAmmoIt = m_playerAmmo.erase(pAmmoIt);

                enemIt->damage(1);//damage the enemy

                if(enemIt->getState() == nasic::enemy::enemyState::dead)
                {
                    //set the explosion position to the enemy location
                    explosion.setPosition(enemIt->getPosition().x, enemIt->getPosition().y);

                    //set the explosion sound buffer and play it
                    explode.setBuffer(buffer);
                    explode.play();

                    //play the explosion animation
                    explosionAnim.playAnimation(animation, false);

                    //erase the enemy from the list
                    enemIt = enemies.erase(enemIt);

                    //return results for updating the score
                    switch(enemIt->getType())
                    {
                    case nasic::enemy::enemyType::Agravu:
                    {

                        tempHit = nasic::player::hitList::agravu;
                    }
                    break;

                    case nasic::enemy::enemyType::Delsiriak:
                    {
                        tempHit = nasic::player::hitList::delsiriak;
                    }
                    break;

                    case nasic::enemy::enemyType::Gluorn:
                    {
                        tempHit = nasic::player::hitList::gluorn;
                    }
                    break;

                    case nasic::enemy::enemyType::Rhiians:
                    {
                        tempHit = nasic::player::hitList::rhiians;
                    }
                    break;

                    default:
                        break;
                    }
                }

                else if(enemIt->getState() != nasic::enemy::enemyState::dead)
                {
                    //no score update, but we keep track of the hits ;)
                    explode.setBuffer(hitbuffer);
                    explode.play();

                    tempHit = nasic::player::hitList::hit;
                }
            }
        }
    }

    return tempHit;
}

bool nasic::player::checkBossCollisions(nasic::killer& killer, sf::Sprite& explosion, sf::Sound& explode, sf::SoundBuffer& buffer, thor::Animator<sf::Sprite, std::string>& explosionAnim, std::string animation)
{
    std::list<nasic::ammo>::iterator playerAmmoIt;
    for(playerAmmoIt = m_playerAmmo.begin(); playerAmmoIt != m_playerAmmo.end(); ++playerAmmoIt)
    {
        if(playerAmmoIt->getPosition().y < killer.getTargetPosition().y + killer.getTargetAABB().y
                && playerAmmoIt->getPosition().y > killer.getTargetPosition().y
                && playerAmmoIt->getPosition().x < killer.getTargetPosition().x + killer.getTargetAABB().x
                && playerAmmoIt->getPosition().x > killer.getTargetPosition().x)
        {
            //set the explosion position to the enemy location
            explosion.setPosition(playerAmmoIt->getPosition().x, playerAmmoIt->getPosition().y);

            //set the explosion sound buffer and play it
            explode.setBuffer(buffer);
            explode.play();

            //play the explosion animation
            explosionAnim.playAnimation(animation, false);

            killer.damage(2);//this is arbitrary, adjust accordingly to alter the difficulty
            playerAmmoIt = m_playerAmmo.erase(playerAmmoIt);
            return true;
        }

        else if(playerAmmoIt->getPosition().y < 0.f)
        {
            playerAmmoIt = m_playerAmmo.erase(playerAmmoIt);
            return false;
        }
    }

    return false;
}

void nasic::player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_spr, states);

    //draw player's ammo
    std::list<nasic::ammo>::const_iterator pAmmoIt;
    for(pAmmoIt = m_playerAmmo.begin(); pAmmoIt != m_playerAmmo.end(); ++pAmmoIt)
    {
        target.draw(*pAmmoIt, states);
    }
}
