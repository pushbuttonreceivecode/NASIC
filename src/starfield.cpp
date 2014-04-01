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

#include <starfield.hpp>

nasic::starfield::starfield(sf::RenderWindow& window, sf::Uint32 style, const float& scale)
{
    //set up starfield background
    using nasic::starfield;
    m_style = style;
    m_scale = scale;

    //set up for placing the stars on a uniform distribution
    std::mt19937 engine;
    std::uniform_int_distribution<int> distr(m_scale*-100, window.getSize().x);
    auto randomizer = std::bind(distr, engine);
    thor::Distribution<int> thorDistr(randomizer);

    //for calculating a random disturbance factor for offsetting stars
    //this helps to alleviate the clumping behavior seen when
    //the random distribution is less than random >.<
    std::uniform_int_distribution<int> disturb(m_scale*-50, m_scale*50);
    auto randdisturb = std::bind(disturb, engine);
    thor::Distribution<int> thorDisturb(randdisturb);

    //fill the container with stars
    //based on the style passed to the constructor
    std::vector<sf::CircleShape>::iterator starit;

    for(int i=0; i<75; ++i)
    {

        if(m_style == starStyle::allStars || m_style == starStyle::starsAndPlanets)
        {
            float x,y;
            x = thorDistr() + thorDisturb();
            y = thorDistr() + thorDisturb();//addition of disturbance for extra variability
            m_star = new sf::CircleShape;
            m_star->setRadius(5.f);
            m_star->setScale(m_scale, m_scale);
            m_star->setFillColor(sf::Color(0, 0, 255, 100));
            m_star->setPosition(i * m_star->getGlobalBounds().width + x, i * m_star->getGlobalBounds().height + y);
            m_stars.push_back(*m_star);
        }

        for(int j=0; j<75; ++j)
        {
            float x,y;
            x = thorDistr() + thorDisturb();//...again
            y = thorDistr() + thorDisturb();

            if(i == 1 && j == 25)
            {
                if(m_style == starStyle::starsAndPlanets)
                {
                    m_star = new sf::CircleShape;
                    m_star->setRadius(50.f);
                    m_star->setScale(m_scale, m_scale);
                    m_star->setFillColor(sf::Color(149, 69, 53, 255));
                    m_star->setPosition(i * m_star->getGlobalBounds().width + x, i * m_star->getGlobalBounds().height + y);
                    m_stars.push_back(*m_star);
                }
            }
            else
            {
                m_star = new sf::CircleShape;
                m_star->setRadius(1.f);
                m_star->setScale(m_scale, m_scale);
                m_star->setFillColor(sf::Color(255, 255, 255, 255));
                m_star->setPosition(i * m_star->getGlobalBounds().width + x, j * m_star->getGlobalBounds().height + y);
                m_stars.push_back(*m_star);
            }
        }
    }
}

nasic::starfield::~starfield()
{
    delete m_star;
}

void nasic::starfield::update(sf::RenderWindow& window, sf::Time dt)
{
    std::vector<sf::CircleShape>::iterator starit;
    for(starit = m_stars.begin(); starit != m_stars.end(); ++starit)
    {
        if(starit->getPosition().y < window.getSize().x * 1.2f)
        {
            if(starit->getRadius() == 1.f)
                starit->move(0.f,m_scale*(dt.asSeconds()*5.f));
            else if(starit->getRadius() == 5.f)
                starit->move(0.f,m_scale*(dt.asSeconds()*10.f));
            else if(starit->getRadius() == 50.f)
                starit->move(0.f,m_scale*(dt.asSeconds()*50.f));
        }
        else
        {
            if(starit->getRadius() == 1.f || starit->getRadius() == 5.f)
                starit->setPosition(starit->getPosition().x, 0.f - (window.getSize().y * .1f));

            else if(starit->getRadius() == 50.f)
            {
                //another random generator for resetting position
                //and color of planet
                int randPos = thor::random(-100,(int)window.getSize().x);
                int randR = thor::random(0,255);
                int randG = thor::random(0,255);
                int randB = thor::random(0,255);
                starit->setFillColor(sf::Color(randR,randG,randB,255));//set color of planet to random color
                starit->setPosition(randPos, 0.f - (window.getSize().y * .1f));
            }
        }
    }
}

void nasic::starfield::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //consider an additional star class
    //to keep track of z-indices
    //...I'm of the opinion that in this case
    //that would be unnecessarily complex
    //feel free to improve upon it ;)

    //drawing shapes conditionally via separate loops
    //simulates the effects nicely enough and without fuss
    //because there aren't many variations on this theme

    std::vector<sf::CircleShape>::const_iterator starit;
    //draw small stars first...
    for(starit = m_stars.begin(); starit != m_stars.end(); ++starit)
    {
        if(starit->getRadius() == 1.f)
            target.draw(*starit, states);
    }

    //...then the larger stars....
    for(starit = m_stars.begin(); starit != m_stars.end(); ++starit)
    {
        if(starit->getRadius() == 5.f)
            target.draw(*starit, states);
    }

    //...and finally, the planets
    for(starit = m_stars.begin(); starit != m_stars.end(); ++starit)
    {
        if(starit->getRadius() == 50.f)
            target.draw(*starit, states);
    }
}

//instantiate static members
sf::Uint32 nasic::starfield::m_style = nasic::starfield::starStyle::smallStars;
