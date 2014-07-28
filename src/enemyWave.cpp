#include <enemyWave.hpp>

nasic::enemyWave::enemyWave()
{
    m_enemyMoveFrames = sf::Time::Zero;
    m_enemyFireFrames = sf::Time::Zero;
}

nasic::enemyWave::~enemyWave()
{

}

void nasic::enemyWave::init(sf::RenderWindow& window, int rows, int columns, float scale)
{
    float offset;

    //fill the grid w/ enemies
    for(int i=0; i<rows; ++i)
    {
        for(int j=0; j<columns; ++j)
        {
            switch(i)
            {
            case 0:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Rhiians, scale);
                m_enemy->setId(sf::Vector2u(i,j));
                offset = scale*50.f;
                m_enemy->setInitialPosition(sf::Vector2f(j * offset + window.getSize().x/16.f, i * offset + offset/2.f));
                m_enemy->setPosition(j * offset + window.getSize().x/16.f, i * offset + offset/2.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            case 1:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Agravu, scale);
                m_enemy->setId(sf::Vector2u(i,j));
                offset = scale*50.f;
                m_enemy->setInitialPosition(sf::Vector2f(j * offset + window.getSize().x/16.f, i * offset + offset/2.f));
                m_enemy->setPosition(j * offset + window.getSize().x/16.f, i * offset + offset/2.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            case 2:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Delsiriak, scale);
                m_enemy->setId(sf::Vector2u(i,j));
                offset = scale*50.f;
                m_enemy->setInitialPosition(sf::Vector2f(j * offset + window.getSize().x/16.f, i * offset + offset/2.f));
                m_enemy->setPosition(j * offset + window.getSize().x/16.f, i * offset + offset/5.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            case 3:
            {
                m_enemy = new nasic::enemy(nasic::enemy::enemyType::Gluorn, scale);
                m_enemy->setId(sf::Vector2u(i,j));
                offset = scale*50.f;
                m_enemy->setInitialPosition(sf::Vector2f(j * offset + window.getSize().x/16.f, i * offset + offset/2.f));
                m_enemy->setPosition(j * offset + window.getSize().x/16.f, i * offset + offset/6.f);
                m_enemies.push_back(*m_enemy);
            }
            break;

            default:
                break;
            }
        }
    }
    setInitStatus(true);//tell the caller initialization is done
    m_dir = 1;//set the direction of the enemy hoard to move right
    m_enemyMoveFrames = sf::Time::Zero;//reset the frames controlling movement
}

void nasic::enemyWave::updateEnemies(sf::Time dt, float scale)
{
    //frame updates for enemy movement
    if(m_enemyMoveFrames.asSeconds() >= 5.f)
    {
        m_enemyMoveFrames = sf::Time::Zero;
        m_dir *= -1;//switch directions...
        //std::cout<<"direction: "<<m_dir<<std::endl;
    }

    else
    {
        m_enemyMoveFrames += dt;
    }

    //movement for enemies
    std::list<nasic::enemy>::iterator enemIt;
    for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
    {
        //perform animation/state updates
        (*enemIt).update(dt);

        //must multiply by the direction int to keep the
        //enemies oscillating back and forth
        if(m_enemyMoveFrames.asSeconds() < 5.f)
            (*enemIt).move(m_dir*((5*scale)*(interpolate::backEaseOut(m_enemyMoveFrames.asSeconds(), 0.f, 1.f, 5.f))), scale*m_enemyMoveFrames.asSeconds()*(float)m_dir/12.f);
    }

    updateProjectiles(dt);
}

void nasic::enemyWave::initProjectiles(sf::Time dt, float scale)
{
    m_enemyFireFrames += dt;

    //load up the enemy ammo vector every second
    if(m_enemyFireFrames.asSeconds() > 1.f && m_enemyAmmo.size() < 1)
    {
        int pickRandEnemy = rand()% m_enemies.size();
        std::list<nasic::enemy>::iterator randEnemyIt = m_enemies.begin();

        //advance the list iterator to a random enemy
        //in the list every second
        std::advance(randEnemyIt, pickRandEnemy);

        switch(randEnemyIt->getType())
        {
        case 0:
        {
            m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::agravu, sf::Vector2f(randEnemyIt->getPosition().x + randEnemyIt->getAABB().x/2.f, randEnemyIt->getPosition().y + randEnemyIt->getAABB().y/2.f), scale);
            m_enemyAmmo.push_back(*m_ammoPtr);
        }
        break;
        case 1:
        {
            m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::delsiriak, sf::Vector2f(randEnemyIt->getPosition().x + randEnemyIt->getAABB().x/2.f, randEnemyIt->getPosition().y + randEnemyIt->getAABB().y/2.f), scale);
            m_enemyAmmo.push_back(*m_ammoPtr);
        }
        break;
        case 2:
        {
            m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::gluorn, sf::Vector2f(randEnemyIt->getPosition().x + randEnemyIt->getAABB().x/2.f, randEnemyIt->getPosition().y + randEnemyIt->getAABB().y/2.f), scale);
            m_enemyAmmo.push_back(*m_ammoPtr);
        }
        break;
        case 3:
        {
            m_ammoPtr = new nasic::ammo(nasic::ammo::ammoType::rhiians, sf::Vector2f(randEnemyIt->getPosition().x + randEnemyIt->getAABB().x/2.f, randEnemyIt->getPosition().y + randEnemyIt->getAABB().y/2.f), scale);
            m_enemyAmmo.push_back(*m_ammoPtr);
        }
        break;
        default:
            break;
        }
        m_enemyFireFrames = sf::Time::Zero;
    }
}

