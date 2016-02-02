#pragma once

#include <vector>
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>

#include <OpenGP/GL/Eigen.h> 
#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>
#include <OpenGP/GL/SceneObject.h>
#include <OpenGP/GL/ArcballCamera.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SceneGraph{
public:
    Mat4x4 _projection;
    Mat4x4 _view;  
    Vec3 _light_dir = Vec3(0,0,1);
    ArcballCamera camera;
    
    ///--- Set of objects that needs to be rendered
    std::vector<SceneObject*> objects;

    SceneGraph(){
        /// Define projection matrix (FOV, aspect, near, far)
        _projection = OpenGP::perspective(45.0f, 4.0f/3.0f, 0.1f, 10.f);
        // cout << projection << endl;

        /// Define the view matrix (camera extrinsics)
        Vec3 cam_pos(0,0,2);
        Vec3 cam_look(0,0,-1); /// Remember: GL swaps viewdir
        Vec3 cam_up(0,1,0);
        _view = OpenGP::lookAt(cam_pos, cam_look, cam_up);
        // cout << view << endl;
    }

    void add(SceneObject& object){
        object.init();
        objects.push_back(&object);
    }
    
    void display(float time=0){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(SceneObject* obj: objects) {
            _view = camera.getCameraMatrix();
            obj->program.bind();
            {
                ///--- Upload Light Specs TODO
                // obj->program.setUniformValue("LDIR", _light_dir);
                
                ///--- Upload time information
                obj->program.set_uniform("time", time);
                
                ///--- Update Matrix Stack
                obj->program.set_uniform("M",   Mat4x4(obj->model));
                obj->program.set_uniform("MV",  Mat4x4(_view*obj->model));
                obj->program.set_uniform("MVP", Mat4x4(_projection*_view*obj->model));
                
                ///--- Display
                obj->display();
            }
            obj->program.release();            
        }
    }

    void screen_resize(int width, int height) {
        glViewport(0, 0, width, height);
        _projection = OpenGP::perspective(45.0f, (float)width / height, 0.1f, 10.f);
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
