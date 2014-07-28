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

nasic::level::level(sf::RenderWindow& window)
{
    using nasic::level;
    m_levelstate = levelstate::uninitialized;

    if(!m_options.loadOptions(m_options, "data/options.xml"))
    {
        std::cerr<<"Could not load options.xml"<<std::endl;
        m_musicVolume = 15;
        m_effectsVolume = 15;
        m_difficulty = 1;
    }

    m_musicVolume = m_options.m_volume;
    m_effectsVolume = m_options.m_effects;
    m_difficulty = m_options.m_difficulty;

    //text
    if(!m_myfont.loadFromFile("fonts/contb.ttf"))
    {
        std::cerr<<"Could not load contb.ttf"<<std::endl;
    }

    if(!m_hudFont.loadFromFile("fonts/PressStart2P.ttf"))
    {
        std::cerr<<"Could not load PressStart2P.ttf"<<std::endl;
    }

    //initialize number of lives
    switch(m_difficulty)
    {
    case 0:
    {
        m_numLives = 4;
    }
    break;
    case 1:
    {
        m_numLives = 3;
    }
    break;
    case 2:
    {
        m_numLives = 2;
    }
    break;
    case 3:
    {
        m_numLives = 1;
    }
    break;
    default:
        m_numLives = 3;
        break;
    }

    //initialize score and bonus data
    m_score = 0;
    m_hits = 0;
    m_misses = 0;
    m_timeBonusFactor = 0;
    m_survivalBonus = 0;
    m_accuracyBonus = 0;
    m_timeBonusMessage.setFont(m_hudFont);
    m_timeBonusMessage.setCharacterSize(18);
    m_survivalBonusMessage.setFont(m_hudFont);
    m_survivalBonusMessage.setCharacterSize(18);
    m_accuracyBonusMessage.setFont(m_hudFont);
    m_accuracyBonusMessage.setCharacterSize(18);
    m_timeBonusMessage.setPosition(window.getSize().x*.9f, window.getSize().y/4.f);
    m_survivalBonusMessage.setPosition(window.getSize().x*.9f, window.getSize().y/6.f);
    m_accuracyBonusMessage.setPosition(window.getSize().x*.9f, window.getSize().y/12.f);

    //initialize enemy wave status
    m_enemyWave.setInitStatus(false);
}

nasic::level::~level()
{

}

