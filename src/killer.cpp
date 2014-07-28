#include <killer.hpp>
#include <player.hpp>

nasic::killer::killer()
{
    m_health = 100;
    m_killerState = killerState::normal;
}

nasic::killer::~killer()
{

}

void nasic::killer::init(sf::RenderWindow& window, float scale)
{

    if(!face.loadFromFile("img/killerFace.png"))
    {
        std::cerr<<"Could not load killerFace.png"<<std::endl;
    }

    if(!helmet.loadFromFile("img/killerHelmet.png"))
    {
        std::cerr<<"Could not load killerHelmet.png"<<std::endl;
    }

    if(!leftEar.loadFromFile("img/killerLeftEar.png"))
    {
        std::cerr<<"Could not load killerLeftEar.png"<<std::endl;
    }

    if(!rightEar.loadFromFile("img/killerRightEar.png"))
    {
        std::cerr<<"Could not load killerRightEar.png"<<std::endl;
    }

    if(!leftEarring.loadFromFile("img/killerLeftEarring.png"))
    {
        std::cerr<<"Could not load killerLeftEarring.png"<<std::endl;
    }

    if(!rightEarring.loadFromFile("img/killerRightEarring.png"))
    {
        std::cerr<<"Could not load killerRightEarring.png"<<std::endl;
    }

    if(!leftMandible.loadFromFile("img/killerLeftMandible.png"))
    {
        std::cerr<<"Could not load killerLeftMandible.png"<<std::endl;
    }

    if(!rightMandible.loadFromFile("img/killerRightMandible.png"))
    {
        std::cerr<<"Could not load killerRightMandible.png"<<std::endl;
    }

    if(!leftTooth.loadFromFile("img/killerLeftTooth.png"))
    {
        std::cerr<<"Could not load killerLeftTooth.png"<<std::endl;
    }

    if(!rightTooth.loadFromFile("img/killerRightTooth.png"))
    {
        std::cerr<<"Could not load killerRightTooth.png"<<std::endl;
    }

    m_face.setTexture(face);
    m_helmet.setTexture(helmet);
    m_leftEar.setTexture(leftEar);
    m_rightEar.setTexture(rightEar);
    m_leftTooth.setTexture(leftTooth);
    m_rightTooth.setTexture(rightTooth);
    m_leftMandible.setTexture(leftMandible);
    m_rightMandible.setTexture(rightMandible);
    m_leftEarring.setTexture(leftEarring);
    m_rightEarring.setTexture(rightEarring);

    m_leftMandible.setOrigin(m_leftMandible.getGlobalBounds().width/2.f, 0.f);
    m_rightMandible.setOrigin(m_rightMandible.getGlobalBounds().width/2.f, 0.f);

    m_face.setScale(scale,scale);
    m_helmet.setScale(scale,scale);
    m_leftEar.setScale(scale,scale);
    m_rightEar.setScale(scale,scale);
    m_leftCannon = m_leftEar;
    m_rightCannon = m_rightEar;
    m_leftTooth.setScale(scale,scale);
    m_rightTooth.setScale(scale,scale);
    m_leftMandible.setScale(scale,scale);
    m_rightMandible.setScale(scale,scale);
    m_leftEarring.setScale(scale,scale);
    m_rightEarring.setScale(scale,scale);

    m_helmet.setPosition(window.getSize().x/5.f, -scale*100.f);
    m_face.setPosition(m_helmet.getPosition().x + m_helmet.getGlobalBounds().width/9.f, m_helmet.getPosition().y + (m_helmet.getGlobalBounds().height * .85f));
    m_leftEar.setPosition(m_face.getPosition().x - m_leftEar.getGlobalBounds().width, m_face.getPosition().y);
    m_rightEar.setPosition(m_face.getPosition().x + m_face.getGlobalBounds().width, m_face.getPosition().y);
    m_leftCannon.setPosition(m_leftEar.getPosition().x - m_leftEar.getGlobalBounds().width/2.f, m_leftEar.getPosition().y - m_leftEar.getGlobalBounds().height/2.f);
    m_rightCannon.setPosition(m_rightEar.getPosition().x + m_rightEar.getGlobalBounds().width/2.f, m_rightEar.getPosition().y - m_rightEar.getGlobalBounds().height/2.f);
    m_leftEarring.setPosition(m_leftEar.getPosition().x, m_leftEar.getPosition().y + m_leftEar.getGlobalBounds().height);
    m_rightEarring.setPosition(m_rightEar.getPosition().x + m_rightEar.getGlobalBounds().width - m_rightEarring.getGlobalBounds().width, m_rightEar.getPosition().y + m_rightEar.getGlobalBounds().height);
    m_leftTooth.setPosition(m_face.getPosition().x + m_face.getGlobalBounds().width/2.f - m_leftTooth.getGlobalBounds().width, m_face.getPosition().y + m_face.getGlobalBounds().height);
    m_rightTooth.setPosition(m_face.getPosition().x + m_face.getGlobalBounds().width/2.f, m_face.getPosition().y + m_face.getGlobalBounds().height);
    m_leftMandible.setPosition(m_face.getPosition().x + m_leftMandible.getGlobalBounds().width/2.f, m_face.getPosition().y + m_face.getGlobalBounds().height);
    m_rightMandible.setPosition(m_face.getPosition().x + m_face.getGlobalBounds().width - m_rightMandible.getGlobalBounds().width/2.f, m_face.getPosition().y + m_face.getGlobalBounds().height);

    m_eyeGlow.setSize(sf::Vector2f(scale*70.f,scale*20.f));
    m_eyeGlow.setFillColor(sf::Color(255,255,0,255));
    m_eyeGlow.setPosition(m_face.getPosition().x + m_face.getGlobalBounds().width*.1f, m_face.getPosition().y + m_face.getGlobalBounds().height*.2f);

    //set up for particles
    if (!particleTexture.loadFromFile("img/particle.png"))
    {
        std::cerr<<"Could not load particle.png"<<std::endl;
    }

    m_particle.setTexture(particleTexture);
    m_particle.setColor(sf::Color(255,255,255,255));

    //motion set up
    m_direction = 1;
    m_motionFrames = sf::Time::Zero;

    //eye set up
    m_eyeColorSwitch = 1;
    m_eyeColorFrames = sf::Time::Zero;

    //mandible set up
    m_mandibleRotationDirection = 1;
    m_mandibleFrames = sf::Time::Zero;

    //initialize time variables
    m_bossMandibleFrames = sf::Time::Zero;
    m_bossCannonFrames = sf::Time::Zero;
    m_bossMissileFrames = sf::Time::Zero;
    m_bossGunFrames = sf::Time::Zero;
    m_bossBurstFrames = sf::Time::Zero;
    m_missileAmmoFrames = sf::Time::Zero;
    m_missileBurstFrames = sf::Time::Zero;
    m_particleFrames = sf::Time::Zero;

    //sound set up
    if(!m_laughBuffer.loadFromFile("sound/evil laugh.wav"))
    {
        std::cerr<<"Could not load evil laugh.wav."<<std::endl;
    }

    if(!m_tauntBuffer.loadFromFile("sound/destroy all humanoids.wav"))
    {
        std::cerr<<"Could not load destroy all humanoids.wav."<<std::endl;
    }

    if(!m_deadBuffer.loadFromFile("sound/noooo.wav"))
    {
        std::cerr<<"Could not load noooo.wav."<<std::endl;
    }

    m_vocals.setVolume(100.f);
    m_vocals.setBuffer(m_laughBuffer);

    m_playSwitch = false;
    m_speechSwitch = 1;
    m_speechCounter = 0;
}

