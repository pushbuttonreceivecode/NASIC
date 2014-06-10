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
    using nasic::info;
    m_infostate = infostate::uninitialized;

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

nasic::info::~info()
{

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

    //play the transition sound...
    m_transition.play();

    //create info stuff
    sf::Font myfont;
    if(!myfont.loadFromFile("fonts/contb.ttf"))
    {
        std::cerr<<"Could not load contb.ttf"<<std::endl;
    }

    sf::Text information("", myfont, 20);
    information.setOrigin(information.getGlobalBounds().width/2.f, information.getGlobalBounds().height/2.f);
    information.setScale(scale,scale);

    //graphics...
    sf::Texture tex;
    if(!tex.loadFromFile("img/star.png"))
    {
        std::cerr<<"Could not load star.png"<<std::endl;
    }

    //set up the parallax diagonal scrolling images
    m_bgOne.setTexture(tex);
    m_bgTwo.setTexture(tex);
    m_bgThree.setTexture(tex);
    m_bgFour.setTexture(tex);
    m_bgOne.setOrigin(0.f,0.f);
    m_bgTwo.setOrigin(0.f,0.f);
    m_bgThree.setOrigin(0.f,0.f);
    m_bgFour.setOrigin(0.f,0.f);
    m_bgOne.setScale(scale,scale);
    m_bgTwo.setScale(scale,scale);
    m_bgThree.setScale(scale,scale);
    m_bgFour.setScale(scale,scale);
    float w = m_bgOne.getGlobalBounds().width;
    float h = m_bgOne.getGlobalBounds().height;
    m_bgOne.setPosition(0.f,0.f);
    m_bgTwo.setPosition(w,-h);
    m_bgThree.setPosition(0.f,-h);
    m_bgFour.setPosition(w-1,0.f-1);

    //hexgrid
    nasic::hexgrid hex(window,nasic::hexgrid::hexStyle::translucent,scale);

    //left side panel
    thor::ConcaveShape panel;
    panel.setPointCount(7);
    panel.setPoint(0, sf::Vector2f(0.f, 0.f));
    panel.setPoint(1, sf::Vector2f(window.getSize().x/2.f, 0.f));
    panel.setPoint(2, sf::Vector2f(window.getSize().x/2.5f, window.getSize().y/8.f));
    panel.setPoint(3, sf::Vector2f(window.getSize().x/3.f, window.getSize().y/10.f));
    panel.setPoint(4, sf::Vector2f(window.getSize().x/8.f, window.getSize().y/6.f));
    panel.setPoint(5, sf::Vector2f(window.getSize().x/16.f, window.getSize().y));
    panel.setPoint(6, sf::Vector2f(0.f, window.getSize().y));
    panel.setFillColor(sf::Color(255,0,255,200));
    //panel.setScale(scale,scale);

    //bottom right corner
    thor::ConcaveShape corner;
    corner.setPointCount(4);
    corner.setPoint(0, sf::Vector2f(window.getSize().x, window.getSize().y));
    corner.setPoint(1, sf::Vector2f(window.getSize().x*.75f, window.getSize().y));
    corner.setPoint(2, sf::Vector2f(window.getSize().x*.9f, window.getSize().y*.95f));
    corner.setPoint(3, sf::Vector2f(window.getSize().x, window.getSize().y*.8f));
    corner.setFillColor(sf::Color(0,255,255,200));
    //corner.setScale(scale,scale);

    //middle message canvas
    thor::ConcaveShape canvas;
    canvas.setPointCount(7);
    canvas.setPoint(0, sf::Vector2f(window.getSize().x/6.5f, window.getSize().y/5.f));
    canvas.setPoint(1, sf::Vector2f(window.getSize().x/3.5f, window.getSize().y/4.f));
    canvas.setPoint(2, sf::Vector2f(window.getSize().x/1.25f, window.getSize().y/6.f));
    canvas.setPoint(3, sf::Vector2f(window.getSize().x/1.3f, window.getSize().y/1.5f));
    canvas.setPoint(4, sf::Vector2f(window.getSize().x/1.25f, window.getSize().y/1.1f));
    canvas.setPoint(5, sf::Vector2f(window.getSize().x/7.f, window.getSize().y/1.1f));
    canvas.setPoint(6, sf::Vector2f(window.getSize().x/6.f, window.getSize().y/2.f));
    canvas.setFillColor(sf::Color(200,100,0,200));
    information.setPosition(canvas.getPoint(0).x + scale*15.f, canvas.getPoint(0).y + scale*25.f);//set information position according to the canvas

    //selections
    sf::Text infoLabel("Info", myfont, 32);
    infoLabel.setScale(scale,scale);
    infoLabel.setPosition(-scale*100.f, window.getSize().y/4.f);
    infoLabel.setColor(sf::Color(0,100,100,255));

    sf::Text webLabel("Web", myfont, 32);
    webLabel.setScale(scale,scale);
    webLabel.setPosition(-scale*100.f, infoLabel.getPosition().y + infoLabel.getGlobalBounds().height*2.f);
    webLabel.setColor(sf::Color(0,100,0,255));

    sf::Text creditLabel("Credits", myfont, 32);
    creditLabel.setScale(scale,scale);
    creditLabel.setPosition(-scale*100.f, webLabel.getPosition().y + infoLabel.getGlobalBounds().height*2.f);
    creditLabel.setColor(sf::Color(100,0,100,255));

    sf::Text resourcesLabel("Assets", myfont, 32);
    resourcesLabel.setScale(scale,scale);
    resourcesLabel.setPosition(-scale*100.f, creditLabel.getPosition().y + infoLabel.getGlobalBounds().height*2.f);
    resourcesLabel.setColor(sf::Color(0,0,255,255));

    sf::Texture spinnerTexture;
    if(!spinnerTexture.loadFromFile("img/spinner.png"))
    {
        std::cerr<<"Could not load spinner.png"<<std::endl;
    }

    sf::Sprite spinner;
    spinner.setTexture(spinnerTexture);
    spinner.setColor(sf::Color(255,255,255,200));
    spinner.setOrigin(spinner.getGlobalBounds().width/2.f, spinner.getGlobalBounds().height/2.f);
    spinner.setScale(scale,scale);
    spinner.setPosition(infoLabel.getPosition().x, infoLabel.getPosition().y+infoLabel.getGlobalBounds().height);

    //sound
    sf::SoundBuffer selectBuff;
    if(!selectBuff.loadFromFile("sound/mousetab.wav"))
    {
        std::cerr<<"Could not load mousetab.wav"<<std::endl;
    }

    sf::Sound selectSound;
    selectSound.setBuffer(selectBuff);
    selectSound.setVolume(m_initialEff);

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time flyInFrames = sf::Time::Zero;
    sf::Time spinnerFrames = sf::Time::Zero;
    sf::Time scaleFrames = sf::Time::Zero;

    //housekeeping for selections
    std::size_t selection = 0;

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

                    case sf::Keyboard::Up:
                    {
                        if(selection < 0)
                            selection = 0;
                        else
                        {
                            selectSound.play();
                            --selection;
                        }
                    }
                    break;

                    case sf::Keyboard::Down:
                    {
                        if(selection > 3)
                            selection = 3;
                        else
                        {
                            selectSound.play();
                            ++selection;
                        }
                    }
                    break;

                default:
                    break;
                }
            }
        }

        //update the selection state
        switch(selection)
        {
            case 0:
            {
                infoLabel.setColor(sf::Color(255,255,0,255));
                webLabel.setColor(sf::Color(0,100,0,255));
                creditLabel.setColor(sf::Color(100,0,100,255));
                resourcesLabel.setColor(sf::Color(0,0,255,255));
                information.setString("Author: Mark Richards\n\nN.A.S.I.C. is a game heavily influenced by\n\nSpace Invaders with elements of other familiar\n\nshoot 'em ups.");
                spinner.setPosition(infoLabel.getPosition().x, infoLabel.getPosition().y+infoLabel.getGlobalBounds().height);
            }
            break;

            case 1:
            {
                infoLabel.setColor(sf::Color(0,100,100,255));
                webLabel.setColor(sf::Color(255,255,0,255));
                creditLabel.setColor(sf::Color(100,0,100,255));
                resourcesLabel.setColor(sf::Color(0,0,255,255));
                information.setString("Check N.A.S.I.C tutorials out on the web at:\n\nwww.pushbuttonreceivecode.com.\n\nCheck out my other work at: \n\nwww.markrichards.ws.");
                spinner.setPosition(webLabel.getPosition().x, webLabel.getPosition().y+webLabel.getGlobalBounds().height);
            }
            break;

            case 2:
            {
                infoLabel.setColor(sf::Color(0,100,100,255));
                webLabel.setColor(sf::Color(0,100,0,255));
                creditLabel.setColor(sf::Color(255,255,0,255));
                resourcesLabel.setColor(sf::Color(0,0,255,255));
                information.setString("Credits go to the following awesome people \nand tools that made this game tutorial possible:\n\nPowered by SFML (Laurent Gomila et. al.)\nhttp://www.sfml-dev.org\n\nTHOR Extensions for SFML(Jan Haller )\nhttp://www.bromeon.ch/libraries/thor/");
                spinner.setPosition(creditLabel.getPosition().x, creditLabel.getPosition().y+creditLabel.getGlobalBounds().height);
            }
            break;

            case 3:
            {
                infoLabel.setColor(sf::Color(0,100,100,255));
                webLabel.setColor(sf::Color(0,100,0,255));
                creditLabel.setColor(sf::Color(100,0,100,255));
                resourcesLabel.setColor(sf::Color(255,100,0,255));
                information.setString(
                                      "Iron Plague graphics courtesy of Daniel Cook\n"
                                      "http://www.lostgarden.com\n\n"
                                      "GNU Image Manipulation Program (GIMP)\n"
                                      "http://www.gimp.org\n\n"
                                      "Sound FX courtesy of BFXR\n"
                                      "http://www.bfxr.net/\n\n"
                                      "Music composed with Musagi\n"
                                      "http://drpetter.se/project_musagi.html\n\n");
                spinner.setPosition(resourcesLabel.getPosition().x, resourcesLabel.getPosition().y+resourcesLabel.getGlobalBounds().height);
            }
            break;

            default:
                break;
        }

        //handle game ticks and return a fixed dt
        //for updates
        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            flyInFrames += TimePerFrame;

            if(spinnerFrames.asSeconds() > 4.f)
                spinnerFrames = sf::Time::Zero;
            else
                spinnerFrames += TimePerFrame;

            if(scaleFrames.asSeconds() > 2.f)
                scaleFrames = sf::Time::Zero;
            else
                scaleFrames += TimePerFrame;

            hex.update(window, TimePerFrame);

            //text fly in
            if(flyInFrames.asSeconds() < 1.f)
            {
                infoLabel.move(scale*interpolate::quarticEaseOut(flyInFrames.asSeconds(),0.f,10.f,1.f), 0.f);
                webLabel.move(scale*interpolate::quarticEaseOut(flyInFrames.asSeconds(),0.f,10.f,1.f), 0.f);
                creditLabel.move(scale*interpolate::quarticEaseOut(flyInFrames.asSeconds(),0.f,10.f,1.f), 0.f);
                resourcesLabel.move(scale*interpolate::quarticEaseOut(flyInFrames.asSeconds(),0.f,10.f,1.f), 0.f);
            }

            //spinner animation
            if(spinnerFrames.asSeconds() < 4.f)
            {
                spinner.rotate(scale*10.f*interpolate::cubicEaseOut(spinnerFrames.asSeconds(),0.f,1.f,4.f));
            }

            if(scaleFrames.asSeconds() < 2.f)
            {
                if(scaleFrames.asSeconds() < 2.f)
                    spinner.setScale(fabs(interpolate::elasticEaseOut(scaleFrames.asSeconds(),0.f,scale*1.45f,2.f)),fabs(interpolate::elasticEaseOut(scaleFrames.asSeconds(),0.f,scale*1.45f,2.f)));
            }

            if(m_bgOne.getPosition().y >= h && m_bgOne.getPosition().x <= 0.f)
            {
                m_bgOne.setPosition(w,-h);
            }
            else
                m_bgOne.move(-30.f*scale*TimePerFrame.asSeconds(), 30.f*scale*TimePerFrame.asSeconds());

            if(m_bgTwo.getPosition().y >= h && m_bgTwo.getPosition().x <= 0.f)
            {
                m_bgTwo.setPosition(w,-h);
            }
            else
                m_bgTwo.move(-30.f*scale*TimePerFrame.asSeconds(), 30.f*scale*TimePerFrame.asSeconds());

            if(m_bgThree.getPosition().y >= 0)
            {
                m_bgThree.setPosition(0.f,-h);
            }
            else
                m_bgThree.move(-30.f*scale*TimePerFrame.asSeconds(), 30.f*scale*TimePerFrame.asSeconds());

            if(m_bgFour.getPosition().y >= h)
            {
                m_bgFour.setPosition(w,0.f);
            }
            else
                m_bgFour.move(-30.f*scale*TimePerFrame.asSeconds(), 30.f*scale*TimePerFrame.asSeconds());
        }

        window.clear(sf::Color(55,55,55,255));
        window.draw(m_bgOne);
        window.draw(m_bgTwo);
        window.draw(m_bgThree);
        window.draw(m_bgFour);
        window.draw(hex);
        window.draw(panel);
        window.draw(corner);
        window.draw(canvas);
        window.draw(spinner);
        window.draw(infoLabel);
        window.draw(webLabel);
        window.draw(creditLabel);
        window.draw(resourcesLabel);
        window.draw(information);
        window.display();
    }
    return;
}

//instantiate static members
sf::Uint32 nasic::info::m_infostate = nasic::info::infostate::uninitialized;
