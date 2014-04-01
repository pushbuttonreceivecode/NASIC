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

#include <info.hpp>

nasic::info::info()
{
    //ctor
    using nasic::info;
    m_infostate = infostate::uninitialized;
}

nasic::info::~info()
{
    //dtor
}

void nasic::info::show(sf::RenderWindow& window)
{
    using nasic::info;

    /////////////////////////////////////////////////////
    //***************IMPORTANT NOTE*********************
    //we're getting the video mode and creating a scale
    //variable for the drawables using 800 as the base
    //800*600 will be the smallest screen supported
    //*****KEEPING IN MIND****
    //we're blowing up small images...
    //this is not ideal for non-retro-looking sprites
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    float scale = mode.width/800.f;

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps

    //bail if the options are not in uninitialized state
    if(!m_infostate == infostate::uninitialized)
        return;

    m_infostate = infostate::showing;

    //create info stuff
    sf::Font myfont;
    if(!myfont.loadFromFile("fonts/PressStart2P.ttf"))
    {
        std::cerr<<"Could not load PressStart2P.ttf"<<std::endl;
    }

    sf::Text information("", myfont, 32);
    information.setString("N.A.S.I.C. is a game\nheavily influenced by\nSpace Invaders.");
    information.setOrigin(information.getGlobalBounds().width/2.f, information.getGlobalBounds().height/2.f);
    information.setPosition(window.getSize().x/2.f, window.getSize().y/2.f);

    starfield stars(window, starfield::starStyle::smallStars, scale);

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);

    bool running = true;
    sf::Event e;
    while(running)
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
               m_infostate = infostate::done;
                return;
            }

            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                    case sf::Keyboard::Escape:
                    {
                        m_infostate = infostate::done;
                        return;
                    }
                    break;
                }
            }
        }

        //handle game ticks and return a fixed dt
        //for updates
        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            stars.update(window,TimePerFrame);
        }

        window.clear();
        window.draw(stars);
        window.draw(information);
        window.display();
    }
    return;
}

//instantiate static members
sf::Uint32 nasic::info::m_infostate = nasic::info::infostate::uninitialized;
