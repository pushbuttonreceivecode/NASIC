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

#ifndef OPSTRUCT_HPP
#define OPSTRUCT_HPP

#include <iostream>
#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>

namespace nasic
{

class opstruct
{
    public:
        opstruct();
        ~opstruct();
        bool saveOptions(nasic::opstruct& op, const char * filename);
        bool loadOptions(nasic::opstruct& op, const char * filename);

        int m_volume;
        int m_effects;
        int m_difficulty;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_volume);
        ar & BOOST_SERIALIZATION_NVP(m_effects);
        ar & BOOST_SERIALIZATION_NVP(m_difficulty);
    }
};

}
#endif // OPSTRUCT_HPP
