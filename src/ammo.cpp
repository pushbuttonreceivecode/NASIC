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

#include <ammo.hpp>

nasic::ammo::ammo()
{

}

nasic::ammo::ammo(sf::Uint32 type, sf::Vector2f initPos, const float& scale)
{
    m_type = type;
    m_scale = scale;
    m_velY = 0;//for missiles

    ////////////////////////////////////
    //initialize textures
    ////////////////////////////////////
    if(!m_shot.loadFromFile("img/shot.png"))
    {
        std::cerr<<"Could not load shot ammo texture"<<std::endl;
    }

    if(!m_fire.loadFromFile("img/fireshot.png"))
    {
        std::cerr<<"Could not load fireshot ammo texture"<<std::endl;
    }

    if(!m_slime.loadFromFile("img/slimeshot.png"))
    {
        std::cerr<<"Could not load slimeshot ammo texture"<<std::endl;
    }

    if(!m_spiral.loadFromFile("img/spiralshot.png"))
    {
        std::cerr<<"Could not load spiralshot ammo texture"<<std::endl;
    }

    if(!m_laser.loadFromFile("img/laser.png"))
    {
        std::cerr<<"Could not load laser ammo texture"<<std::endl;
    }

    if(!m_missile.loadFromFile("img/missile.png"))
    {
        std::cerr<<"Could not load laser ammo texture"<<std::endl;
    }

    this->setPosition(initPos);

    switch(m_type)
    {
        case nasic::ammo::ammoType::warrior:
        {
            this->setTexture(m_shot);
        }
        break;
        case nasic::ammo::ammoType::hunter:
        {
            this->setTexture(m_shot);
        }
        break;

        case nasic::ammo::ammoType::guard:
        {
            this->setTexture(m_shot);
        }
        break;

        case nasic::ammo::ammoType::special_warrior:
        {
            this->setTexture(m_shot);
        }
        break;

        case nasic::ammo::ammoType::special_hunter:
        {
            this->setTexture(m_shot);
        }
        break;

        case nasic::ammo::ammoType::special_guard:
        {
            this->setTexture(m_shot);
        }
        break;

        case nasic::ammo::ammoType::delsiriak:
        {
            this->setTexture(m_fire);
        }
        break;

        case nasic::ammo::ammoType::agravu:
        {
            this->setTexture(m_slime);
        }
        break;

        case nasic::ammo::ammoType::gluorn:
        {
            this->setTexture(m_laser);
        }
        break;

        case nasic::ammo::ammoType::rhiians:
        {
            this->setTexture(m_spiral);
        }
        break;

        case nasic::ammo::ammoType::missile:
        {
            this->setTexture(m_missile);
        }
        break;

        default:
            break;
    }

    this->setScale(m_scale, m_scale);
}

nasic::ammo::~ammo()
{

}

void nasic::ammo::fire(sf::Time dt)
{
    switch(m_type)
    {
        case nasic::ammo::ammoType::warrior:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;
        case nasic::ammo::ammoType::hunter:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::guard:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::special_warrior:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::special_hunter:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::special_guard:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::rhiians:
        {
            this->move(0.f, m_scale*(400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::agravu:
        {
            this->move(0.f, m_scale*(400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::delsiriak:
        {
            this->move(0.f, m_scale*(400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::gluorn:
        {
            this->move(0.f, m_scale*(400*dt.asSeconds()));
        }
        break;

        case nasic::ammo::ammoType::missile:
        {
            float terminalVelocity = 20.f;

            if(m_velY >= terminalVelocity)
                m_velY = terminalVelocity;
            else
                m_velY = m_scale*(interpolate::quarticEaseIn(dt.asSeconds(), 0.f,1000.f,2.f));

            this->move(0.f, m_velY);
        }
        break;

        default:
            break;
    }
}
