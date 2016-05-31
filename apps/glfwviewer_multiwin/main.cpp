#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>
using namespace OpenGP;

struct GlfwWindowOne : public GlfwWindow{
    GlfwWindowOne(){ glClearColor(1.0f, 0.0f, 0.0f, 0.0f); }
    bool mouse_press_callback(int button, int event, int) override { 
        mLogger() << "[GlfwWindowOne] mouse" << button << event;
        return true;
    }
};

struct GlfwWindowTwo : public GlfwWindow{
    GlfwWindowTwo(){ glClearColor(0.0f, 1.0f, 0.0f, 0.0f); }
    bool mouse_press_callback(int button, int event, int) override { 
        mLogger() << "[GlfwWindowTwo] mouse" << button << event; 
        return true;
    }
};

int main(int, char**){   
    GlfwWindowOne win1;
    GlfwWindowTwo win2;
    return GlfwWindow::run();
}


