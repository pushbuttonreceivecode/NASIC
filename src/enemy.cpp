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

#include <enemy.hpp>

nasic::enemy::enemy()
{

}

nasic::enemy::enemy(sf::Uint32 type, const float& scale)
{
    //set up the basics
    using nasic::enemy;
    m_state = enemyState::normal;
    m_type = type;

    m_scale = scale;

    //initialize enemy data
    //conditional on the input enemy type
    switch(m_type)
    {
        case enemyType::Rhiians:
        {
            m_data.health = 2;
            m_data.strength = 2;

            if(!m_tex.loadFromFile("img/Rhiians.png"))
            {
                std::cerr<<"Could not load Rhiians.png"<<std::endl;
            }

            //pass the texture to the sprite
            this->setTexture(m_tex);

            //set up frames for the Rhiians ship animation
            addFrames(m_frames, 45, 46, 0, 0, 6);
            m_animator.addAnimation("fly", m_frames, sf::seconds(0.5f));
        }
        break;

        case enemyType::Agravu:
        {
            m_data.health = 1;
            m_data.strength = 1;

            if(!m_tex.loadFromFile("img/Agravu.png"))
            {
                std::cerr<<"Could not load Agravu.png"<<std::endl;
            }

            //pass the texture to the sprite
            this->setTexture(m_tex);

            //set up frames for the Agravu ship animation
            addFrames(m_frames, 23, 23, 0, 0, 2);
            m_animator.addAnimation("fly", m_frames, sf::seconds(0.5f));
        }
        break;

        case enemyType::Delsiriak:
        {
            m_data.health = 1;
            m_data.strength = 1;

            if(!m_tex.loadFromFile("img/Delsiriak.png"))
            {
                std::cerr<<"Could not load Delsiriak.png"<<std::endl;
            }

            //pass the texture to the sprite
            this->setTexture(m_tex);

            //set up frames for the Delsiriak ship animation
            addFrames(m_frames, 21, 24, 0, 0, 2);
            m_animator.addAnimation("fly", m_frames, sf::seconds(0.5f));
        }
        break;

        case enemyType::Gluorn:
        {
            m_data.health = 2;
            m_data.strength = 2;

            if(!m_tex.loadFromFile("img/Gluorn.png"))
            {
                std::cerr<<"Could not load Gluorn.png"<<std::endl;
            }

            //pass the texture to the sprite
            this->setTexture(m_tex);

            //set up frames for the Gluorn ship animation
            addFrames(m_frames, 37, 35, 0, 0, 2);
            m_animator.addAnimation("fly", m_frames, sf::seconds(0.5f));
        }
        break;

        default:
            break;
    }
    this->setScale(m_scale,m_scale);
    //std::cout<<"Enemy scaled to: "<<m_spr.getScale().x<<","<<m_spr.getScale().y<<std::endl;
    if(!m_animator.isPlayingAnimation())
        m_animator.playAnimation("fly",true);
}

nasic::enemy::~enemy()
{

}

sf::Uint32 nasic::enemy::getState()
{
    using nasic::enemy;

    //return state for updates outside of the enemy class
    return m_state;
}

sf::Vector2f nasic::enemy::getAABB()
{
    switch(m_type)
    {
        case enemyType::Rhiians:
        {
            m_aabb = sf::Vector2f(45.f,45.f);
        }
        break;

        case enemyType::Agravu:
        {
            m_aabb = sf::Vector2f(23.f,23.f);
        }
        break;

        case enemyType::Delsiriak:
        {
            m_aabb = sf::Vector2f(21.f,24.f);
        }
        break;

        case enemyType::Gluorn:
        {
            m_aabb = sf::Vector2f(37.f,35.f);
        }
        break;
    }
    return m_aabb;
}

void nasic::enemy::damage(int d)
{
    m_data.health -= d;
}

void nasic::enemy::updateState()
{
    using nasic::enemy;
    switch(m_type)
    {
        case enemyType::Rhiians:
        {
            if(m_data.health == 2)
                m_state = enemyState::normal;
            else if(m_data.health == 1)
                m_state = enemyState::pissed;
            else if(m_data.health == 0)
                m_state = enemyState::dead;
        }
        break;

        case enemyType::Agravu:
        {
            if(m_data.health == 1)
                m_state = enemyState::normal;
            else
                m_state = enemyState::dead;
        }
        break;

        case enemyType::Delsiriak:
        {
            if(m_data.health == 1)
                m_state = enemyState::normal;
            else
                m_state = enemyState::dead;
        }
        break;

        case enemyType::Gluorn:
        {
            if(m_data.health == 2)
                m_state = enemyState::normal;
            else if(m_data.health == 1)
                m_state = enemyState::pissed;
            else if(m_data.health == 0)
                m_state = enemyState::dead;
        }
        break;
    }
}

void nasic::enemy::handleState()
{
    using nasic::enemy;
    switch(m_state)
    {
        case enemyState::normal:
        {
            //do nothing...
        }
        break;

        case enemyState::pissed:
        {
            this->setColor(sf::Color(255,100,100,255));
        }
        break;

        default:
            break;
    }
}

void nasic::enemy::animate(sf::Time dt)
{
    m_animator.update(dt);
    m_animator.animate(*this);
}

void nasic::enemy::update(sf::Time dt)
{
    handleState();
    updateState();

    animate(dt);
}

void nasic::enemy::addFrames(thor::FrameAnimation& animation, int w, int h, int y, int xFirst, int xLast, float duration)
{
	const int step = (xFirst < xLast) ? +1 : -1;
	xLast += step; // so xLast is excluded in the range

	for (int x = xFirst; x != xLast; x += step)
		animation.addFrame(duration, sf::IntRect(w*x, h*y, w, h));
}
