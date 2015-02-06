#include "Texture.h"


// STL headers.
#include <exception>


// Engine headers.
#include <HAPI_lib.h>


// Constants used throughout.
const auto sizeOfColour = sizeof (HAPI_TColour);



#pragma region Constructors and destructor

Texture::Texture (const std::string& fileLocation)
{   
    // Attempt to load the texture during construction.
    loadTexture (fileLocation);
}


Texture::Texture (const std::string& fileLocation, const Point& frameDimensions)
{
    // Attempt to load the texture during construction.
    loadTexture (fileLocation);

    // Attempt to set the frame dimensions.
    setFrameDimensions (frameDimensions);
}


Texture::Texture (Texture&& move)
{
    // Just use the operator implementation.
    *this = std::move (move);
}


Texture& Texture::operator= (Texture&& move)
{
    if (this != &move)
    {
        // Obtain all of the data from the movee.
        m_frames = move.m_frames;

        m_frameDimensions = std::move (move.m_frameDimensions);
        m_textureSpace = std::move (move.m_textureSpace);
        m_pData = move.m_pData;


        // Reset the movee.
        move.m_frames = 0;
        move.m_pData = nullptr;
    }

    return *this;
}


Texture::~Texture()
{
    if (m_pData)
    {
        delete[] m_pData;
        m_pData = nullptr;
    }
}

#pragma endregion


#pragma region Getters and setters

Texture& Texture::resetFrameDimensions()
{
    m_frames = 0;
    m_frameDimensions.x = 0;
    m_frameDimensions.y = 0;

    return *this;
}


Texture& Texture::setFrameDimensions (const Point& dimensions)
{
    if (dimensions.x == 0 || dimensions.y == 0)
    {
        // Disable spritesheet functionality.
        resetFrameDimensions();
    }

    else
    {
        // Recalculate frame values.
        m_frames            = dimensions.x * dimensions.y;
        m_frameDimensions.x = dimensions.x;
        m_frameDimensions.y = dimensions.y;
    }

    return *this;
}

#pragma endregion


#pragma region Loading functionality

void Texture::loadTexture (const std::string& fileLocation)
{
    // Create variable cache for interfacing with HAPI.
    int width = 0, height = 0;

    // Attempt to load the data. If the loading succeeds then the width and height will be set.
    if (HAPI->LoadTexture (fileLocation, &m_pData, &width, &height))
    {
        resetFrameDimensions();
        
        // Initialise the texture space.
        m_textureSpace = { 0, 0, width - 1, height - 1 };
    }
    
    else
    {   
        throw std::runtime_error ("Texture::loadTexture(): Unable to initialise Texture with file \"" + fileLocation + "\"");
    }
}

#pragma endregion


#pragma region Rendering

void Texture::blit (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const BlendType blend, const Point& frame)
{
    // Pre-conditions: target is a valid pointer.
    if (!target)
    {
        throw std::invalid_argument ("Texture::blit(): Invalid pointer to the target was given.");
    }

    // Pre-condition: frame is valid.
    if (m_frames != 0 && 
       (frame.x >= m_frameDimensions.x || frame.x < 0 ||
        frame.y >= m_frameDimensions.y || frame.y < 0))
    {
        throw std::invalid_argument ("Texture::blit(): Invalid frame number given (" + 
                                     std::to_string (frame.x) + ", " + std::to_string (frame.y) + "). " +

                                     "Texture frame count is (" + 
                                     std::to_string (m_frameDimensions.x) + ", " + std::to_string (m_frameDimensions.y) + ").");
    }

    const auto  textureWidth = m_textureSpace.width(), 
                textureHeight = m_textureSpace.height();

    // We need to check if the texture will actually need to be drawn.
    Rectangle<int>  texture     {   point.x,
                                    point.y,
                                    point.x + textureWidth - 1,
                                    point.y + textureHeight - 1 };

    // Determine the frame offset and check if the texture size needs changing.
    Point frameOffset { 0, 0 };

    // If m_frames is zero it's a single texture and therefore do not need to calculate offsets.
    if (m_frames != 0)
    {
        const unsigned int  frameWidth = textureWidth / m_frameDimensions.x,
                            frameHeight = textureHeight / m_frameDimensions.y;

        // Update the texture co-ordinates.
        texture.setRight (point.x + frameWidth - 1);
        texture.setBottom (point.y + frameHeight - 1);

        // Calculate how much we need to translate by.
        frameOffset.x = frame.x * frameWidth, 0;
        frameOffset.y = frame.y * frameHeight, 0;
    }
    
    // We will only draw if the texture is out-of-bounds.
    if (targetSpace.intersects (texture))
    {
        // Check if any clipping is necessary.
        if (!targetSpace.contains (texture))
        {
            // Clip the rectangle.
            texture.clipTo (targetSpace);
        }
            
        // Translate back to texture space, ready for blitting.
        texture.translate (-point.x, -point.y);

        // Call the correct blitting function.
        switch (blend)
        {
            case BlendType::Opaque:
                blitOpaque (target, targetSpace, point, frameOffset, texture);
                break;

            case BlendType::Transparent:
                blitTransparent (target, targetSpace, point, frameOffset, texture);
                break;
        }
    }

    // Do nothing if it doesn't intersect.
}


