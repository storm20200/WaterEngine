#if !defined WATER_INTERFACE_GAME_WORLD_INCLUDED
#define WATER_INTERFACE_GAME_WORLD_INCLUDED


// Engine namespace.
namespace water
{
    // Forward declarations.
    class GameState;


    /// <summary>
    /// A basic interface to the world model of the engine. The world model manages the state system and gives the user
    /// control over which state is active and which states exist.
    /// </summary>
    class IGameWorld
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IGameWorld() { }


            #pragma region State management

            /// <summary> Pops all states, removes all states and then exits the game. </summary>
            virtual void exit() = 0;

            /// <summary> Adds a game state to the world, this allows it to be accessible.</summary>
            /// <param name="id"> The unique identifier of the state. If a state with this ID already exists it will be ignored. </param>
            /// <param name="state"> The state to add. </param>
            /// <returns> True if successful, false if the ID is already in use or the state doesn't exist. </returns>
            virtual bool addState (const int id, GameState* const state) = 0;

            /// <summary> Removes a state from the game world, this will cause it to be inaccessible from now on. </summary>
            /// <param name="id"> The unique identifier of the state to remove. </param>
            /// <returns> If the removal was successful. If the ID is invalid this will return false. </returns>
            virtual bool removeState (const int id) = 0;

            /// <summary> Request a state to be pushed onto the top of the state stack, therefore becoming the active state. </summary>
            /// <param name="id"> The unique identifier of the state to push. </param>
            /// <returns> Whether the action could be performed. </returns>
            virtual bool push (const int id) = 0;

            /// <summary> 
            /// Request the active state be popped from the top of the state stack. The previously push state will become active. 
            /// If this results in an empty state stack then the game will close.
            /// </summary>
            /// <param name="id"> The unique identifier of the state to pop. </param>
            virtual void pop (const int id) = 0;

            /// <summary> Swaps the active state with another, this pops the active state but doesn't reactivate the previous state. </summary>
            /// <param name="id"> The identifier of the state to swap to. </param>
            /// <returns> Whether the swap was successful. </returns>
            virtual bool swap (const int id) = 0;

            #pragma endregion
    };
}


#endif