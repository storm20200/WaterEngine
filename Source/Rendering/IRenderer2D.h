#if !defined INTERFACE_RENDERER_INCLUDED
#define INTERFACE_RENDERER_INCLUDED


// STL headers.
#include <string>


// Forward declarations.
template <typename T> struct Vector2D;
using Point = Vector2D<int>;
using TextureID = size_t;



/// <summary> Represents how an image should be blended when blit using the ScreenManager. </summary>
enum class BlendType : int
{
    Opaque = 0,         //!< Enables fast blitting by disregarding alpha values.
    Transparent = 1     //!< Enables alpha blending when blitting, slower but allows for transparency.
};



/// <summary> An interface for 2D rendering systems, used for platform-independant drawing functionality. </summary>
class IRenderer2D
{
    public:

        // Ensure destructor is virtual since this is an interface.
        virtual ~IRenderer2D() {}
    
        /// <summary> Initialise all data and prepare for rendering. </summary>
        virtual void initialise (const int screenWidth, const int screenHeight) = 0;

        /// <summary> Causes all loaded texture data to be deleted, invalidating all current keys. </summary>
        virtual void clearTextureData() = 0;

        /// <summary>
        /// Loads a texture from local storage, ready for rendering. 
        /// </summary>
        /// <returns> Returns the ID for the loaded texture. </returns>
        virtual TextureID loadTexture (const std::string& fileLocation, const Point& frameDimensions) = 0;
        
        /// <summary> Clears the screen to a black level between 0 and 1, quicker than clearing to a colour. </summary>
        virtual void clearToBlack (const float blackLevel = 0) = 0;        

        /// <summary> Clears the entire screen to a single colour. </summary>
        virtual void clearToColour (const float red, const float green, const float blue, const float alpha = 1.f) = 0;

        /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        virtual void drawToScreen (const Point& point, const TextureID id, const BlendType blend) = 0;

        /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
        virtual void drawToScreen (const Point& point, const TextureID id, const BlendType blend, const Point& frame) = 0;

        /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
        /// <param name="point"> The target top-left point on the texture to draw onto. </param>
        /// <param name="source"> The source texture to draw. </param>
        /// <param name="target"> The target texture to draw onto. </param>
        /// <param name="blend"> The type of alpha blending to perform. </param>
        virtual void drawToTexture (const Point& point, const TextureID source, const TextureID target, const BlendType blend) = 0;

        /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
        /// <param name="point"> The target top-left point on the texture to draw onto. </param>
        /// <param name="source"> The source texture to draw. </param>
        /// <param name="target"> The target texture to draw onto. </param>
        /// <param name="blend"> The type of alpha blending to perform. </param>
        /// <param name="frame"> The frame of the source texture to draw. </param>
        virtual void drawToTexture (const Point& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame) = 0;
};


#endif // INTERFACE_RENDERER_INCLUDED