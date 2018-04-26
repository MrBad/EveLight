#include "input_manager.h"

InputManager::InputManager()
{
    mQuit = false;
}

void InputManager::SetKey(uint32_t key)
{
    mKeys.insert(pair<int, bool>(key, true));
}

void InputManager::ResetKey(uint32_t key)
{
    mKeys.erase(key);
}

// TODO: Handle mouse events
bool InputManager::Update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            mQuit = true;
        if (e.type == SDL_KEYDOWN)
            SetKey(e.key.keysym.sym);
        else if (e.type == SDL_KEYUP)
            ResetKey(e.key.keysym.sym);
    }

    return true;
}

bool InputManager::isKeyPressed(uint32_t key)
{
    return mKeys.find(key) != mKeys.end();
}

bool InputManager::isQuitRequested()
{
    return mQuit;
}
