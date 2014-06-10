#include <killer.hpp>

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

    m_helmet.setPosition(window.getSize().x/5.f, 0.f);
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

    m_eyeGlow.setSize(sf::Vector2f(scale*70.f,scale*15.f));
    m_eyeGlow.setFillColor(sf::Color(255,255,0,255));
    m_eyeGlow.setPosition(m_face.getPosition().x*1.1f, m_face.getPosition().y*1.25f);
}

void nasic::killer::animateMandibles(float angle)
{
    m_leftMandible.rotate(angle);
    m_rightMandible.rotate(-angle);
}

void nasic::killer::updateEyeColor(unsigned int g, unsigned int a)
{
    m_eyeGlow.setFillColor(sf::Color(255,g,0,a));
}

void nasic::killer::updateState()
{
    if(m_health > 30)
        m_killerState = killerState::normal;
    else if(m_health < 30 && m_health > 0)
        m_killerState = killerState::pissed;
    else if(m_health <= 0)
        m_killerState = killerState::dead;
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

sf::Vector2f nasic::killer::getPosition()const
{
    return m_helmet.getPosition();
}

void nasic::killer::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
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

    states.transform *= getTransform();
}
