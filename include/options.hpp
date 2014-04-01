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

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <THOR/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <starfield.hpp>
#include <hexgrid.hpp>
#include <button.hpp>
#include <optionbox.hpp>
#include <opstruct.hpp>

namespace nasic
{

class options
{
public:
    options();
    ~options();
    void show(sf::RenderWindow& window);

    int optionState()
    {
        return m_optionstate;
    };

    enum optionstate
    {
        s_uninitialized,
        s_settingoptions,
        s_done
    };

private:
    static sf::Uint32 m_optionstate;
    nasic::opstruct m_options;
    float m_vol;
    float m_eff;
    float m_diff;
    std::string m_filename;
    sf::Uint32 m_initialVol;
    sf::Uint32 m_initialEff;
    sf::Uint32 m_initialDif;
};

}

#endif // OPTIONS_HPP
