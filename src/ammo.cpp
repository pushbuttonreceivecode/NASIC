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

nasic::ammo::ammo(sf::Uint32 type, sf::Vector2f initPos, const float& scale)
{
    m_type = type;
    m_scale = scale;

    this->setPosition(initPos);

    switch(m_type)
    {
        case warrior:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load warrior ammo texture"<<std::endl;
            }
        }
        break;
        case hunter:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load hunter ammo texture"<<std::endl;
            }
        }
        break;

        case guard:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load guard ammo texture"<<std::endl;
            }
        }
        break;

        case special_warrior:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load special warrior ammo texture"<<std::endl;
            }
        }
        break;

        case special_hunter:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load special hunter ammo texture"<<std::endl;
            }
        }
        break;

        case special_guard:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load special guard ammo texture"<<std::endl;
            }
        }
        break;

        case rhiians:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load rhiians ammo texture"<<std::endl;
            }
        }
        break;

        case agravu:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load agravu ammo texture"<<std::endl;
            }
        }
        break;

        case delsiriak:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load delsiriak ammo texture"<<std::endl;
            }
        }
        break;

        case gluorn:
        {
            if(!m_tex.loadFromFile("img/shot.png"))
            {
                std::cerr<<"Could not load gluorn ammo texture"<<std::endl;
            }
        }
        break;
    }
    this->setScale(m_scale, m_scale);
    this->setTexture(m_tex);
}

nasic::ammo::~ammo()
{
    //dtor
}

void nasic::ammo::fire(sf::Time dt)
{
    switch(m_type)
    {
        case warrior:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;
        case hunter:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case guard:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case special_warrior:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case special_hunter:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case special_guard:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case rhiians:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case agravu:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case delsiriak:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;

        case gluorn:
        {
            this->move(0.f, m_scale*(-400*dt.asSeconds()));
        }
        break;
    }
}