void nasic::killer::speak(sf::Time dt)
{
    if(m_speechSwitch == 1 && m_playSwitch)
    {
        m_vocals.setBuffer(m_laughBuffer);
    }

    else if(m_speechSwitch == -1 && m_playSwitch)
    {
        m_vocals.setBuffer(m_tauntBuffer);

    }

    if(m_killerState == killerState::dead)
    {
        m_vocals.setBuffer(m_deadBuffer);
        m_playSwitch = true;
    }

    if(m_speechCounter == 1)
    {
        m_vocals.play();
    }

    /////////////////////////////////////////////
    //do the housekeeping necessary to play
    //the sounds and stop them from repeating
    /////////////////////////////////////////////

    if(m_speechFrames.asSeconds() > 10.f)//say something every 10 seconds
    {
        m_speechSwitch *= -1;
        m_playSwitch = true;
        m_speechFrames = sf::Time::Zero;
    }

    else
    {
        m_playSwitch = false;
        m_speechFrames += dt;
    }

    if(!m_playSwitch)
        m_speechCounter = 0;

    else
        ++m_speechCounter;

}

void nasic::killer::animateMandibles(sf::Time dt, float scale)
{

    switch(m_killerState)
    {
    case killerState::normal:
    {
        if(m_mandibleFrames.asSeconds() > 1.f)
        {
            if(m_mandibleRotationDirection == -1)
            {
                m_leftMandible.setRotation(0.f);
                m_rightMandible.setRotation(0.f);
            }

            m_mandibleFrames = sf::Time::Zero;
            m_mandibleRotationDirection *= -1;
        }

        else
        {
            m_mandibleFrames += dt;
        }

        m_leftMandible.rotate(scale*m_mandibleRotationDirection*interpolate::expoEaseIn(m_mandibleFrames.asSeconds(), 0.f, 1.f, 1.f));
        m_rightMandible.rotate(-scale*m_mandibleRotationDirection*interpolate::expoEaseIn(m_mandibleFrames.asSeconds(), 0.f, 1.f, 1.f));
    }
    break;

    case killerState::pissed:
    {
        if(m_mandibleFrames.asSeconds() > 0.25f)
        {
            if(m_mandibleRotationDirection == -1)
            {
                m_leftMandible.setRotation(0.f);
                m_rightMandible.setRotation(0.f);
            }

            m_mandibleFrames = sf::Time::Zero;
            m_mandibleRotationDirection *= -1;
        }

        else
        {
            m_mandibleFrames += dt;
        }

        m_leftMandible.rotate(scale*5.f*m_mandibleRotationDirection*interpolate::expoEaseIn(m_mandibleFrames.asSeconds(), 0.f, 1.f, 0.25f));
        m_rightMandible.rotate(-scale*5.f*m_mandibleRotationDirection*interpolate::expoEaseIn(m_mandibleFrames.asSeconds(), 0.f, 1.f, 0.25f));
    }
    break;

    default:
        break;
    }
}

