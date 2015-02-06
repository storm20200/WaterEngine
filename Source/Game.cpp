#include "Game.h"


// STL headers.
#include <exception>
#include <iostream>


// Engine headers.
//#include <Entities/MilestoneEntity.h>
#include <Maths/Vector2D.h>
#include <Maths/Vector3D.h>
#include <Rendering/Renderer2DHAPI.h>
#include <Utility/Maths.h>
#include <Utility/RNG.h>
#include <Utility/Time.h>


// Milestone related constants.
const auto  backgroundLocation  = "background.tga", //!< The file location for the background image.
            circleLocation      = "alphaThing.tga", //!< The file location for the circle image.
            explosionLocation   = "explosion.png";  //!< The file location for the explosion spritesheet.

const auto  circleSpeed         = 150.f;            //!< The number of pixels a second the circle can travel.



#pragma region Engine functionality


bool Game::initialise()
{
    try 
    {
        // Initialise HAPI.
        if (!HAPI->Initialise (&m_screenWidth, &m_screenHeight))
        {
            throw std::runtime_error ("Game::initialise(): HAPI couldn't initialise.");
        }

        // Set up the rendering engine.
        m_pRenderer = std::make_shared<Renderer2DHAPI>();
        m_pRenderer->initialise (m_screenWidth, m_screenHeight);

        // Load the textures.
        TextureID ids[3] = {    m_pRenderer->loadTexture (backgroundLocation, { 0, 0 }),
                                m_pRenderer->loadTexture (circleLocation, { 0, 0 }),
                                m_pRenderer->loadTexture (explosionLocation, { 5, 5 }) };

        // Make the entities.
        /*auto background = std::make_unique<MilestoneEntity>(), circle = std::make_unique<MilestoneEntity>();

        background->getPosition() = { 0.f, 0.f };
        background->setTextureID (ids[0]);
        background->setBlendType (BlendType::Opaque);
        background->setFrameSize ({ 0, 0 });
        background->setFrame ({ 0, 0 });
        
        // Centre the circle.
        circle->getPosition() = {   m_screenWidth / 2.f - 32,
                                    m_screenHeight / 2.f - 32 };

        circle->setTextureID (ids[1]);
        circle->setBlendType (BlendType::Transparent);
        circle->setFrameSize ({ 0, 0 });
        circle->setFrame ({ 0, 0 });
        
        m_entities.push_back (std::move (background));
        m_entities.push_back (std::move (circle));
        
        
        // Randomise explosions everywhere.
        util::RNG<float>        rngF    { util::getCurrentTime<unsigned int>() };
        util::RNG<unsigned int> rngU    { util::getCurrentTime<unsigned int>() };
                
        for (int i = 0; i < 10; ++i)
        {
            auto explosion = std::make_unique<MilestoneEntity>();

            explosion->getPosition() = { m_screenWidth * rngF() - 61, m_screenHeight * rngF() - 61 };
            explosion->setTextureID (ids[2]);
            explosion->setBlendType (BlendType::Transparent);
            explosion->setFrameSize ({ 5, 5 });
            explosion->setFrame ({ rngU() % 5, rngU() % 5 });

            m_entities.push_back (std::move (explosion));
        }

        m_centreZone    = { static_cast<int> (m_screenWidth / 2.f - m_screenWidth / 20.f), 
                            static_cast<int> (m_screenHeight / 2.f - m_screenHeight / 20.f), 
                            static_cast<int> (m_screenWidth / 2.f + m_screenWidth / 20.f), 
                            static_cast<int> (m_screenHeight / 2.f + m_screenHeight / 20.f) };
                            */
        return true;
    }

    // Initialisation failed so just exit.
    catch (std::exception& error)
    {
        std::cerr << "Exception caught in Game::initialise(): " << error.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "Unknown error caught in Game::initialise()." << std::endl;
    }
    return false;
}


void Game::run()
{
    if (initialise())
    {
        const float sixtyFPS = static_cast<float> (1.0 / 60.0);

        HAPI->SetShowFPS (true);

        // Update each system.
        while (HAPI->Update())
        {
            updateDeltaTime();

            // Check whether to perform the capped update.
            if (m_sixtyFPSDeltaTime >= sixtyFPS)
            {
                updateCapped();
                m_sixtyFPSDeltaTime = 0.f;
            }
            
            // Perform the uncapped update.
            updateMain();
            
            renderAll();
        }
    }
}


void Game::updateDeltaTime()
{
    // Update the time figures.
    m_oldTime = m_currentTime;
    m_currentTime = HAPI->GetTime();

    // Convert from milliseconds to seconds.
    m_deltaTime = (m_currentTime - m_oldTime) / 1000.f;

    m_sixtyFPSDeltaTime += m_deltaTime;
}


void Game::updateCapped()
{
    // Update keyboard data.
    HAPI->GetKeyboardData (&m_keyboard);
    
    // Update controller data.
    m_controllerOn = HAPI->GetControllerData (0, &m_controller);

    if (m_controllerOn)
    {
        /*const auto& circlePosition = m_entities[1]->getPosition();
        const Rectangle circleRect { static_cast<int> (circlePosition.x), 
                                     static_cast<int> (circlePosition.y), 
                                     static_cast<int> (circlePosition.x + 63), 
                                     static_cast<int> (circlePosition.y + 63) };

        if (circleRect.intersects (m_centreZone))
        {
            HAPI->SetControllerRumble (0, 10000, 10000);
        }

        else
        {
            HAPI->SetControllerRumble (0, 0, 0);
        }*/
    }
}


void Game::updateMain()
{
    /*auto& circlePosition = m_entities[1]->getPosition();

    // Handle keyboard input.
    if (m_keyboard.scanCode[HK_LEFT] || m_keyboard.scanCode['A'] || 
       (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        circlePosition.x -= circleSpeed * m_deltaTime;
    }

    if (m_keyboard.scanCode[HK_UP] || m_keyboard.scanCode['W'] || 
       (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        circlePosition.y -= circleSpeed * m_deltaTime;
    }

    if (m_keyboard.scanCode[HK_RIGHT] || m_keyboard.scanCode['D'] || 
       (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        circlePosition.x += circleSpeed * m_deltaTime;
    }

    if (m_keyboard.scanCode[HK_DOWN] || m_keyboard.scanCode['S'] || 
       (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        circlePosition.y += circleSpeed * m_deltaTime;
    }
    
    for (auto& entity : m_entities)
    {
        if (entity)
        {
            entity->update (m_deltaTime);
        }
    }*/
}


void Game::renderAll()
{    
    // Render images.
    m_pRenderer->clearToBlack();

    Vector2D<> point2D { };
    Vector3D<> point3D { };

    /*for (auto& entity : m_entities)
    {
        if (entity)
        {
            entity->render (m_pRenderer);
        }
    }*/
}


#pragma endregion Engine functionality