void nasic::level::show(sf::RenderWindow& window)
{
    using nasic::level;

    /////////////////////////////////////////////////////
    //***************IMPORTANT NOTE*********************
    //we're getting the video mode and creating a scale
    //variable for the drawables using 800 as the base
    //800*600 will be the smallest screen supported
    //*****KEEPING IN MIND*****
    //we're blowing up small images...
    //this is not ideal for non-retro-looking sprites
    /////////////////////////////////////////////////////
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    m_scaleX = mode.width/800.f;
    m_scaleY = mode.height/600.f;

    m_winsizeX = mode.width;
    m_winsizeY = mode.height;

    sf::ContextSettings settings;

    settings = window.getSettings();
    //std::cout<<settings.majorVersion<<std::endl;
    //std::cout<<settings.minorVersion<<std::endl;
    //std::cout<<settings.antialiasingLevel<<std::endl;
    //std::cout<<settings.stencilBits<<"\n"<<settings.depthBits<<std::endl;

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps

    //std::cout<<"Video mode is: "<<mode.width<<"\n"<<"Scale factor is: "<<m_scaleX<<std::endl;

    //bail if the level is not in uninitialized state
    if(!m_levelstate == levelstate::uninitialized)
        return;

    m_levelstate = levelstate::playing;

    ///////////////////////////////
    //***************************
    //create level stuff
    //***************************
    ///////////////////////////////

    //initialize sounds and music
    if(!m_bgSndBuffer.loadFromFile("sound/beepboop.wav"))
    {
        std::cerr<<"Could not load beepboop.wav"<<std::endl;
    }

    m_bgSnd.setBuffer(m_bgSndBuffer);
    m_bgSnd.setVolume(m_effectsVolume);
    m_bgSnd.setLoop(true);
    m_bgSnd.play();

    if(!m_music.openFromFile("sound/theme.ogg"))
    {
        std::cerr<<"Could not load theme.ogg"<<std::endl;
    }

    m_music.setVolume(m_musicVolume);
    m_music.setLoop(true);

    if(!m_shotBuff.loadFromFile("sound/shot.wav"))
    {
        std::cerr<<"Could not load shot.wav"<<std::endl;
    }

    m_shot.setBuffer(m_shotBuff);
    m_shot.setVolume(m_effectsVolume);

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

    //set up player stuff
    player hero(nasic::player::shipType::warrior, m_scaleX);
    hero.setPosition(m_winsizeX/2.f, m_winsizeY/1.25f);
    sf::Uint32 initialLives =  m_numLives;
    sf::Uint32 accuracyRatio = 0;
    bool moveleft = false;
    bool moveright = false;
    bool moveup = false;
    bool movedown = false;
    bool shooting = false;
    sf::Clock ammoClock;

    sf::Texture lifeTexture;
    switch(hero.getType())
    {
    case nasic::player::shipType::warrior:
    {
        if(!lifeTexture.loadFromFile("img/warrior.png"))
        {
            std::cerr<<"Could not load warrior.png"<<std::endl;
        }
    }
    break;

    case nasic::player::shipType::hunter:
    {
        if(!lifeTexture.loadFromFile("img/hunter.png"))
        {
            std::cerr<<"Could not load hunter.png"<<std::endl;
        }
    }
    break;

    case nasic::player::shipType::guard:
    {
        if(!lifeTexture.loadFromFile("img/guard.png"))
        {
            std::cerr<<"Could not load guard.png"<<std::endl;
        }
    }
    break;

    default:
        break;
    }

    //set up enemy stuff
    m_enemyWave.init(window, 4, 10, m_scaleX);
    sf::Uint32 collision = 0;
    m_wave = 1;
    bool doBossFight = false;

    //set up for killer
    sf::Texture blood;
    if(!blood.loadFromFile("img/blood.png"))
    {
        std::cerr<<"Could not load blood.png"<<std::endl;
    }

    m_killer.init(window, m_scaleX);

    //some stuff to uniformly distribute the particles
    std::mt19937 engine;
    std::uniform_int_distribution<int> distr(m_scaleX*10, (int)m_killer.getAABB().x/4);
    auto randomizer = std::bind(distr, engine);
    thor::Distribution<int> thorDistr(randomizer);

    sf::SoundBuffer killerBuff;
    if(!killerBuff.loadFromFile("sound/bossDrone.wav"))
    {
        std::cerr<<"Could not load bossDrone.wav"<<std::endl;
    }
    sf::Sound killerDrone;
    killerDrone.setBuffer(killerBuff);
    killerDrone.setVolume(m_effectsVolume);
    killerDrone.setLoop(true);

    sf::Text bossHealthLabel("BOSS:", m_hudFont, 16);
    bossHealthLabel.setScale(m_scaleX, m_scaleX);
    bossHealthLabel.setColor(sf::Color(255,255,255,200));
    bossHealthLabel.setPosition(m_scaleX*10.f, m_winsizeY*.91f);
    sf::Uint32 bossMaxHealth = m_killer.getHealth();

    sf::RectangleShape bossHealth;
    bossHealth.setFillColor(sf::Color(200,0,0,100));
    bossHealth.setScale(m_scaleX,m_scaleX);
    bossHealth.setSize(sf::Vector2f(m_scaleX*m_killer.getHealth(),m_scaleX*5.f));
    bossHealth.setPosition(bossHealthLabel.getPosition().x + bossHealthLabel.getGlobalBounds().width, bossHealthLabel.getPosition().y);

    sf::Text score("Score: ", m_hudFont, 16);
    score.setScale(m_scaleX, m_scaleX);
    score.setColor(sf::Color(255,255,255,200));
    score.setPosition(m_winsizeX*.65f, m_scaleX*10.f);
    m_scoreLabel.setFont(m_hudFont);
    m_scoreLabel.setString(toString(m_score));
    m_scoreLabel.setColor(sf::Color(0,200,0,200));
    m_scoreLabel.setCharacterSize(16);
    m_scoreLabel.setScale(m_scaleX,m_scaleX);
    m_scoreLabel.setPosition(score.getPosition().x + score.getGlobalBounds().width, score.getPosition().y);

    sf::Text lives("Lives: ", m_hudFont, 16);
    lives.setScale(m_scaleX, m_scaleX);
    lives.setColor(sf::Color(255,255,255,200));
    lives.setPosition(m_scaleX*10.f, m_winsizeY*.95f);

    sf::Text lifeLabel("Life:", m_hudFont, 16);
    lifeLabel.setScale(m_scaleX, m_scaleX);
    lifeLabel.setColor(sf::Color(255,255,255,200));
    lifeLabel.setPosition(m_scaleX*10.f,m_scaleX*10.f);
    sf::Uint32 maxHealth = hero.getHealth();

    sf::RectangleShape life;
    life.setFillColor(sf::Color(0,255,0,100));
    life.setScale(m_scaleX,m_scaleX);
    life.setSize(sf::Vector2f(m_scaleX*hero.getHealth(),m_scaleX*5.f));
    life.setPosition(lifeLabel.getPosition().x + lifeLabel.getGlobalBounds().width, lifeLabel.getPosition().y);

    m_life.setTexture(lifeTexture);
    m_life.setScale(m_scaleX/2.f,m_scaleX/2.f);
    m_life.setOrigin(0.f,m_life.getGlobalBounds().height/5.f);
    for(int i=1; i<m_numLives; i++)
    {
        if(i==1)
        {
            m_life.setPosition(m_life.getGlobalBounds().width + m_scaleX*lives.getGlobalBounds().width/2.f, lives.getPosition().y);
        }
        else
        {
            m_life.setPosition(i*m_life.getGlobalBounds().width + m_scaleX*lives.getGlobalBounds().width/2.f, lives.getPosition().y);
        }

        m_lives.push_back(m_life);
    }

    m_timeLabel.setFont(m_hudFont);
    m_timeLabel.setString("Time: ");
    m_timeLabel.setCharacterSize(16);
    m_timeLabel.setScale(m_scaleX,m_scaleX);
    m_timeLabel.setPosition(m_winsizeX*.65f, m_winsizeY*.95f);

    sf::RectangleShape bottomLine;
    bottomLine.setSize(sf::Vector2f(m_winsizeX, 1.f));
    bottomLine.setFillColor(sf::Color(200,0,0,200));
    bottomLine.setPosition(0,m_winsizeY*.9f);

    sf::RectangleShape bottomBG;
    bottomBG.setSize(sf::Vector2f(m_winsizeX, m_winsizeY - (m_winsizeY*.9f)));
    bottomBG.setFillColor(sf::Color(100,100,100,200));
    bottomBG.setPosition(0,m_winsizeY*.9f);

    starfield stars(window, starfield::starStyle::starsAndPlanets, m_scaleX);

    //effects stuff...
    sf::Texture expTexture;
    if(!expTexture.loadFromFile("img/shipexplode.png"))
    {
        std::cerr<<"Could not load shipexplode.png"<<std::endl;
    }

    m_explosionSpr.setTexture(expTexture);
    m_explosionSpr.setScale(m_scaleX,m_scaleX);
    nasic::addFrames(m_expFrames, 24, 25, 0, 0, 8);
    m_expAnim.addAnimation("explode", m_expFrames, sf::seconds(1.f));
    bool explode = false;

    //initialize the scale of intro and boss messages
    m_introMessage.setScale(0.f,0.f);
    m_bossMessage.setScale(0.f,0.f);

    //initialize pause stuff...
    sf::RectangleShape pauseBg;
    pauseBg.setSize(sf::Vector2f(m_scaleX*400.f, m_scaleY*200.f));
    pauseBg.setFillColor(sf::Color(0,255,0,200));
    pauseBg.setOrigin(pauseBg.getGlobalBounds().width/2.f, pauseBg.getGlobalBounds().height/2.f);
    pauseBg.setPosition(window.getSize().x/2.f, window.getSize().y/2.f);

    thor::ConcaveShape rightCorner;
    rightCorner.setPointCount(4);
    rightCorner.setPoint(0, sf::Vector2f(pauseBg.getGlobalBounds().width, pauseBg.getGlobalBounds().height));
    rightCorner.setPoint(1, sf::Vector2f(pauseBg.getGlobalBounds().width*.75f, pauseBg.getGlobalBounds().height));
    rightCorner.setPoint(2, sf::Vector2f(pauseBg.getGlobalBounds().width*.9f, pauseBg.getGlobalBounds().height*.95f));
    rightCorner.setPoint(3, sf::Vector2f(pauseBg.getGlobalBounds().width, pauseBg.getGlobalBounds().height*.8f));
    rightCorner.setPosition(pauseBg.getPosition().x - pauseBg.getGlobalBounds().width/2.1f, pauseBg.getPosition().y - pauseBg.getGlobalBounds().height/2.1f);
    rightCorner.setFillColor(sf::Color(0,255,255,255));

    thor::ConcaveShape leftCorner;
    leftCorner.setPointCount(7);
    leftCorner.setPoint(0, sf::Vector2f(0.f, 0.f));
    leftCorner.setPoint(1, sf::Vector2f(pauseBg.getGlobalBounds().width/2.f, 0.f));
    leftCorner.setPoint(2, sf::Vector2f(pauseBg.getGlobalBounds().width/2.5f, pauseBg.getGlobalBounds().height/8.f));
    leftCorner.setPoint(3, sf::Vector2f(pauseBg.getGlobalBounds().width/3.f, pauseBg.getGlobalBounds().height/10.f));
    leftCorner.setPoint(4, sf::Vector2f(pauseBg.getGlobalBounds().width/8.f, pauseBg.getGlobalBounds().height/6.f));
    leftCorner.setPoint(5, sf::Vector2f(pauseBg.getGlobalBounds().width/16.f, pauseBg.getGlobalBounds().height));
    leftCorner.setPoint(6, sf::Vector2f(0.f, pauseBg.getGlobalBounds().height));
    leftCorner.setPosition(pauseBg.getPosition().x - pauseBg.getGlobalBounds().width/1.9f, pauseBg.getPosition().y - pauseBg.getGlobalBounds().height/1.9f);
    leftCorner.setFillColor(sf::Color(255,0,255,255));

    m_pauseOverlay = sf::Text("-PAUSED-", m_myfont, 128);
    m_pauseOverlay.setOrigin(m_pauseOverlay.getGlobalBounds().width/2.f, m_pauseOverlay.getGlobalBounds().height/2.f);
    m_pauseOverlay.setColor(sf::Color(200,0,0,200));
    m_pauseOverlay.setPosition(pauseBg.getPosition().x, pauseBg.getPosition().y);

    int pauseSwitch = -1;
    int pauseCounter = 0;
    if(!m_pauseBuffer.loadFromFile("sound/pause.wav"))
    {
        std::cerr<<"Could not load pause.wav"<<std::endl;
    }
    m_pauseSnd.setBuffer(m_pauseBuffer);
    m_pauseSnd.setVolume(m_effectsVolume);
    m_pauseSnd.setLoop(false);

    //time stuff...
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time pauseFrames = sf::Time::Zero;
    sf::Time introFrames = sf::Time::Zero;
    sf::Time bossIntroFrames = sf::Time::Zero;

    int tempcounter = 0;
    sf::Time bonusFrames = sf::Time::Zero;
    sf::Time messageFrames = sf::Time::Zero;
    int colorSwitch = 1;
    sf::Time timeClock;

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

            if(e.type == sf::Event::KeyPressed && (introFrames.asSeconds() > 10.f && bossIntroFrames.asSeconds() > 10.f))
            {
                switch(e.key.code)
                {
                case sf::Keyboard::Escape:
                {
                    m_levelstate = levelstate::exit;
                    return;
                }
                break;

                case sf::Keyboard::Delete://super secret enemy clearing mechanism ;)
                {
                    m_enemyWave.debugClearEnemies();
                }
                break;

                case sf::Keyboard::B:
                {
                    m_wave = 4;
                }
                break;

                case sf::Keyboard::K://super secret player killswitch
                {
                    if(m_numLives > 0)
                    {
                        m_numLives -=1;

                        if(m_lives.size() > 0)
                            m_lives.pop_back();

                        std::cout<<"Number of lives: "<<m_numLives<<std::endl;
                    }
                    else
                    {
                        std::cout<<"Number of lives: "<<m_numLives<<std::endl;
                        m_levelstate = levelstate::lost;
                        return;
                    }
                }
                break;

                case sf::Keyboard::F://super secret win button...
                {
                    m_levelstate = levelstate::won;
                    return;
                }
                break;

                case sf::Keyboard::L://super secret lose button...
                {
                    m_levelstate = levelstate::lost;
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

                case sf::Keyboard::P:
                {
                    pauseSwitch *= -1;//toggle the pause overlay
                    m_pauseSnd.play();
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

            if(e.type == sf::Event::KeyReleased && (introFrames.asSeconds() > 10.f && bossIntroFrames.asSeconds() > 10.f))
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

        //////////////////////////////////////////
        //handle game ticks and return
        //a fixed dt for updates
        //thanks to the SFML book for
        //providing this code
        //////////////////////////////////////////

        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            ///////////////////////////////////
            //frame updates
            ///////////////////////////////////
            if(introFrames.asSeconds() < 10.f)
                timeClock = sf::Time::Zero;//reset the timer until the wave starts...

            timeSinceLastUpdate -= TimePerFrame;
            introFrames += TimePerFrame;
            bossIntroFrames += TimePerFrame;
            bonusFrames += TimePerFrame;

            if(pauseSwitch != 1)
                timeClock += TimePerFrame;

            srand(time(NULL));//seed a random number every frame

            stars.update(window,TimePerFrame);//update the star field

            m_expAnim.update(TimePerFrame);//update the explosion animation
            m_expAnim.animate(m_explosionSpr);

            /////////////////////////////////
            //pause screen overlay updates
            /////////////////////////////////

            if(pauseSwitch)
            {
                //updates and time for pause message
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
                    m_pauseOverlay.setColor(sf::Color(255,(unsigned int)g,(unsigned int)b,255));
                    pauseBg.setFillColor(sf::Color(0,(unsigned int)g,255-(unsigned int)b,200));
                }

                if(colorSwitch == 1 && r < 255)
                {
                    m_pauseOverlay.setColor(sf::Color(255,255-(unsigned int)g,255-(unsigned int)b,255));
                    pauseBg.setFillColor(sf::Color(0,255-(unsigned int)g,(unsigned int)b,200));
                }
            }

            //show the intro text boxes at the beginning of each wave
            if(m_wave < 4)
                showIntro(window, introFrames);

            if(introFrames.asSeconds() < 2.f)
                m_introMessage.setScale(fabs(interpolate::elasticEaseOut(introFrames.asSeconds(),0.f,m_scaleX*1.45f,2.f)),fabs(interpolate::elasticEaseOut(introFrames.asSeconds(),0.f,m_scaleX*1.45f,2.f)));

            ///////////////////////////////
            //updates for bonus messages
            ///////////////////////////////

            if(bonusFrames.asSeconds() < 3.f)
            {
                m_timeBonusMessage.move(-4.f*m_scaleX*interpolate::cubicEaseOut(bonusFrames.asSeconds(),0.f,1.f,3.f),0.f);
                m_survivalBonusMessage.move(-4.f*m_scaleX*interpolate::cubicEaseOut(bonusFrames.asSeconds(),0.f,1.f,3.f),0.f);
                m_accuracyBonusMessage.move(-4.f*m_scaleX*interpolate::cubicEaseOut(bonusFrames.asSeconds(),0.f,1.f,3.f),0.f);
                showBonus(window);
            }
            else
            {
                m_timeBonusMessage.setPosition(window.getSize().x*.9f, window.getSize().y/4.f);
                m_survivalBonusMessage.setPosition(window.getSize().x*.9f, window.getSize().y/6.f);
                m_accuracyBonusMessage.setPosition(window.getSize().x*.9f, window.getSize().y/12.f);
            }

            ////////////////////////////
            //perform player updates
            ////////////////////////////

            if(introFrames.asSeconds() > 10.f && pauseSwitch != 1)
            {
                //update the player
                hero.update(window, TimePerFrame);

                if(moveleft && hero.getPosition().x >= 0.f - hero.getAABB().x/2.f && pauseSwitch != 1)
                    hero.move(sf::Vector2f(-hero.getVel()*m_scaleX, 0.f),TimePerFrame);

                if(moveright && hero.getPosition().x <= m_winsizeX - hero.getAABB().x*2.f && pauseSwitch != 1)
                    hero.move(sf::Vector2f(hero.getVel()*m_scaleX, 0.f),TimePerFrame);

                if(moveup && hero.getPosition().y >= m_winsizeY/1.5f - hero.getAABB().y/2.f && pauseSwitch != 1)
                    hero.move(sf::Vector2f(0.f, -hero.getVel()*m_scaleX),TimePerFrame);

                if(movedown && hero.getPosition().y <= m_winsizeY*.85f - hero.getAABB().y && pauseSwitch != 1)
                    hero.move(sf::Vector2f(0.f, hero.getVel()*m_scaleX),TimePerFrame);

                if(ammoClock.getElapsedTime().asSeconds() < .01f)
                    hero.initAmmo(shooting, m_scaleY, m_shot);

                hero.updateProjectiles(TimePerFrame);
            }

            //////////////////////////////////////////////////////////////
            //perform *ONLY* the enemy animation while intro is playing
            //////////////////////////////////////////////////////////////

            if(m_wave < 4 && !m_enemyWave.isEmpty() && introFrames.asSeconds() < 10.f)
            {
                m_enemyWave.animate(TimePerFrame);
            }

            ///////////////////////////////////////
            //collision detection and resolution
            ///////////////////////////////////////

            if(m_wave < 4 && m_enemyWave.initStatus() && introFrames.asSeconds() > 10.f && pauseSwitch != 1)
            {
                if(!m_enemyWave.isEmpty())
                {
                    //update the player's bullets
                    hero.updateProjectiles(TimePerFrame);

                    //check collisions with enemies
                    sf::Uint32 scorePoints = hero.checkEnemyCollisions(window, m_enemyWave.m_enemies, m_explosionSpr, m_explosion, m_explodBuff, m_hitBuff, m_expAnim, "explode", m_scaleX);
                    if(scorePoints != nasic::player::hitList::miss)
                    {
                        //update the hit count
                        m_hits++;

                        //update the score
                        switch(scorePoints)
                        {
                        case nasic::player::hitList::agravu:
                        {
                            m_score += 10;
                        }
                        break;

                        case nasic::player::hitList::delsiriak:
                        {
                            m_score += 10;
                        }
                        break;

                        case nasic::player::hitList::gluorn:
                        {
                            m_score += 50;
                        }
                        break;

                        case nasic::player::hitList::rhiians:
                        {
                            m_score += 50;
                        }
                        break;

                        case nasic::player::hitList::hit:
                            {
                                m_score += 0;
                            }
                            break;

                        default:
                            {
                                m_score += 0;
                            }
                            break;
                        }
                    }
                    else
                    {
                        //update the miss count
                        m_misses++;
                    }

                    ///////////////////////////////////////////////////////////////
                    //do all updates for the enemies after the intro is over
                    //and *ONLY* if the list is not empty
                    //doing otherwise will result in a crash because the iterator
                    //will point to enemies that do not exist in our list
                    ///////////////////////////////////////////////////////////////

                    m_enemyWave.initProjectiles(TimePerFrame, m_scaleX);
                    m_enemyWave.updateEnemies(TimePerFrame, m_scaleX);

                    //check collisions with enemies - because the value
                    //we're retrieving is an enumerated type and a miss = 0,
                    //we need to check if the value returned by collision is > 0
                    collision = m_enemyWave.checkPlayerCollisions(window, hero, m_scaleX, m_explosionSpr, m_explosion, m_explodBuff, m_expAnim, "explode");

                    if(collision > 0)
                    {
                            life.setFillColor(sf::Color(255,255,0,200));

                            switch(collision)
                            {
                            case nasic::enemyWave::hitList::agravu:
                            {
                                if(hero.getHealth() <= 0 && m_numLives > 0)
                                {
                                    hero.heal(maxHealth);
                                    m_numLives -=1;
                                    if(m_lives.size() > 0)
                                        m_lives.pop_back();
                                }
                                else if(hero.getHealth() <=0 && m_numLives == 0) //player is a complete loser...
                                {
                                    m_levelstate = levelstate::lost;
                                    return;
                                }
                                else
                                    hero.damage(10);
                            }
                            break;

                            case nasic::enemyWave::hitList::delsiriak:
                            {
                                if(hero.getHealth() <= 0 && m_numLives > 0)
                                {
                                    hero.heal(maxHealth);
                                    m_numLives -=1;
                                    if(m_lives.size() > 0)
                                        m_lives.pop_back();
                                }
                                else if(hero.getHealth() <=0 && m_numLives == 0)//player is a complete loser...
                                {
                                    m_levelstate = levelstate::lost;
                                    return;
                                }
                                else
                                    hero.damage(10);
                            }
                            break;

                            case nasic::enemyWave::hitList::gluorn:
                            {
                                if(hero.getHealth() <= 0 && m_numLives > 0)
                                {
                                    hero.heal(maxHealth);
                                    m_numLives -=1;
                                    if(m_lives.size() > 0)
                                        m_lives.pop_back();
                                }
                                else if(hero.getHealth() <=0 && m_numLives == 0) //player is a complete loser...
                                {
                                    m_levelstate = levelstate::lost;
                                    return;
                                }
                                else
                                    hero.damage(20);
                            }
                            break;

                            case nasic::enemyWave::hitList::rhiians:
                            {
                                if(hero.getHealth() <= 0 && m_numLives > 0)
                                {
                                    hero.heal(maxHealth);
                                    m_numLives -=1;
                                    if(m_lives.size() > 0)
                                        m_lives.pop_back();
                                }
                                else if(hero.getHealth() <=0 && m_numLives == 0) //player is a complete loser...
                                {
                                    m_levelstate = levelstate::lost;
                                    return;
                                }
                                else
                                    hero.damage(20);
                            }
                            break;

                            default:
                                break;
                            }
                    }

                    else
                    {
                        life.setFillColor(sf::Color(0,255,0,200));
                    }

                }

                else
                {
                    //do the time bonus calculation
                    if(timeClock.asSeconds() < 30.f)
                        m_timeBonusFactor = 3;

                    else if(timeClock.asSeconds() < 60.f)
                        m_timeBonusFactor = 2;

                    else
                        m_timeBonusFactor = 1;

                    m_score = m_score*m_timeBonusFactor;//multiply score by bonus factor

                    //do the survival bonus calculation
                    if(m_numLives == initialLives)
                        m_survivalBonus = 10000*m_wave;//multiply by wave so it scales with difficulty to achieve the bonus

                    else if(m_numLives == initialLives - 1)
                        m_survivalBonus = 1000*m_wave;

                    else
                        m_survivalBonus = 0;

                    m_score = m_score+m_survivalBonus;

                    //do the accuracy bonus calculation
                    float tempRate = (float)m_hits/(m_hits+m_misses);
                    accuracyRatio = (int)100*(tempRate);

                    if(accuracyRatio > 90)
                        m_accuracyBonus = 10000;
                    else if(accuracyRatio > 75)
                        m_accuracyBonus = 5000;
                    else if(accuracyRatio > 50)
                        m_accuracyBonus = 1000;
                    else if(accuracyRatio < 50)
                        m_accuracyBonus = 0;

                    m_score = m_score+m_accuracyBonus;

                    bonusFrames = sf::Time::Zero;

                    //std::cout<<"Time: "<<timeClock.asSeconds()<<"\nInitial lives: "<<initialLives<<"\nNew lives: "<<m_numLives<<"\nHits: "<<m_hits<<"\nMisses: "<<m_misses<<"\nHit ratio: "<<accuracyRatio<<std::endl;
                    //std::cout<<"Time bonus: "<<m_timeBonusFactor<<"\nSurvival bonus: "<<m_survivalBonus<<"\nAccuracy bonus: "<<m_accuracyBonus<<std::endl;

                    ++m_wave;
                    timeClock = sf::Time::Zero;//reset the timer for bonuses
                    introFrames = sf::Time::Zero;
                    m_enemyWave.setInitStatus(false);
                    m_enemyWave.purgeEnemies();
                    m_enemyWave.init(window, 4, 10, m_scaleX);
                }
            }

            else if(m_wave == 4)
            {
                doBossFight = true;
                //a little housekeeping to reset boss intro frames
                ++tempcounter;
                if(tempcounter == 1)
                {
                    m_enemyWave.setInitStatus(false);
                    m_enemyWave.purgeEnemies();
                    bossIntroFrames = sf::Time::Zero;
                    killerDrone.play();
                }
            }

            if(doBossFight)
            {
                //////////////////////////////////////
                //perform boss animation updates
                //////////////////////////////////////

                m_killer.updateEyeColor(TimePerFrame);
                m_killer.animateMandibles(TimePerFrame, m_scaleX);

                //show the boss intro messages
                showBossIntro(window,bossIntroFrames);

                if(bossIntroFrames.asSeconds() < 1.f)
                    m_killer.move(0.f, m_scaleX*10.f*interpolate::expoEaseOut(bossIntroFrames.asSeconds(),0.f,1.f,1.f));

                m_killer.speak(TimePerFrame);

                if(bossIntroFrames.asSeconds() > 10.f && pauseSwitch != 1)
                {
                    //////////////////////////////////////
                    //perform boss speech/motion updates
                    //////////////////////////////////////

                    m_killer.updateMotion(TimePerFrame, m_scaleX, m_scaleY);
                    m_killer.updateState();

                    //////////////////////////////////////////
                    //perform boss ammo and particle updates
                    //////////////////////////////////////////

                    //instantiate as many particles as the frame rate will allow
                    m_killer.initParticles(thorDistr());

                    //update particles
                    m_killer.updateParticles(TimePerFrame,m_scaleX);
                    m_killer.fireAmmo(TimePerFrame, m_scaleX, m_scaleY);

                    if(m_killer.checkCollisions(window,hero, m_scaleX, m_scaleY) == nasic::killer::collisionType::gun)
                    {
                        //set the explosion position to the enemy location
                        m_explosionSpr.setTexture(expTexture);
                        m_explosionSpr.setPosition(hero.getPosition().x, hero.getPosition().y);
                        m_expAnim.playAnimation("explode", false);

                        //set the explosion sound buffer and play it
                        m_explosion.setBuffer(m_explodBuff);
                        m_explosion.play();

                        life.setFillColor(sf::Color(255,255,0,200));

                        if(hero.getHealth() <= 0 && m_numLives > 0)
                        {
                            hero.heal(maxHealth);
                            m_numLives -=1;
                            if(m_lives.size() > 0)
                                m_lives.pop_back();
                        }
                        else if(hero.getHealth() <=0 && m_numLives == 0) //player is a complete loser...
                        {
                            m_levelstate = levelstate::lost;
                            return;
                        }
                        else
                            hero.damage(10);
                    }

                    else if(m_killer.checkCollisions(window, hero, m_scaleX, m_scaleY) == nasic::killer::collisionType::cannon)
                    {
                        //set the explosion position to the player location
                        m_explosionSpr.setTexture(expTexture);
                        m_explosionSpr.setPosition(hero.getPosition().x, hero.getPosition().y);
                        m_expAnim.playAnimation("explode", false);

                        //set the explosion sound buffer and play it
                        m_explosion.setBuffer(m_explodBuff);
                        m_explosion.play();

                        life.setFillColor(sf::Color(255,255,0,200));

                        if(hero.getHealth() <= 0 && m_numLives > 0)
                        {
                            hero.heal(maxHealth);
                            m_numLives -=1;
                            if(m_lives.size() > 0)
                                m_lives.pop_back();
                        }
                        else if(hero.getHealth() <=0 && m_numLives == 0)//player is a complete loser...
                        {
                            m_levelstate = levelstate::lost;
                            return;
                        }
                        else
                            hero.damage(20);
                    }

                    else if(m_killer.checkCollisions(window, hero, m_scaleX, m_scaleY) == nasic::killer::collisionType::missile)
                    {
                        //set the explosion position to the enemy location
                        m_explosionSpr.setTexture(expTexture);
                        m_explosionSpr.setPosition(hero.getPosition().x, hero.getPosition().y);
                        m_expAnim.playAnimation("explode", false);

                        //set the explosion sound buffer and play it
                        m_explosion.setBuffer(m_explodBuff);
                        m_explosion.play();

                        life.setFillColor(sf::Color(255,255,0,200));

                        if(hero.getHealth() <= 0 && m_numLives > 0)
                        {
                            hero.heal(maxHealth);
                            m_numLives -=1;
                            if(m_lives.size() > 0)
                                m_lives.pop_back();
                        }
                        else if(hero.getHealth() <=0 && m_numLives == 0) //player is a complete loser...
                        {
                            m_levelstate = levelstate::lost;
                            return;
                        }
                        else
                            hero.damage(30);
                    }

                    else
                    {
                        life.setFillColor(sf::Color(0,255,0,200));
                    }

                    //update the player's bullets
                    hero.updateProjectiles(TimePerFrame);

                    //check for collisions against the boss
                    if(hero.checkBossCollisions(m_killer))
                    {
                        //play the explosion animation
                        m_explosionSpr.setTexture(blood);
                        m_expAnim.playAnimation("explode", false);

                        //set the explosion sound buffer and play it
                        m_explosion.setBuffer(m_explodBuff);
                        m_explosion.play();

                        m_score += 100;

                        bossHealth.setFillColor(sf::Color(255,155,0,200));

                        //update the hit count
                        m_hits++;

                        if(m_killer.getHealth() <= 0 && hero.getHealth() > 0)
                            m_wave = 5;//winner screen!!!

                    }

                    else
                    {
                        //update the miss count
                        m_misses++;
                        bossHealth.setFillColor(sf::Color(200,0,0,200));
                    }

                }
            }

            if(bossIntroFrames.asSeconds() < 2.f)
                m_bossMessage.setScale(fabs(interpolate::elasticEaseOut(bossIntroFrames.asSeconds(),0.f,m_scaleX*1.45f,2.f)),fabs(interpolate::elasticEaseOut(bossIntroFrames.asSeconds(),0.f,m_scaleX*1.45f,2.f)));

            m_introMessage.update(e,window);
            m_bossMessage.update(e,window);

            if(m_wave == 5)//check to see if player is full of win or a complete loser....
            {
                m_levelstate = levelstate::won;
                return;
            }
        }

        //check pause status for music
        if(pauseSwitch == 1)
        {
            m_music.pause();
            pauseCounter = 0;
        }

        else
        {
            pauseCounter++;
            if(pauseCounter == 1)
                m_music.play();
        }

        window.clear();

        window.draw(stars);

        if(m_wave < 4)
            window.draw(m_enemyWave);

        window.draw(hero);

        if(m_wave == 4)
        {
            window.draw(bossHealthLabel);
            bossHealth.setSize(sf::Vector2f(m_scaleX*m_killer.getHealth(),m_scaleX*5.f));
            window.draw(bossHealth);

            window.draw(m_killer);
        }

        if(m_expAnim.isPlayingAnimation())
            window.draw(m_explosionSpr);

        window.draw(bottomBG);
        window.draw(bottomLine);

        window.draw(score);

        m_scoreLabel.setString(toString(m_score));
        window.draw(m_scoreLabel);

        window.draw(lives);
        for(m_livesItr = m_lives.begin(); m_livesItr != m_lives.end(); ++m_livesItr)
        {
            window.draw(*m_livesItr);
        }

        m_timeLabel.setString("Time: " + toString((int)timeClock.asSeconds()));
        window.draw(m_timeLabel);

        window.draw(lifeLabel);

        life.setSize(sf::Vector2f(m_scaleX*hero.getHealth(),m_scaleX*5.f));
        window.draw(life);

        //draw the intro message conditionally on the intro time and wave
        if(introFrames.asSeconds() < 10.f && m_wave < 4)
            window.draw(m_introMessage);

        //draw the boss message conditionally on the boss intro time and wave
        if(bossIntroFrames.asSeconds() < 10.f && m_wave == 4)
            window.draw(m_bossMessage);

        //draw the bonus messages
        if(m_wave > 1 && m_timeBonusFactor > 1 && bonusFrames.asSeconds() < 3.f)
        {
            window.draw(m_timeBonusMessage);
        }
        if(m_wave > 1 && m_survivalBonus > 1 && bonusFrames.asSeconds() < 3.f)
        {
            window.draw(m_survivalBonusMessage);
        }

        if(m_wave > 1 && m_accuracyBonus > 1 && bonusFrames.asSeconds() < 3.f)
        {
            window.draw(m_accuracyBonusMessage);
        }

        //draw pause overlay if game is paused
        if(pauseSwitch == 1)
        {
            window.draw(pauseBg);
            window.draw(m_pauseOverlay);
            window.draw(rightCorner);
            window.draw(leftCorner);
        }

        window.display();
    }
    return;
}

void nasic::level::showIntro(sf::RenderWindow& window, sf::Time dt)
{
    if(m_wave == 1)
    {
        m_introMessage = gui::button("Time to destroy \nsome alien ships!", m_myfont, sf::Vector2f(window.getSize().x/2.f, window.getSize().y/2.f), gui::style::clean);

        if(dt.asSeconds() < 2.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
        }

        if(dt.asSeconds() > 2.f && dt.asSeconds() < 4.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Press the arrow \nkeys to move...");
        }

        else if(dt.asSeconds() > 4.f && dt.asSeconds() < 6.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Press the space \nbar to fire...");
        }

        else if(dt.asSeconds() > 6.f && dt.asSeconds() < 8.f)
        {
            m_introMessage.setSize(64);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*8.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Ready...");
        }

        else if(dt.asSeconds() > 8.f && dt.asSeconds() < 10.f)
        {
            m_introMessage.setSize(64);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Go!!!");
        }
    }

    if(m_wave == 2)
    {
        m_introMessage = gui::button("In N.A.S.I.C.,\nenemies come in waves.", m_myfont, sf::Vector2f(window.getSize().x/2.f, window.getSize().y/2.f), gui::style::clean);

        if(dt.asSeconds() < 2.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
        }

        if(dt.asSeconds() > 2.f && dt.asSeconds() < 4.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Destroy enemy ships\nas quickly and\naccurately as possible.");
        }

        else if(dt.asSeconds() > 4.f && dt.asSeconds() < 6.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("The more quickly\nand accurately you\ndestroy each wave,\nthe higher your\nscore will be.");
        }

        else if(dt.asSeconds() > 6.f && dt.asSeconds() < 8.f)
        {
            m_introMessage.setSize(64);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*8.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Ready...");
        }

        else if(dt.asSeconds() > 8.f && dt.asSeconds() < 10.f)
        {
            m_introMessage.setSize(64);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Go!!!");
        }
    }

    if(m_wave == 3)
    {
        m_introMessage = gui::button("In each level of N.A.S.I.C.,\nyou will encounter a boss.", m_myfont, sf::Vector2f(window.getSize().x/2.f, window.getSize().y/2.f), gui::style::clean);

        if(dt.asSeconds() < 2.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
        }

        if(dt.asSeconds() > 2.f && dt.asSeconds() < 4.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("After three waves of enemies,\na boss fight commences.");
        }

        else if(dt.asSeconds() > 4.f && dt.asSeconds() < 6.f)
        {
            m_introMessage.setSize(32);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Each boss has its\nunique weaknesses, and\nyou must learn to exploit them.");
        }

        else if(dt.asSeconds() > 6.f && dt.asSeconds() < 8.f)
        {
            m_introMessage.setSize(64);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*8.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Ready...");
        }

        else if(dt.asSeconds() > 8.f && dt.asSeconds() < 10.f)
        {
            m_introMessage.setSize(64);
            m_introMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*4.f));
            m_introMessage.setScale(m_scaleX, m_scaleX);
            m_introMessage.setText("Go!!!");
        }
    }
}

