#if !defined WATER_INTERFACE_RENDERER_ENGINE_INCLUDED
#define WATER_INTERFACE_RENDERER_ENGINE_INCLUDED


// Engine headers.
#include <IRenderer.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to IRenderer systems.
    /// </summary>
    class IRendererEngine : public IRenderer
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IRendererEngine() {}


            #pragma region System management
    
            /// <summary> Initialise all data and prepare for rendering. </summary>
            /// <param name="screenWidth"> The width of the screen resolution. </param>
            /// <param name="screenHeight"> The height of the screen resolution. </param>
            /// <param name="unitToPixelScale"> How much to scale the position values during the rendering process. </param>
            virtual void initialise (const int screenWidth, const int screenHeight, const Vector2<float>& unitToPixelScale) = 0;

            /// <summary> Updates the IRenderer system. </summary>
            virtual void update() = 0;

            #pragma endregion
    };   
}

#endif