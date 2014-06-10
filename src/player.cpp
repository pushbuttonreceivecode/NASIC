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

void nasic::player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_spr, states);
}
