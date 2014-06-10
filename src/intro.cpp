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

}

nasic::intro::~intro()
{

}

void nasic::intro::show(sf::RenderWindow& window)
{
    using nasic::intro;

    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    float scale = mode.width/800.f;
    float scaleY = mode.height/600.f;

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
    sprite.setTextureRect(sf::IntRect(0,0,800,230));
    sprite.setScale(scale/2,scale/2);
    sprite.setPosition(window.getSize().x/2.f, scale*-100);

    sf::RectangleShape mask;
    mask.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));
    mask.setPosition(0,0);
    mask.setFillColor(sf::Color(0,0,0,0));

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time switchFrames = sf::Time::Zero;
    sf::Time fadeFrames = sf::Time::Zero;
    sf::Time maskFrames = sf::Time::Zero;

    while(m_introstate == introstate::s_playing
          && clock.getElapsedTime().asSeconds() < 7.f)
    {
        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            if(clock.getElapsedTime().asSeconds() < 3.f)
                fadeFrames = sf::Time::Zero;
            else
                fadeFrames += TimePerFrame;

            switchFrames += TimePerFrame;

            maskFrames += TimePerFrame;

            if(clock.getElapsedTime().asSeconds() < 4.f)
            {
                if(switchFrames.asSeconds() < 1.f)
                    sprite.move(0.f, (scaleY*65.f)*interpolate::backEaseOut(switchFrames.asSeconds(),0.f,1.f,1.f));

                else if(switchFrames.asSeconds() > 1.f && switchFrames.asSeconds() < 2.f)
                    sprite.setColor(sf::Color(255,255,255,255-(unsigned int)interpolate::sineEaseIn(switchFrames.asSeconds(),0.f,255,1.f)));

                else if(switchFrames.asSeconds() > 3.f)
                {
                    sprite.setColor(sf::Color(255,255,255,0));
                    sprite.setTextureRect(sf::IntRect(0,230,800,230));
                }
            }

            else if(clock.getElapsedTime().asSeconds() > 4.f && clock.getElapsedTime().asSeconds() < 5.f)
            {
                sprite.setColor(sf::Color(255,255,255,(unsigned int)interpolate::sineEaseIn(fadeFrames.asSeconds(),0.f,255,1.f)));
            }

            else if(clock.getElapsedTime().asSeconds() > 5.f && clock.getElapsedTime().asSeconds() < 6.f)
            {
                sprite.setColor(sf::Color(255,255,255,255));
                maskFrames = sf::Time::Zero;//prepare the mask counter for a fade out
            }

            else if(clock.getElapsedTime().asSeconds() > 6.f)
            {
                mask.setFillColor(sf::Color(0,0,0,(unsigned int)interpolate::sineEaseIn(maskFrames.asSeconds(),0.f,255.f,1.f)));
                if(mask.getFillColor().a >= 255)
                    mask.setFillColor(sf::Color(0,0,0,255));
            }
        }

        window.clear();

        window.draw(sprite);
        window.draw(mask);

        window.display();
    }
    return;
}

//instantiate static members
sf::Uint32 nasic::intro::m_introstate = s_uninitialized;
