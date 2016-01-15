#pragma once
#include <OpenGP/GL/glfw.h>
#include <OpenGP/MLogger.h>

//== NAMESPACE ================================================================
namespace OpenGP {
//=============================================================================

class GlfwFpsCounter{
/// @{
private:
    float _tMark = glfwGetTime();
    float _tLast = _tMark;
    int frame_count=0;
    int _fps = 0;
public:
    bool is_printed = false; ///< print to screen?
    float eval_fps_every=1; ///< seconds
/// @}

public:
    int fps(){ return _fps; }
    void tick(){
        frame_count++;
        float time = glfwGetTime();
        // float _tDelta = time - _tLast;
        _tLast = time;
        if (time - _tMark > eval_fps_every) {
            _tMark = time;
            _fps = frame_count / eval_fps_every;
            frame_count = 0;
            if(is_printed)
                mLogger() << "FPS: " << std::to_string(_fps);
        }
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
