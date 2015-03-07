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

#include <youwin.hpp>

nasic::youwin::youwin()
{
    using nasic::youwin;
    m_winState = winState::uninitialized;

    //sound and settings
    //read option settings from file
    m_filename =  std::string("data/options.xml");
    if(!m_options.loadOptions(m_options, m_filename.c_str()))
    {
        std::cerr<<"Could not load options data from options.xml"<<std::endl;
        m_initialVol = 3;
        m_initialEff = 5;
        m_initialDif = 1;
    }
    else
    {
        std::cout<<m_options.m_volume<<"\n"<<m_options.m_effects<<"\n"<<m_options.m_difficulty<<std::endl;

        m_initialVol = m_options.m_volume;
        m_initialEff = m_options.m_effects;
        m_initialDif = m_options.m_difficulty;
    }

    //std::cout<<m_initialVol<<"\n"<<m_initialEff<<"\n"<<m_initialDif<<std::endl;

    //load the transition sound file
    //generated with musagi ;)
    if(!m_transitionBuff.loadFromFile("sound/winner.wav"))
    {
        std::cerr<<"Could not load winner.wav."<<std::endl;
    }
    m_transition.setBuffer(m_transitionBuff);
    m_transition.setVolume(m_initialEff);

    //load killer death-moan
    if(!m_killerDeath.loadFromFile("sound/noooo.wav"))
    {
        std::cerr<<"Could not load noooo.wav."<<std::endl;
    }
    m_killerScream.setBuffer(m_killerDeath);
    m_killerScream.setVolume(m_initialEff*1.5f);
}

nasic::youwin::~youwin()
{

}

void nasic::youwin::show(sf::RenderWindow& window)
{
    using nasic::youwin;

    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    float scale = mode.width/800.f;

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps

    //bail if the options are not in uninitialized state
    if(!m_winState == winState::uninitialized)
        return;

    m_winState = winState::showing;

    //play the transition sound...
    m_transition.play();

    //play killer scream
    m_killerScream.play();

    //set up the graphics
    sf::Font myfont;
    if(!myfont.loadFromFile("fonts/contb.ttf"))
    {
        std::cerr<<"Could not load contb.ttf"<<std::endl;
    }

    sf::Text message("A Winner is You!!!", myfont, 96);
    message.setOrigin(message.getGlobalBounds().width/2.f, message.getGlobalBounds().height/2.f);
    message.setScale(scale,scale);
    message.setPosition(window.getSize().x/2.f, window.getSize().y/2.f);
    message.setColor(sf::Color(255,0,0,255));

    sf::Texture tex;
    if(!tex.loadFromFile("img/killer.png"))
    {
        std::cerr<<"Could not load .png."<<std::endl;
    }

    sf::Sprite killer;
    killer.setTexture(tex);
    killer.setOrigin(killer.getGlobalBounds().width/2.f, killer.getGlobalBounds().height/2.f);
    killer.setPosition(window.getSize().x/2.f, window.getSize().y/2.f);
    killer.setScale(scale,scale);

    sf::RectangleShape eyeGlow;
    eyeGlow.setSize(sf::Vector2f(scale*100.f,scale*25.f));
    eyeGlow.setOrigin(eyeGlow.getGlobalBounds().width/2.f, eyeGlow.getGlobalBounds().height/2.f);
    eyeGlow.setFillColor(sf::Color(255,255,0,255));
    eyeGlow.setPosition(killer.getPosition().x, killer.getPosition().y*.95f);

    //starfield
    nasic::starfield stars(window, nasic::starfield::smallStars, scale);

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time messageFrames = sf::Time::Zero;
    sf::Time aliveFrames = sf::Time::Zero;
    int colorSwitch = 1;

    bool running = true;
    sf::Event e;
    while(running)
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                m_winState = winState::done;
                return;
            }

            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {

                case sf::Keyboard::Escape:
                {
                    m_winState = winState::done;
                    return;
                }
                break;

                default:
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
            aliveFrames += TimePerFrame;

            stars.update(window, TimePerFrame);

            if(messageFrames.asSeconds() > 2.f)
            {
                messageFrames = sf::Time::Zero;
                colorSwitch *= -1;
            }

            else
                messageFrames += TimePerFrame;

            float r = interpolate::sineEaseIn(messageFrames.asSeconds(),0.f,255.f,2.f);
            float g = interpolate::sineEaseIn(messageFrames.asSeconds(),0.f,255.f,2.f);
            float b = interpolate::sineEaseIn(messageFrames.asSeconds(),0.f,255.f,2.f);

            if(colorSwitch == -1 && r < 255)
            {
                message.setColor(sf::Color(255,(unsigned int)g,(unsigned int)b,255));
                eyeGlow.setFillColor(sf::Color(255,255-(unsigned int)g,0,(unsigned int)b));
            }

            if(colorSwitch == 1 && r < 255)
            {
                message.setColor(sf::Color(255,255-(unsigned int)g,255-(unsigned int)b,255));
                eyeGlow.setFillColor(sf::Color(255,(unsigned int)g,0,255-(unsigned int)b));
            }
        }

        if(aliveFrames.asSeconds() > 10.f)
        {
            m_winState = winState::done;
            return;
        }
        else
            m_winState = winState::showing;

        window.clear();
        window.draw(stars);
        window.draw(eyeGlow);
        window.draw(killer);
        window.draw(message);
        window.display();
    }
    return;
}
