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

#include <options.hpp>

nasic::options::options()
{
    //ctor
    //reset the state each time the object is created
    using nasic::options;
    m_optionstate = optionstate::s_uninitialized;

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

    std::cout<<m_initialVol<<"\n"<<m_initialEff<<"\n"<<m_initialDif<<std::endl;
}

nasic::options::~options()
{
    //dtor
}

void nasic::options::show(sf::RenderWindow& window)
{
    using nasic::options;

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
    if(!m_optionstate == optionstate::s_uninitialized)
        return;

    //create info stuff
    sf::Color bg = sf::Color(255,150,0,200);
    sf::Color none = sf::Color(0,0,0,0);
    sf::Color darkred = sf::Color(100,0,0,255);

    sf::Vector2f volpos = sf::Vector2f(window.getSize().x/2.f,window.getSize().y/4.f);
    sf::Vector2f effpos = sf::Vector2f(window.getSize().x/2.f,window.getSize().y/2.f);
    sf::Vector2f diffpos = sf::Vector2f(window.getSize().x/2.f,window.getSize().y/1.25f);
    gui::optionbox vol("Volume", volpos, 20, window);
    gui::optionbox eff("Effects Volume", effpos, 20, window);
    gui::optionbox dif("Difficulty", diffpos, 3, window);

    sf::Uint32 focus = 0;

    sf::Font myfont;
    if(!myfont.loadFromFile("fonts/contb.ttf"))
    {
        std::cerr<<"Could not load contb.ttf"<<std::endl;
    }

    sf::Text title("Options", myfont, 48);
    title.setColor(darkred);
    title.setPosition(window.getSize().x * 0.01f, window.getSize().y * 0.01f);

    sf::ConvexShape bgBox = thor::Shapes::roundedRect(sf::Vector2f(window.getSize().x, window.getSize().y), 0.f, sf::Color(bg), 0.f, sf::Color(none));
    bgBox.setPosition(0.f,0.f);

    nasic::starfield stars(window,nasic::starfield::starStyle::smallStars, scale);

    nasic::hexgrid hex(window, nasic::hexgrid::hexStyle::translucent, scale);

    m_optionstate = optionstate::s_settingoptions;

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);

    vol.setVal(m_initialVol);
    eff.setVal(m_initialEff);
    dif.setVal(m_initialDif);

    sf::SoundBuffer menubuff;
    if(!menubuff.loadFromFile("sound/select.wav"))
    {
        std::cerr<<"Could not load select.wav."<<std::endl;
    }

    sf::Sound menusound;
    menusound.setBuffer(menubuff);
    menusound.setVolume(m_initialVol*5.f);

    sf::Music menumusic;
    if(!menumusic.openFromFile("sound/title.ogg"))
    {
        std::cout<<"Could not open stream for title.ogg"<<std::endl;
    }
    menumusic.setVolume(m_initialEff*5.f);
    menumusic.play();

    bool running = true;
    sf::Event e;
    while(running)
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                m_options.saveOptions(m_options,m_filename.c_str());
                m_optionstate = optionstate::s_done;
                return;
            }

            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                    case sf::Keyboard::Escape:
                    {
                        m_options.saveOptions(m_options,m_filename.c_str());
                        m_optionstate = optionstate::s_done;
                        return;
                    }
                    break;

                    case sf::Keyboard::Up:
                    {
                        if(focus > 0)
                        {
                            focus--;
                            menusound.play();
                        }

                    }
                    break;

                    case sf::Keyboard::Down:
                    {
                        if(focus < 2)
                        {
                            focus++;
                            menusound.play();
                        }
                    }
                    break;

                    case sf::Keyboard::Left:
                    {
                        menusound.play();
                    }
                    break;

                    case sf::Keyboard::Right:
                    {
                        menusound.play();
                    }
                    break;

                    case sf::Keyboard::Tab:
                    {
                        menusound.play();
                        if(focus >= 2)
                            focus = 0;
                        else
                            focus++;
                    }

                    default:
                        break;
                }
            }
            vol.update(e, window);
            eff.update(e, window);
            dif.update(e, window);
        }

        //handle game ticks and return a fixed dt
        //for updates
        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            stars.update(window, TimePerFrame);
        }

        m_vol = vol.getVal() * 5.f;
        menumusic.setVolume(m_vol);
        m_options.m_volume = vol.getVal();

        switch(focus)
        {
            case 0://volume is on focus
            {
                vol.setFocus(true);
                eff.setFocus(false);
                dif.setFocus(false);
            }
            break;

            case 1://effects is on focus
            {
                vol.setFocus(false);
                eff.setFocus(true);
                dif.setFocus(false);
            }
            break;

            case 2://difficulty is on focus
            {
                vol.setFocus(false);
                eff.setFocus(false);
                dif.setFocus(true);
            }
            break;

            default:
                break;
        }

        if(eff.getDownClicks() == 1)
            menusound.play();

        if(eff.getUpClicks() == 1)
            menusound.play();

        m_eff = eff.getVal() * 5.f;
            menusound.setVolume(m_eff);

        m_options.m_effects = eff.getVal();
        m_options.m_difficulty = dif.getVal();

        window.clear();
        window.draw(stars);
        window.draw(bgBox);
        window.draw(hex);
        window.draw(vol);
        window.draw(eff);
        window.draw(dif);
        window.draw(title);
        window.display();
    }
    std::cout<<m_options.m_volume<<"\n"<<m_options.m_effects<<"\n"<<m_options.m_difficulty<<std::endl;
    m_options.saveOptions(m_options,m_filename.c_str());
    return;
}

//instantiate static members
sf::Uint32 nasic::options::m_optionstate = nasic::options::optionstate::s_uninitialized;
