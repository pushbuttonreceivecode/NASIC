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

#include <level.hpp>

nasic::level::level()
{
    using nasic::level;
    m_levelstate = levelstate::uninitialized;

    if(!m_options.loadOptions(m_options, "data/options.xml"))
    {
        std::cerr<<"Could not load options.xml"<<std::endl;
        m_musicVolume = 3;
        m_effectsVolume = 5;
        m_difficulty = 1;
    }

    m_musicVolume = m_options.m_volume;
    m_effectsVolume = m_options.m_effects;
    m_difficulty = m_options.m_difficulty;
}

nasic::level::~level()
{
    delete m_enemy;//delete enemy pointer
}

void nasic::level::show(sf::RenderWindow& window)
{
    using nasic::level;

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

    sf::ContextSettings settings;

    settings = window.getSettings();
    std::cout<<settings.majorVersion<<std::endl;
    std::cout<<settings.minorVersion<<std::endl;
    std::cout<<settings.antialiasingLevel<<std::endl;
    std::cout<<settings.stencilBits<<"\n"<<settings.depthBits<<std::endl;

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps

    std::cout<<"Video mode is: "<<mode.width<<"\n"<<"Scale factor is: "<<scale<<std::endl;

    //bail if the options are not in uninitialized state
    if(!m_levelstate == levelstate::uninitialized)
        return;

    m_levelstate = levelstate::playing;

    //create level stuff

    //sound
    sf::SoundBuffer shotBuff;
    if(!shotBuff.loadFromFile("sound/shot.wav"))
    {
        std::cerr<<"Could not load shot.wav"<<std::endl;
    }
    sf::Sound shot;
    shot.setBuffer(shotBuff);
    shot.setVolume(m_effectsVolume);

    //set up the sound buffers for the explosion sound
    if(!m_explodBuff.loadFromFile("sound/explosion.wav"))
    {
        std::cerr<<"Could not load explosion.wav"<<std::endl;
    }


    if(!m_hitBuff.loadFromFile("sound/hit.wav"))
    {
        std::cerr<<"Could not load hit.wav"<<std::endl;
    }

    m_explosion.setVolume(m_effectsVolume);

    //text
    sf::Font myfont;
    if(!myfont.loadFromFile("fonts/PressStart2P.ttf"))
    {
        std::cerr<<"Could not load PressStart2P.ttf"<<std::endl;
    }

    sf::Text score("Score ", myfont, 16);
    score.setOrigin(score.getGlobalBounds().width/2.f, score.getGlobalBounds().height/2.f);
    score.setScale(scale, scale);
    score.setPosition(score.getGlobalBounds().width, m_winsizeY/12.f);

    sf::Text lives("Lives ", myfont, 16);
    lives.setOrigin(lives.getGlobalBounds().width/2.f, lives.getGlobalBounds().height/2.f);
    lives.setScale(scale, scale);
    lives.setPosition(m_winsizeX/2.f, m_winsizeY/12.f);

    starfield stars(window, starfield::starStyle::starsAndPlanets, scale);

    //set up enemy stuff
    initEnemies(window, 4, 10, scale);
    bool dir = 0;

    //set up player stuff
    player hero(nasic::player::shipType::warrior, scale);
    hero.setPosition(m_winsizeX/2.f, m_winsizeY/1.25f);
    bool moveleft = false;
    bool moveright = false;
    bool moveup = false;
    bool movedown = false;
    bool shooting = false;
    sf::Clock ammoClock;

    //effects stuff...
    sf::Texture expTexture;
    if(!expTexture.loadFromFile("img/shipexplode.png"))
    {
        std::cerr<<"Could not load shipexplode.png"<<std::endl;
    }

    m_explosionSpr.setTexture(expTexture);
    m_explosionSpr.setScale(scale,scale);
    nasic::addFrames(m_expFrames, 24, 25, 0, 0, 8);
    m_expAnim.addAnimation("explode", m_expFrames, sf::seconds(1.f));

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
                m_levelstate = levelstate::exit;
                return;
            }

            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::Escape:
                {
                    m_levelstate = levelstate::exit;
                    return;
                }
                break;

                case sf::Keyboard::Left:
                {
                    moveleft = true;
                    moveright = false;
                }
                break;

                case sf::Keyboard::Right:
                {
                    moveright = true;
                    moveleft = false;
                }
                break;

                case sf::Keyboard::Up:
                {
                    moveup = true;
                    movedown = false;
                }
                break;

                case sf::Keyboard::Down:
                {
                    movedown = true;
                    moveup = false;
                }
                break;

                case sf::Keyboard::Space:
                {
                    if(ammoClock.getElapsedTime().asSeconds() > .01f)
                    {
                        ammoClock.restart();
                        shooting = true;
                    }
                }
                break;

                default:
                    break;
                }
            }

            if(e.type == sf::Event::KeyReleased)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::Left:
                {
                    moveleft = false;
                }
                break;

                case sf::Keyboard::Right:
                {
                    moveright = false;
                }
                break;

                case sf::Keyboard::Up:
                {
                    moveup = false;
                }
                break;

                case sf::Keyboard::Down:
                {
                    movedown = false;
                }
                break;

                case sf::Keyboard::Space:
                {
                    shooting = false;
                }
                break;

                default:
                    break;
                }
            }
        }

        if(shooting && m_playerAmmo.size() < 2 && ammoClock.getElapsedTime().asSeconds() < .01f)
        {
            nasic::ammo m_ammo(nasic::ammo::ammoType::warrior, sf::Vector2f(hero.getPosition().x, hero.getPosition().y), scale);
            m_playerAmmo.push_back(m_ammo);
            std::cout<<m_playerAmmo.size()<<std::endl;
            shot.play();
        }

        //handle game ticks and return a fixed dt
        //for updates
        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            stars.update(window,TimePerFrame);

            updateEnemies(window, TimePerFrame);
            m_expAnim.update(TimePerFrame);

            //movement for enemies
            std::list<nasic::enemy>::iterator enemIt;
            for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
            {
                if((*enemIt).getPosition().x <= (*enemIt).getAABB().x)
                {
                    dir = 0;
                }

                if((*enemIt).getPosition().x >= m_winsizeX - (*enemIt).getAABB().x)
                {
                    dir = 1;
                }

                switch(dir)
                {
                case 0://move enemies right
                {
                    (*enemIt).move(scale*(TimePerFrame.asSeconds()*75), 0.f);
                }
                break;

                case 1://move enemies left
                {
                    (*enemIt).move(scale*(-TimePerFrame.asSeconds()*75), 0.f);
                }
                break;

                default:
                    break;
                }
            }

            updateProjectiles(TimePerFrame);
            checkEnemyCollisions(window);
            hero.update(window, TimePerFrame);

            if(moveleft && hero.getPosition().x >= 0.f + hero.getAABB().x)
                hero.move(sf::Vector2f(-(float)hero.getVel()*scale, 0.f),TimePerFrame);

            if(moveright && hero.getPosition().x <= m_winsizeX - hero.getAABB().x)
                hero.move(sf::Vector2f((float)hero.getVel()*scale, 0.f),TimePerFrame);

            if(moveup && hero.getPosition().y >= m_winsizeY/2.f + hero.getAABB().y)
                hero.move(sf::Vector2f(0.f, -(float)hero.getVel()*scale),TimePerFrame);

            if(movedown && hero.getPosition().y <= m_winsizeY - hero.getAABB().y)
                hero.move(sf::Vector2f(0.f, (float)hero.getVel()*scale),TimePerFrame);
        }

        m_expAnim.animate(m_explosionSpr);

        window.clear();
        window.draw(stars);
        renderEnemies(window);
        renderProjectiles(window);
        window.draw(hero);
        if(m_expAnim.isPlayingAnimation())
            window.draw(m_explosionSpr);
        window.draw(score);
        window.draw(lives);
        window.display();
    }

    return;
}