void nasic::killer::fireAmmo(sf::Time dt, float scaleX, float scaleY)
{
    m_bossCannonFrames += dt;
    m_bossMissileFrames += dt;
    m_bossGunFrames += dt;

    if(m_missileAmmoFrames.asSeconds() > 3.f)
        m_missileAmmoFrames = sf::Time::Zero;

    else
        m_missileAmmoFrames += dt;

    if(m_bossBurstFrames.asSeconds() > .5f)
        m_bossBurstFrames = sf::Time::Zero;

    else
        m_bossBurstFrames += dt;

    if(m_missileBurstFrames.asSeconds() > 3.f)
        m_missileBurstFrames = sf::Time::Zero;
    else
        m_missileBurstFrames += dt;

    //load up the enemy ammo list with gunfire every 1/4 of a second
    if(m_bossGunFrames.asSeconds() > .1f && m_bossBurstFrames.asSeconds() > 0.25f)
    {
        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::delsiriak, sf::Vector2f(leftGunPosition().x  + scaleX*15.f, leftGunPosition().y - scaleY*10.f), scaleY);
        m_enemyAmmo.push_back(*m_ammoPtr);

        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::delsiriak, sf::Vector2f(rightGunPosition().x, rightGunPosition().y - scaleY*10.f), scaleY);
        m_enemyAmmo.push_back(*m_ammoPtr);

        m_bossGunFrames = sf::Time::Zero;
    }

    //load up the enemy ammo list with cannon fire every 1/2 second
    if(m_bossCannonFrames.asSeconds() > .1f && m_bossBurstFrames.asSeconds() > 0.5f)
    {
        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::rhiians, sf::Vector2f(leftCannonPosition().x + scaleX*10.f, leftCannonPosition().y + scaleY*60.f), scaleY);
        m_enemyAmmo.push_back(*m_ammoPtr);

        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::rhiians, sf::Vector2f(rightCannonPosition().x + scaleX*30.f, rightCannonPosition().y + scaleY*60.f), scaleY);
        m_enemyAmmo.push_back(*m_ammoPtr);

        m_bossCannonFrames = sf::Time::Zero;
    }

    //load up the enemy missile list with missiles every 3 seconds
    if(m_bossMissileFrames.asSeconds() > .1f && m_missileBurstFrames.asSeconds() > 3.f)
    {
        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::missile, leftEyeLaserPosition(), scaleY);
        m_missileAmmo.push_back(*m_ammoPtr);

        m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::missile, rightEyeLaserPosition(), scaleY);
        m_missileAmmo.push_back(*m_ammoPtr);

        m_bossMissileFrames = sf::Time::Zero;
    }

    //fire ammo
    for(m_eAmmoIt = m_enemyAmmo.begin(); m_eAmmoIt != m_enemyAmmo.end(); ++m_eAmmoIt)
    {
        m_eAmmoIt->fire(dt);
    }

    //missiles are a special case as ammo, therefore,
    //the firing of missiles will be done specially...;)
    for(m_missileIt = m_missileAmmo.begin(); m_missileIt != m_missileAmmo.end(); ++m_missileIt)
    {
        if(m_missileAmmoFrames.asSeconds() < 2.f)
            m_missileIt->fire(m_missileAmmoFrames);
    }
}

