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

#include <optionbox.hpp>

gui::optionbox::optionbox()
{
    //ctor
}

gui::optionbox::optionbox(std::string label, sf::Vector2f pos, sf::Uint32 precision, sf::RenderWindow& window)
{
    //ctor
    using gui::optionbox;
    window.setKeyRepeatEnabled(false);

    sf::Color bg = sf::Color(255,150,0,200);
    sf::Color red = sf::Color(200,0,0,200);
    sf::Color darkred = sf::Color(100,0,0,255);
    sf::Color white = sf::Color(255,255,255,100);
    sf::Color black = sf::Color(0,0,0,200);
    sf::Color tDarkRed = sf::Color(100,0,0,100);

    if(!myfont.loadFromFile("fonts/contb.ttf"))
    {
        std::cerr<<"Could not load contb.ttf"<<std::endl;
    }

    //initialize the control value to 0
    m_val = 0;
    m_precision = precision;
    m_focused = false;

    m_box = thor::Shapes::roundedRect(sf::Vector2f(window.getSize().x * 0.95f, window.getSize().y/5.f), 5.f, sf::Color(red), 1.f, sf::Color(darkred));
    m_box.setOrigin(m_box.getGlobalBounds().width/2.f, m_box.getGlobalBounds().height/2.f);
    m_box.setPosition(pos);

    m_label = sf::Text(label, myfont, 32);
    m_label.setOrigin(0.f, m_label.getGlobalBounds().height/2.f);
    m_label.setColor(black);
    m_label.setPosition(m_box.getPosition().x - m_box.getGlobalBounds().width/2.1f, m_box.getPosition().y - m_box.getGlobalBounds().height/2.5f);

    m_up = gui::button("  Up  ", myfont, sf::Vector2f(0.f,0.f), gui::style::none);
    m_down = gui::button("  Down  ", myfont, sf::Vector2f(0.f,0.f), gui::style::none);

    m_up.setPosition(sf::Vector2f(m_box.getPosition().x - m_box.getGlobalBounds().width/2.f + m_up.getDimensions().x, m_box.getPosition().y));
    m_down.setPosition(sf::Vector2f(m_box.getPosition().x + m_box.getGlobalBounds().width/2.f - m_down.getDimensions().x/1.25f, m_box.getPosition().y));
    m_up.setColorNormal(tDarkRed);
    m_up.setColorHover(white);
    m_up.setColorClicked(bg);
    m_down.setColorNormal(tDarkRed);
    m_down.setColorHover(white);
    m_down.setColorClicked(bg);

    m_line.setSize(sf::Vector2f(0.65f*(m_down.getPosition().x - m_up.getPosition().x), 1.f));
    m_line.setFillColor(white);
    m_line.setOrigin(0.f, m_line.getGlobalBounds().height/2.f);
    m_line.setPosition(m_box.getPosition().x - m_box.getGlobalBounds().width/4.f, m_box.getPosition().y);

    m_marker.setSize(sf::Vector2f(m_box.getGlobalBounds().width * 0.01f, m_box.getGlobalBounds().height/3.f));
    m_marker.setFillColor(tDarkRed);
    m_marker.setOrigin(m_marker.getGlobalBounds().width/2.f, m_marker.getGlobalBounds().height/2.f);
    m_marker.setPosition(m_line.getPosition().x, m_line.getPosition().y);
    m_initialPos = m_marker.getPosition().x;
    //std::cout<<m_initialPos<<std::endl;
}

gui::optionbox::~optionbox()
{
    //dtor
}

void gui::optionbox::update(sf::Event& e, sf::RenderWindow& window)
{
    m_up.update(e,window);
    m_down.update(e,window);

    if(m_up.getState() == 2 && m_val < m_precision)
        m_val++;

    else if(m_down.getState() == 2 && m_val > 0)
        m_val--;

    m_returnval = m_val * m_line.getGlobalBounds().width/m_precision;
    m_marker.setPosition(m_initialPos + (m_returnval), m_marker.getPosition().y);

    //update the "on focus" events
    sf::Color red = sf::Color(200,0,0,200);
    sf::Color white = sf::Color(255,255,255,100);

    m_focused ? m_box.setFillColor(white) : m_box.setFillColor(red);

    if(e.type == sf::Event::KeyPressed)
    {
        switch(e.key.code)
        {
            case sf::Keyboard::Left:
            {
                if(m_focused && m_val > 0)
                    m_val--;

            }
            break;

            case sf::Keyboard::Right:
            {
                if(m_focused && m_val < m_precision)
                    m_val++;
            }
            break;

            default:
                break;
        }
    }
}

sf::Uint32 gui::optionbox::getUpClicks()
{
    if(m_up.getState() == 2)
        m_upclicks++;
    else
        m_upclicks = 0;

    return m_upclicks;
}

sf::Uint32 gui::optionbox::getDownClicks()
{
    if(m_down.getState() == 2)
        m_downclicks++;
    else
        m_downclicks = 0;

    return m_downclicks;
}

void gui::optionbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_box, states);
    target.draw(m_label, states);
    target.draw(m_up, states);
    target.draw(m_down, states);
    target.draw(m_line, states);
    target.draw(m_marker, states);
}