void nasic::level::initEnemies(sf::RenderWindow& window, int rows, int columns, float scale)
{
    float offset;
    for(int i=0; i<rows; ++i)
    {
        for(int j=0; j<columns; ++j)
        {
            switch(i)
            {
            case 0:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Rhiians, scale);
                offset = scale*50.f;
                m_enemy->setPosition(j * offset + m_winsizeX/2.f, i * offset + offset/2.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            case 1:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Agravu, scale);
                offset = scale*50.f;
                m_enemy->setPosition(j * offset + m_winsizeX/1.95f, i * offset + offset/2.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            case 2:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Delsiriak, scale);
                offset = scale*50.f;
                m_enemy->setPosition(j * offset + m_winsizeX/1.95f, i * offset + offset/2.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            case 3:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Gluorn, scale);
                offset = scale*50.f;
                m_enemy->setPosition(j * offset + m_winsizeX/2.f, i * offset + offset/2.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            default:
                break;
            }
        }
    }
}

void nasic::level::updateEnemies(sf::RenderWindow& window, sf::Time dt)
{
    std::list<nasic::enemy>::iterator enemIt;
    for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
    {
        //perform animation/state updates
        (*enemIt).update(dt);
    }
}

void nasic::level::renderEnemies(sf::RenderWindow& window)
{
    std::list<nasic::enemy>::iterator enemIt;
    for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
    {
        window.draw(*enemIt);
    }
}