sf::Uint32 nasic::killer::checkCollisions(sf::RenderWindow& window, nasic::player& hero, float scaleX, float scaleY)
{
    //////////////////////////////////////
    //handle ammo/missiles leaving screen
    //////////////////////////////////////

    if(m_enemyAmmo.size() > 0)
    {
        //ammo (cannon and gun fire)
        std::list<nasic::ammo>::iterator ammoIt;
        for(ammoIt = m_enemyAmmo.begin(); ammoIt != m_enemyAmmo.end(); ++ammoIt)
        {
            if(ammoIt->getPosition().y > window.getSize().y)
            {
                ammoIt = m_enemyAmmo.erase(ammoIt);//delete missed shots...
            }

            if(ammoIt->getPosition().y >= hero.getPosition().y
                && ammoIt->getPosition().y <= hero.getPosition().y + scaleX*hero.getAABB().y
                && ammoIt->getPosition().x >= hero.getPosition().x
                && ammoIt->getPosition().x <= hero.getPosition().x + scaleX*hero.getAABB().x)
            {
                ammoIt = m_enemyAmmo.erase(ammoIt);//delete missed shots...
                return collisionType::gun;//we've got a hit!!
            }
        }
    }

    //missiles...
    if(m_missileAmmo.size() > 0)
    {
        std::list<nasic::ammo>::iterator missileIt;
        for(missileIt = m_missileAmmo.begin(); missileIt != m_missileAmmo.end(); ++missileIt)
        {
            if(missileIt->getPosition().y >= window.getSize().y)
            {

                missileIt = m_missileAmmo.erase(missileIt);
            }

            if(missileIt->getPosition().y >= hero.getPosition().y
                && missileIt->getPosition().y <= hero.getPosition().y + scaleX*hero.getAABB().y
                && missileIt->getPosition().x >= hero.getPosition().x
                && missileIt->getPosition().x <= hero.getPosition().x + scaleX*hero.getAABB().x)
            {
                missileIt = m_missileAmmo.erase(missileIt);
                return collisionType::missile;//we've got a hit!!
            }
        }
    }

    //if all tests fail...
    return collisionType::miss;
}

void nasic::killer::updateEyeColor(sf::Time dt)
{
    //updates and time for pause message
    if(m_eyeColorFrames.asSeconds() > 2.f)
    {
        m_eyeColorFrames = sf::Time::Zero;
        m_eyeColorSwitch *= -1;
    }

    else
        m_eyeColorFrames += dt;

    float r = interpolate::sineEaseIn(m_eyeColorFrames.asSeconds(),0.f,255.f,2.f);
    float g = interpolate::sineEaseIn(m_eyeColorFrames.asSeconds(),0.f,255.f,2.f);
    float b = interpolate::sineEaseIn(m_eyeColorFrames.asSeconds(),0.f,255.f,2.f);

    if(m_eyeColorSwitch == -1 && r < 255)
    {
        if(m_killerState == killerState::normal)
            m_eyeGlow.setFillColor(sf::Color(0,255-(unsigned int)g, (unsigned int)b,255));

        else if(m_killerState == killerState::pissed)
            m_eyeGlow.setFillColor(sf::Color(255,255-(unsigned int)g, 0,255));
    }

    if(m_eyeColorSwitch == 1 && r < 255)
    {
        if(m_killerState == killerState::normal)
            m_eyeGlow.setFillColor(sf::Color(0,(unsigned int)g,255-(unsigned int)b,255));

        else if(m_killerState == killerState::pissed)
            m_eyeGlow.setFillColor(sf::Color(255,(unsigned int)g,0,255));
    }
}

void nasic::killer::initParticles(int disturbance)
{
    //instantiate as many particles as the frame rate will allow
    m_particle.setPosition(getPosition().x + getAABB().x/4.f + disturbance, getPosition().y + getAABB().y * 1.5f);
    particles.push_back(m_particle);
}

