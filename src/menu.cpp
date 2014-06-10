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

#include <menu.hpp>

nasic::menu::menu()
{
    using nasic::menu;

    m_choice = choice::s_uninitialized;

    //sound and settings
    //read option settings from file
    m_filename =  std::string("data/options.xml");

    if(!m_options.loadOptions(m_options, m_filename.c_str()))
    {
        std::cerr<<"Could not load options data from options.xml"<<std::endl;
        m_initialVol = 15;
        m_initialEff = 15;
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

    //load the transition sound file
    if(!m_transitionBuff.loadFromFile("sound/transition.wav"))
    {
        std::cerr<<"Could not load transition.wav."<<std::endl;
    }
    m_transition.setBuffer(m_transitionBuff);
    m_transition.setVolume(m_initialEff);
}

nasic::menu::~menu()
{

}

void nasic::menu::show(sf::RenderWindow& window)
{
    using nasic::menu;

    /////////////////////////////////////////////////////
    //***************IMPORTANT NOTE*********************
    //we're getting the video mode and creating a scale
    //variable for the drawables using 800 as the base
    //800*600 will be the smallest screen supported
    //*****KEEPING IN MIND****
    //we're blowing up small images...
    //this is not ideal for non-retro-looking sprites
    /////////////////////////////////////////////////////
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    float scale = mode.width/800.f;
    m_winsizeX = mode.width;
    m_winsizeY = mode.height;
    std::cout<<m_winsizeX<<"\n"<<m_winsizeY<<std::endl;
    m_scale = scale;

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps

    if(!m_choice == choice::s_uninitialized)
        return;

    m_choice = choice::s_undecided;

    //play the transition sound...
    m_transition.play();

    //set up menu resources
    sf::Font standardfont;
    if(!standardfont.loadFromFile("fonts/contb.ttf"))
    {
        std::cerr<<"Could not load contb.ttf."<<std::endl;
    }

    sf::Texture bgtex;
    if(!bgtex.loadFromFile("img/title.png"))
    {
        std::cerr<<"Could not load title.png."<<std::endl;
    }

    sf::Sprite title;
    title.setTexture(bgtex);
    title.setOrigin(title.getGlobalBounds().width/2.f, title.getGlobalBounds().height/2.f);
    title.setScale(m_scale, m_scale);
    title.setPosition(m_winsizeX/2.f, m_winsizeY/6.f);
    title.setColor(sf::Color(255,255,255,255));
    std::cout<<title.getPosition().x<<"\n"<<title.getPosition().y<<std::endl;

    sf::Text subtitle("...not another Space Invaders clone!", standardfont, 24);
    subtitle.setOrigin(subtitle.getGlobalBounds().width/2.f, subtitle.getGlobalBounds().height/2.f);
    subtitle.setScale(m_scale, m_scale);
    subtitle.setPosition(title.getPosition().x, title.getPosition().y + (title.getGlobalBounds().height * 1.25f)/m_scale);
    subtitle.setColor(sf::Color(0,255,255,255));

    sf::Text play("Play", standardfont, 52);
    play.setOrigin(0.f, play.getGlobalBounds().height/2.f);
    play.setScale(m_scale, m_scale);
    play.setPosition(scale*20.f, window.getSize().y/2.f);

    sf::Text info("Info", standardfont, 52);
    info.setOrigin(0.f, info.getGlobalBounds().height/2.f);
    info.setScale(m_scale, m_scale);
    info.setPosition(scale*20.f, play.getPosition().y + (play.getGlobalBounds().height * 2.0f));

    sf::Text options("Options", standardfont, 52);
    options.setOrigin(0.f, options.getGlobalBounds().height/2.f);
    options.setScale(m_scale, m_scale);
    options.setPosition(scale*20.f, info.getPosition().y + (info.getGlobalBounds().height * 2.0f));

    //set up hexgrid and starfield backgrounds
    nasic::hexgrid hex(window, nasic::hexgrid::hexStyle::colorful, scale);
    starfield stars(window, starfield::starStyle::starsAndPlanets, scale);

    //mask for fade in/out
    sf::RectangleShape mask;
    mask.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));
    mask.setPosition(0,0);
    mask.setFillColor(sf::Color(0,0,0,255));

    sf::SoundBuffer menubuff;
    if(!menubuff.loadFromFile("sound/select.wav"))
    {
        std::cerr<<"Could not load select.wav."<<std::endl;
    }
    sf::Sound menusound;
    menusound.setBuffer(menubuff);
    menusound.setVolume(m_initialEff);

    sf::Music menumusic;
    if(!menumusic.openFromFile("sound/beepboop.wav"))
    {
        std::cout<<"Could not open stream for beepboop.wav"<<std::endl;
    }
    menumusic.setVolume(m_initialVol);
    menumusic.setLoop(true);
    menumusic.play();

    //housekeeping variables for keeping track of the selection
    std::size_t selection = 0;
    std::size_t boundcount = 0;
    std::size_t hovercount = 0;

    bool running = true;
    sf::Event e;

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time fadeInFrames = sf::Time::Zero;
    sf::Time fadeOutFrames = sf::Time::Zero;
    sf::Time moveText = sf::Time::Zero;
    sf::Time colorChanger = sf::Time::Zero;
    int colorSwitch = 1;

    while(running)
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                m_choice = choice::s_quit;
                return;
            }

            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::Escape:
                {
                    m_choice = choice::s_quit;
                    return;
                }
                break;

                case sf::Keyboard::Return:
                {
                    return;
                }
                break;

                case sf::Keyboard::Up:
                {
                    if(selection <= 0)
                    {
                        boundcount++;
                        selection = 0;
                        std::cout<<"bound count : "<<boundcount<<std::endl;
                    }
                    else
                    {
                        selection--;
                        boundcount = 0;
                        std::cout<<"bound count : "<<boundcount<<std::endl;
                    }

                    if(boundcount == 0)
                        menusound.play();
                }
                break;

                case sf::Keyboard::Down:
                {
                    if(selection >= 2)
                    {
                        boundcount++;
                        selection = 2;
                        std::cout<<"bound count : "<<boundcount<<std::endl;
                    }
                    else
                    {
                        boundcount = 0;
                        selection++;
                        std::cout<<"bound count : "<<boundcount<<std::endl;
                    }

                    if(boundcount == 0)
                        menusound.play();
                }
                break;

                default:
                    break;
                }
            }

            sf::Vector2i mousepos = sf::Mouse::getPosition(window);
            if(mousepos.x >= play.getPosition().x
                    && mousepos.y >= play.getPosition().y
                    && mousepos.x <= play.getPosition().x + play.getGlobalBounds().width
                    && mousepos.y <= play.getPosition().y + play.getGlobalBounds().height)
            {

                //set selection to play
                //and keep track of mouse over text
                selection = 0;
                hovercount++;

                //play sound according to mouse hover events
                if(hovercount == 1)//because there are multiple mouse events we just want the first one
                {
                    menusound.play();
                }
                else
                {
                    //kill it after the first play
                    //so it doesn't do that aweful repeating thing
                    if(!menusound.getStatus() == sf::Sound::Playing)
                        menusound.stop();
                }

                if(e.type == sf::Event::MouseButtonPressed)
                {
                    return;
                }

            }

            else if(mousepos.x >= info.getPosition().x
                    && mousepos.y >= info.getPosition().y
                    && mousepos.x <= info.getPosition().x + info.getGlobalBounds().width
                    && mousepos.y <= info.getPosition().y + info.getGlobalBounds().height)
            {

                //set selection to info
                //and keep track of mouse over text
                selection = 1;
                hovercount++;


                //play sound according to mouse hover events
                if(hovercount == 1)//because there are multiple mouse events we just want the first one
                {
                    menusound.play();
                }
                else
                {
                    //kill it after the first play
                    //so it doesn't do that aweful repeating thing
                    if(!menusound.getStatus() == sf::Sound::Playing)
                        menusound.stop();
                }

                if(e.type == sf::Event::MouseButtonPressed)
                {
                    return;
                }
            }

            else if(mousepos.x >= options.getPosition().x
                    && mousepos.y >= options.getPosition().y
                    && mousepos.x <= options.getPosition().x + options.getGlobalBounds().width
                    && mousepos.y <= options.getPosition().y + options.getGlobalBounds().height)
            {

                //set selection to options
                //and keep track of mouse over text
                selection = 2;
                hovercount++;

                //play sound according to mouse hover events
                if(hovercount == 1)//because there are multiple mouse events we just want the first one
                {
                    menusound.play();
                }
                else
                {
                    //kill it after the first play
                    //so it doesn't do that aweful repeating thing
                    if(!menusound.getStatus() == sf::Sound::Playing)
                        menusound.stop();
                }

                if(e.type == sf::Event::MouseButtonPressed)
                {
                    return;
                }
            }
            else
                hovercount = 0;
        }

        //handle game ticks and return a fixed dt
        //for updates
        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            stars.update(window,TimePerFrame);

            fadeInFrames += TimePerFrame;

            if(fadeInFrames.asSeconds() < 1.f)
                mask.setFillColor(sf::Color(0,0,0,255-(unsigned int)interpolate::sineEaseIn(fadeInFrames.asSeconds(),0.f,255.f,1.f)));

            if(fadeInFrames.asSeconds() > 1.f)//delayed for fade in
            {
                moveText += TimePerFrame;//for fixed movement of text below...

                if(colorChanger.asSeconds() > 3.f)
                {
                    colorChanger = sf::Time::Zero;
                    colorSwitch *= -1;
                }

                else
                    colorChanger += TimePerFrame;

                float r = interpolate::sineEaseIn(colorChanger.asSeconds(),0.f,255.f,2.f);
                float g = interpolate::sineEaseIn(colorChanger.asSeconds(),0.f,255.f,2.f);
                float b = interpolate::sineEaseIn(colorChanger.asSeconds(),0.f,255.f,2.f);

                //visual indicators of selection made
                //as well as setting the choice enum accordingly
                //to trigger the proper state
                if(moveText.asSeconds() < 1.f)
                {
                    play.move(scale*(m_winsizeX*.01f)*interpolate::backEaseInOut(moveText.asSeconds(), 0.f, 1.f, 1.f), 0.f);
                    info.move(scale*(m_winsizeX*.01f)*interpolate::backEaseInOut(moveText.asSeconds(), 0.f, 1.f, 1.f), 0.f);
                    options.move(scale*(m_winsizeX*.01f)*interpolate::backEaseInOut(moveText.asSeconds(), 0.f, 1.f, 1.f), 0.f);
                }

                switch(selection)
                {
                case 0:
                {
                    m_choice = choice::s_play;

                    //set properties for the play option
                    play.setColor(sf::Color(255,0,255,255));

                    //set properties for the info option
                    info.setColor(sf::Color(255,255,255,255));

                    //set properties for the options option
                    options.setColor(sf::Color(255,255,255,255));
                }
                break;

                case 1:
                {
                    m_choice = choice::s_info;
                    play.setColor(sf::Color(255,255,255,255));

                    info.setColor(sf::Color(255,55,0,255));

                    options.setColor(sf::Color(255,255,255,255));
                }
                break;

                case 2:
                {
                    m_choice = choice::s_options;
                    play.setColor(sf::Color(255,255,255,255));

                    info.setColor(sf::Color(255,255,255,255));

                    options.setColor(sf::Color(0,255,0,255));
                }
                break;

                default:
                    break;
                }

                //subtract result from 255 to fade from cyan(?) to green
                if(colorSwitch == 1 && r < 255)
                    title.setColor(sf::Color(255, 255, 255-b, 255));
                if(colorSwitch == -1 && r < 255)
                    title.setColor(sf::Color(255, 255, b, 255));
            }
        }

        window.clear();

        window.draw(stars);
        window.draw(hex);
        window.draw(title);
        window.draw(subtitle);
        window.draw(play);
        window.draw(info);
        window.draw(options);
        window.draw(mask);
        window.display();
    }
    return;
}

//instantiate static members
nasic::menu::choice nasic::menu::m_choice = nasic::menu::choice::s_uninitialized;