void nasic::enemyWave::updateProjectiles(sf::Time dt)
{
    if(m_enemyAmmo.size() > 0)
    {
        for(m_eAmmoIt = m_enemyAmmo.begin(); m_eAmmoIt != m_enemyAmmo.end(); ++m_eAmmoIt)
        {
            m_eAmmoIt->fire(dt);
        }
    }
}

sf::Uint32 nasic::enemyWave::checkPlayerCollisions(sf::RenderWindow& window, nasic::player& hero, float scale, sf::Sprite& explosion, sf::Sound& explode, sf::SoundBuffer& buffer, thor::Animator<sf::Sprite, std::string>& explosionAnim, std::string animation)
{
    if(m_enemyAmmo.size() > 0)
    {
        std::list<nasic::ammo>::iterator screenAmmoIt;//check to see if enemy bullet left the screen...
        for(screenAmmoIt = m_enemyAmmo.begin(); screenAmmoIt != m_enemyAmmo.end(); ++screenAmmoIt)
        {
            if(screenAmmoIt->getPosition().y > window.getSize().y)
            {
                screenAmmoIt = m_enemyAmmo.erase(screenAmmoIt);//delete missed shots...

                return hitList::miss;
            }
        }

        std::list<nasic::ammo>::iterator ammoIt;
        for(ammoIt = m_enemyAmmo.begin(); ammoIt != m_enemyAmmo.end(); ++ammoIt)
        {
            if(ammoIt->getPosition().y >= hero.getPosition().y
                    && ammoIt->getPosition().y <= hero.getPosition().y + scale*hero.getAABB().y
                    && ammoIt->getPosition().x >= hero.getPosition().x
                    && ammoIt->getPosition().x <= hero.getPosition().x + scale*hero.getAABB().x)
            {
                //set the explosion position to the enemy location
                explosion.setPosition(ammoIt->getPosition().x, ammoIt->getPosition().y);

                //set the explosion sound buffer and play it
                explode.setBuffer(buffer);
                explode.play();

                //play the explosion animation
                explosionAnim.playAnimation(animation, false);

                ammoIt = m_enemyAmmo.erase(ammoIt);
                switch(ammoIt->getType())
                {
                case nasic::ammo::ammoType::agravu:
                {
                    return hitList::agravu;
                }
                break;

                case nasic::ammo::ammoType::delsiriak:
                {
                    return hitList::delsiriak;
                }
                break;

                case nasic::ammo::ammoType::gluorn:
                {
                    return hitList::gluorn;
                }
                break;

                case nasic::ammo::ammoType::rhiians:
                {
                    return hitList::rhiians;
                }
                break;

                default:
                    break;
                }
            }
            else
                return hitList::miss;
        }
    }
}

void nasic::enemyWave::purgeEnemies()
{
    m_enemies.clear();
    m_enemyAmmo.clear();
}

void nasic::enemyWave::animate(sf::Time dt)
{
    std::list<nasic::enemy>::iterator enemIt;
    for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
    {
        enemIt->animate(dt);
    }
}

void nasic::enemyWave::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states = getTransform();

    std::list<nasic::enemy>::const_iterator enemIt;
    for(enemIt = m_enemies.begin(); enemIt != m_enemies.end(); ++enemIt)
    {
        target.draw(*enemIt, states);
    }

    //draw enemy ammo
    std::list<nasic::ammo>::const_iterator eAmmoIt;
    for(eAmmoIt = m_enemyAmmo.begin(); eAmmoIt != m_enemyAmmo.end(); ++eAmmoIt)
    {
        target.draw(*eAmmoIt, states);
    }
}
