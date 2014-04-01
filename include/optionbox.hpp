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

#ifndef OPTIONBOX_HPP
#define OPTIONBOX_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <THOR/Graphics.hpp>
#include <button.hpp>

namespace gui
{
    class optionbox : public sf::Drawable
    {
        public:
            optionbox();
            optionbox(std::string label, sf::Vector2f pos, sf::Uint32 precision, sf::RenderWindow& window);
            ~optionbox();

            sf::Uint32 getVal(){return m_val;};
            void setVal(int v){m_val = v;};
            void incVal(){m_val++;};
            void decVal(){m_val--;};
            sf::Uint32 getUpBtn(){return m_up.getState();};
            sf::Uint32 getDownBtn(){return m_down.getState();};
            sf::Uint32 getUpClicks();
            sf::Uint32 getDownClicks();
            void setFocus(bool f){m_focused = f;};

            void update(sf::Event& e, sf::RenderWindow& window);

            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:

        sf::Text m_label;
        sf::Text m_valLabel;

        float m_initialPos;
        sf::Uint32 m_val;
        sf::Uint32 m_precision;
        sf::Uint32 m_returnval;
        sf::Uint32 m_upclicks;
        sf::Uint32 m_downclicks;
        bool m_focused;

        sf::ConvexShape m_box;

        gui::button m_up;
        gui::button m_down;

        sf::RectangleShape m_line;
        sf::RectangleShape m_marker;

        sf::Font myfont;
    };
}

#endif // OPTIONBOX_HPP
