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

#include <Game.hpp>

nasic::Game::Game()
{
    //ctor
}

nasic::Game::~Game()
{
    //dtor
}

void nasic::Game::init()
{
    using nasic::Game;
    //if the game is not in an uninitialized state
    //when it is first run, bail
    if(!m_state == state::s_uninitialized)
        return;

    sf::VideoMode mode = sf::VideoMode::getDesktopMode();

    std::vector<sf::VideoMode> modes = mode.getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
        << mode.width << "x" << mode.height << " - "
        << mode.bitsPerPixel << " bpp" << std::endl;
    }

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 0;

    m_window.setFramerateLimit(60);//set the refresh limit to the current frame rate 60fps
    m_window.create(sf::VideoMode(mode.width,mode.height,mode.bitsPerPixel), "Not Another Space Invaders Clone", sf::Style::Fullscreen, settings);

    m_state = state::s_intro;

    while(!isQuitting())
    {
        run();
    }
    std::cout<<"Quitting..."<<std::endl;
    m_window.close();
}

bool nasic::Game::isQuitting()
{
    using nasic::Game;
    return m_state == state::s_quit;
}

void nasic::Game::run()
{
    switch(m_state)
    {
    case state::s_intro:
    {
        intro();
        m_state = state::s_menu;
    }
    break;

    case state::s_menu:
    {
        menu();
    }
    break;

    case state::s_info:
    {
        info();
    }
    break;

    case state::s_options:
    {
        options();
    }
    break;

    case state::s_level:
    {
        level();
    }
    break;

    case state::s_win:
    {
        win();
    }
    break;

    case state::s_lose:
    {
        lose();
    }
    break;

    default:
        break;
    }
}

void nasic::Game::intro()
{
    using nasic::Game;
    m_window.setTitle("Intro");
    std::cout<<"Intro State"<<std::endl;

    //create an intro object
    nasic::intro gameIntro;
    gameIntro.show(m_window);
}

void nasic::Game::menu()
{
    m_window.setTitle("Menu");
    std::cout<<"Menu State"<<std::endl;

    //create a menu object
    nasic::menu gameMenu;
    gameMenu.show(m_window);
    if(gameMenu.menuState() == nasic::menu::s_play)
        m_state = state::s_level;
    else if(gameMenu.menuState() == nasic::menu::s_options)
        m_state = state::s_options;
    else if(gameMenu.menuState() == nasic::menu::s_info)
        m_state = state::s_info;
    else if(gameMenu.menuState() == nasic::menu::s_quit)
        m_state = state::s_quit;
}

void nasic::Game::info()
{
    m_window.setTitle("Info");
    std::cout<<"Info State"<<std::endl;

    //create an info object
    nasic::info inf;
    inf.show(m_window);
    if(inf.infoState() == nasic::info::infostate::done)
        m_state = state::s_menu;
}

void nasic::Game::options()
{
    m_window.setTitle("Options");
    std::cout<<"Options State"<<std::endl;

    //create an options object
    nasic::options opt;
    opt.show(m_window);
    if(opt.optionState() == nasic::options::optionstate::s_done)
        m_state = state::s_menu;
}

void nasic::Game::level()
{
    m_window.setTitle("Level");
    std::cout<<"Level State"<<std::endl;

    //create a level object
    nasic::level lvl;
    lvl.show(m_window);
    if(lvl.levelState() == nasic::level::exit)
        m_state = state::s_menu;
}

void nasic::Game::win()
{
    m_window.setTitle("You Win");
    std::cout<<"Win State"<<std::endl;

    //create a win object

    m_state = state::s_menu;
}

void nasic::Game::lose()
{
    m_window.setTitle("You Lose");
    std::cout<<"Lose State"<<std::endl;

    //create a lose object

    m_state = state::s_menu;
}

//initialize static member variables
sf::Uint32 nasic::Game::m_state = s_uninitialized;
sf::RenderWindow nasic::Game::m_window;