void nasic::level::updateProjectiles(sf::Time dt)
{
    for(m_pAmmoIt = m_playerAmmo.begin(); m_pAmmoIt != m_playerAmmo.end(); ++m_pAmmoIt)
    {
        (*m_pAmmoIt).fire(dt);
    }
}

void nasic::level::checkEnemyCollisions(sf::RenderWindow& window)
{
    std::list<nasic::ammo>::iterator ammoIt;
    for(ammoIt = m_playerAmmo.begin(); ammoIt != m_playerAmmo.end(); ++ammoIt)
    {
        if((*ammoIt).getPosition().y < 0.f)
        {
            ammoIt = m_playerAmmo.erase(ammoIt);
            std::cout<<m_playerAmmo.size()<<std::endl;
        }
    }

    std::list<nasic::enemy>::iterator enemIt;
    for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
    {
        if(enemIt->getState() == nasic::enemy::enemyState::dead)
        {
            //set the explosion position to the enemy location
            m_explosionSpr.setPosition(enemIt->getPosition().x, enemIt->getPosition().y);
            m_expAnim.playAnimation("explode");

            //set the explosion sound buffer and play it
            m_explosion.setBuffer(m_explodBuff);
            m_explosion.play();

            //erase the enemy from the list
            enemIt = m_enemies.erase(enemIt);
        }

        std::list<nasic::ammo>::iterator pAmmoIt;
        for(pAmmoIt = m_playerAmmo.begin(); pAmmoIt != m_playerAmmo.end(); ++pAmmoIt)
        {
            if(pAmmoIt->getPosition().y >= enemIt->getPosition().y
                    && pAmmoIt->getPosition().y <= enemIt->getPosition().y + enemIt->getAABB().y
                    && pAmmoIt->getPosition().x >= enemIt->getPosition().x
                    && pAmmoIt->getPosition().x <= enemIt->getPosition().x + enemIt->getAABB().x*2.f)
            {
                //erase the bullet from the list
                pAmmoIt = m_playerAmmo.erase(pAmmoIt);

                enemIt->damage(1);

                //set the explosion sound buffer and play it
                m_explosion.setBuffer(m_hitBuff);
                m_explosion.play();
            }
        }
    }
}

void nasic::level::renderProjectiles(sf::RenderWindow& window)
{
    for(m_pAmmoIt = m_playerAmmo.begin(); m_pAmmoIt != m_playerAmmo.end(); ++m_pAmmoIt)
    {
        window.draw(*m_pAmmoIt);
    }
}

void nasic::addFrames(thor::FrameAnimation& animation, int w, int h, int y, int xFirst, int xLast, float duration)
{
    const int step = (xFirst < xLast) ? +1 : -1;
    xLast += step; // so xLast is excluded in the range

    for (int x = xFirst; x != xLast; x += step)
        animation.addFrame(duration, sf::IntRect(w*x, h*y, w, h));
}

//instantiate static members
sf::Uint32 nasic::level::m_levelstate = nasic::level::levelstate::uninitialized;
