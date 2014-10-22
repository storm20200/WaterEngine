// Engine headers
#include <Interfaces/IEntity.h>
#include <Helper/RNG.h>
#include <Helper/Time.h>
#include <ScreenManager/ScreenManager.h>



int screenWidth = 256, screenHeight = 256;
DWORD oldTime = 0, currentTime = 0;


/// <summary> Should only be called once a frame, returns the delta time as a float </summary>
const float calcDeltaTime()
{
    // Update the time figures.
    oldTime = currentTime;
    currentTime = HAPI->GetTime();

    // Convert from milliseconds to seconds.
    return (currentTime - oldTime) / 1000.f;
}


void HAPI_Main()
{
    /*const unsigned int screenSize = (unsigned int) screenWidth * (unsigned int) screenHeight;

    if (HAPI->Initialise (&screenWidth, &screenHeight))
    {
        // Set up the screen
        auto screenManager = std::make_unique<ScreenManager> (ScreenManager (screenWidth, screenHeight));
        HAPI->SetShowFPS (true);


        // Create RNGs
        const unsigned int seed = helper::getCurrentTimeUInt();

        auto floatRNG   = RNG<float>        (seed);
        auto uintRNG    = RNG<unsigned int> (seed);


        // Set up the stars
        const int starsSize = 1000;
        std::vector<Star> stars (starsSize);

        for (unsigned int i = 0; i < starsSize; ++i)
        {
            // Ensure the correct direction and randomise each attribute of the star
            Star star { };
            star.setDirection ( {0.f, 0.f, -1.f } );

            resetStar (star, floatRNG, uintRNG);

            stars[i] = star;
        }

        
        // Move and draw the stars
        while (HAPI->Update())
        {
            // Clear the screen
            screenManager->clearToBlack();

            for (auto& star : stars)
            {
                // Move the star
                star.update();

                // Ensure position is valid so that it doesn't need resetting.
                const auto& position = star.getPosition();

                if (position.x >= screenWidth || position.x < 0.f ||
                    position.y >= screenHeight || position.y < 0.f ||
                    position.z < 0.f)
                {
                    resetStar (star, floatRNG, uintRNG);
                }
                
                //screenManager->setPixel (star.getDisplayData (screenWidth, screenHeight));
            }
        }
    }*/
}