void Texture::blit (Texture& target, const Point& point, const BlendType blend, const Point& frame)
{
    // Forward onto the normal blitting function.
    blit (target.m_pData, target.m_textureSpace, point, blend, frame);
}


void Texture::blitOpaque (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
{    
    // Cache zee variables captain!
    const auto  blitWidth       = (size_t) drawArea.width(),
                blitHeight      = (size_t) drawArea.height();
               
    const auto  dataWidth       = blitWidth * sizeOfColour,
                targetWidth     = targetSpace.width() * sizeOfColour,
                textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                targetOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * targetWidth;
    
    // Obtain the data from the texture.
    const auto  textureData     = m_pData + dataOffset;
    
    // Calculate the starting pointer to the target position.
    BYTE* targetLine = target + targetOffset;    
    BYTE* currentLine = nullptr;

    for (size_t y = 0; y < blitHeight; ++y)
    {
        // Increment the pointer and copy line-by-line.
        currentLine = targetLine + y * targetWidth;
        std::memcpy (currentLine, (textureData + y * textureWidth), dataWidth);
    }
}


void Texture::blitTransparent (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
{
    // Cache zee variables captain!
    const auto  blitWidth       = (size_t) drawArea.width(),
                blitHeight      = (size_t) drawArea.height(),
               
                dataWidth       = blitWidth * sizeOfColour,
                targetWidth     = targetSpace.width() * sizeOfColour,
                textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                targetOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * targetWidth;

    // Create the required pointers for the blitting process.
    auto        currentData     = m_pData + dataOffset;
    auto        currentPixel    = target + targetOffset;

    // Avoid those magic constants!
    const auto  alphaIndex = 3U;

    for (size_t y = 0; y < blitHeight; ++y)
    {
        for (size_t x = 0; x < blitWidth; ++x)
        {
            const auto alpha = currentData[alphaIndex];

            // Avoid unnecessary blending when alpha is 0 or 255.
            switch (alpha)
            {
                case 0:
                    break;

                case 255:
                    for (auto i = 0U; i < alphaIndex; ++i)
                    {
                        currentPixel[i] = currentData[i];
                    }

                    break;

                default:                     
                    for (auto i = 0U; i < alphaIndex; ++i)
                    {
                        // Avoid floating-point arithmetic by bit-shifting.
                        const auto currentChannel = currentPixel[i];
                        currentPixel[i] = currentChannel + ((alpha * (currentData[i] - currentChannel)) >> 8);
                    }

                    break;
            }
            
            // Increment each pointer.
            currentPixel += sizeOfColour;
            currentData += sizeOfColour;
        }

        // Since the width is done we must go onto the next line.
        currentPixel += targetWidth - dataWidth;
        currentData += textureWidth - dataWidth;
    }
}

#pragma endregion