void nasic::level::showBossIntro(sf::RenderWindow& window, sf::Time dt)
{
    m_bossMessage = gui::button("Bosses are capable of\ndealing catastrophic damage...", m_myfont, sf::Vector2f(window.getSize().x/2.f, window.getSize().y/2.f), gui::style::clean);

    if(dt.asSeconds() < 2.f)
    {
        m_bossMessage.setSize(32);
        m_bossMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
    }

    if(dt.asSeconds() > 2.f && dt.asSeconds() < 4.f)
    {
        m_bossMessage.setSize(32);
        m_bossMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
        m_bossMessage.setScale(m_scaleX, m_scaleX);
        m_bossMessage.setText("Fortunately for you,\n-Killer- has some weaknesses.");
    }

    else if(dt.asSeconds() > 4.f && dt.asSeconds() < 6.f)
    {
        m_bossMessage.setSize(32);
        m_bossMessage.setLabelOffset(sf::Vector2f(0.f,m_scaleX*4.f));
        m_bossMessage.setScale(m_scaleX, m_scaleX);
        m_bossMessage.setText("Try firing into\nthe eyes...");
    }

    else if(dt.asSeconds() > 6.f && dt.asSeconds() < 8.f)
    {
        m_bossMessage.setSize(64);
        m_bossMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*8.f));
        m_bossMessage.setScale(m_scaleX, m_scaleX);
        m_bossMessage.setText("Ready...");
    }

    else if(dt.asSeconds() > 8.f && dt.asSeconds() < 10.f)
    {
        m_bossMessage.setSize(64);
        m_bossMessage.setLabelOffset(sf::Vector2f(0.f,-m_scaleX*4.f));
        m_bossMessage.setScale(m_scaleX, m_scaleX);
        m_bossMessage.setText("Go!!!");
    }
}

void nasic::level::showBonus(sf::RenderWindow& window)
{
    m_timeBonusMessage.setString("Time Bonus X " + toString(m_timeBonusFactor));
    m_timeBonusMessage.setColor(sf::Color(0,255,0,200));

    m_survivalBonusMessage.setString("Survival Bonus + " + toString(m_survivalBonus));
    m_survivalBonusMessage.setColor(sf::Color(255,255,0,200));

    m_accuracyBonusMessage.setString("Accuracy Bonus + " + toString(m_accuracyBonus));
    m_accuracyBonusMessage.setColor(sf::Color(255,0,255,200));
}

//not a part of the level object....
void nasic::addFrames(thor::FrameAnimation& animation, int w, int h, int y, int xFirst, int xLast, float duration)
{
    const int step = (xFirst < xLast) ? +1 : -1;
    xLast += step; // so xLast is excluded in the range

    for (int x = xFirst; x != xLast; x += step)
        animation.addFrame(duration, sf::IntRect(w*x, h*y, w, h));
}

//instantiate static members
sf::Uint32 nasic::level::m_levelstate = nasic::level::levelstate::uninitialized;