void nasic::killer::updateParticles(sf::Time dt, float scale)
{
    for(particleIt = particles.begin(); particleIt != particles.end(); ++particleIt)
    {
        if(!particleIt->isAlive())
            particleIt = particles.erase(particleIt);
        else
        {
            particleIt->move(0.f, scale*2.f);
            particleIt->decAlpha(dt);
        }
    }
}

void nasic::killer::updateState()
{
    if(getHealth() > 50)
        m_killerState = killerState::normal;
    else if(getHealth() < 50)
        m_killerState = killerState::pissed;
    else if(getHealth() <= 0)
        m_killerState = killerState::dead;
}

void nasic::killer::toggleState(int s)
{
    switch(s)
    {
    case 1:
    {
        m_killerState = killerState::normal;
    }
    break;

    case -1:
    {
        m_killerState = killerState::pissed;
    }
    break;

    default:
        break;
    }
}

void nasic::killer::move(float offsetX, float offsetY)
{
    m_helmet.move(offsetX, offsetY);
    m_face.move(offsetX, offsetY);
    m_leftEar.move(offsetX, offsetY);
    m_rightEar.move(offsetX, offsetY);
    m_leftCannon.move(offsetX, offsetY);
    m_rightCannon.move(offsetX, offsetY);
    m_leftEarring.move(offsetX, offsetY);
    m_rightEarring.move(offsetX, offsetY);
    m_leftTooth.move(offsetX, offsetY);
    m_rightTooth.move(offsetX, offsetY);
    m_leftMandible.move(offsetX, offsetY);
    m_rightMandible.move(offsetX, offsetY);
    m_eyeGlow.move(offsetX, offsetY);
}

void nasic::killer::updateMotion(sf::Time dt, float scaleX, float scaleY)
{
    switch(m_killerState)
    {
    case killerState::normal:
    {
        if(m_motionFrames.asSeconds() >= 5.f)
        {
            m_motionFrames = sf::Time::Zero;
            m_direction *= -1;//switch directions...
        }

        else
        {
            m_motionFrames += dt;
        }

        move(m_direction*(scaleX*10.f)*(interpolate::backEaseOut(m_motionFrames.asSeconds(), 0.f, 1.f, 5.f)), scaleY*m_motionFrames.asSeconds()*(float)m_direction/12.f);
    }
    break;

    case killerState::pissed:
    {
        if(m_motionFrames.asSeconds() >= 3.f)
        {
            m_motionFrames = sf::Time::Zero;
            m_direction *= -1;//switch directions...
        }

        else
        {
            m_motionFrames += dt;
        }

        move(m_direction*(scaleX*20.f)*(interpolate::backEaseOut(m_motionFrames.asSeconds(), 0.f, 1.f, 3.f)), scaleY*m_motionFrames.asSeconds()*(float)m_direction/8.f);
    }
    break;

    default:
        break;
    }
}

sf::Vector2f nasic::killer::getPosition()const
{
    return m_helmet.getPosition();
}

void nasic::killer::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
    //draw particles
    std::list<nasic::particle>::const_iterator partIt;
    if(particles.size() > 0)
    {
        for(partIt = particles.begin(); partIt != particles.end(); ++partIt)
        {
            target.draw(*partIt, states);
        }
    }

    target.draw(m_eyeGlow,states);
    target.draw(m_face,states);
    target.draw(m_helmet,states);
    target.draw(m_leftEar,states);
    target.draw(m_rightEar,states);
    target.draw(m_leftCannon,states);
    target.draw(m_rightCannon,states);
    target.draw(m_leftTooth,states);
    target.draw(m_rightTooth,states);
    target.draw(m_leftMandible,states);
    target.draw(m_rightMandible,states);
    target.draw(m_leftEarring,states);
    target.draw(m_rightEarring,states);

    //draw the ammo from the std::list containers
    std::list<ammo>::const_iterator constMissileAmmoIt;
    for(constMissileAmmoIt = m_missileAmmo.begin(); constMissileAmmoIt != m_missileAmmo.end(); ++constMissileAmmoIt)
    {
        target.draw(*constMissileAmmoIt, states);
    }

    std::list<ammo>::const_iterator constAmmoIt;
    for(constAmmoIt = m_enemyAmmo.begin(); constAmmoIt != m_enemyAmmo.end(); ++constAmmoIt)
    {
        target.draw(*constAmmoIt, states);
    }


    states.transform *= getTransform();
}
