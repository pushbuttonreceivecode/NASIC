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

#include <intro.hpp>

nasic::intro::intro()
{
    //ctor
}

nasic::intro::~intro()
{
    //dtor
}

void nasic::intro::show(sf::RenderWindow& window)
{
    using nasic::intro;

    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps

    if(!m_introstate == introstate::s_uninitialized)
        return;

    m_introstate = introstate::s_playing;

    sf::Clock clock;

    //create the intro image
    sf::Texture tex;
    if(!tex.loadFromFile("img/intro.png"))
    {
        std::cerr<<"Could not load image."<<std::endl;
    }

    sf::Sprite sprite;
    sprite.setTexture(tex);
    sprite.setOrigin(sprite.getGlobalBounds().width/2.f, sprite.getGlobalBounds().height/4.f);
    sprite.setPosition(window.getSize().x/2.f, window.getSize().y/2.f);
    sprite.setTextureRect(sf::IntRect(0,0,800,230));

    while(m_introstate == introstate::s_playing
          && clock.getElapsedTime().asSeconds() < 5.f)
    {
        window.clear();

        if(clock.getElapsedTime().asSeconds() > 3.f)
        {
            sprite.setTextureRect(sf::IntRect(0,230,800,230));
        }

        window.draw(sprite);

        window.display();
    }
    return;
}

//instantiate static members
sf::Uint32 nasic::intro::m_introstate = s_uninitialized;
