#pragma once

#include <vector>
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>

#include <OpenGP/GL/Eigen.h> 
#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>
#include <OpenGP/GL/SceneObject.h>
#include <OpenGP/GL/TrackballCamera.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SceneGraph{
public:   
    /// Camera for the 3D scene
    /// TODO: create superclass?
    TrackballCamera trackball_camera;
    
    /// Set of objects that needs to be rendered
    std::vector<SceneObject*> objects;

    /// Scene lights
    /// TODO: generate set
    Vec3 _light_dir = Vec3(0,0,1);
    
    void add(SceneObject& object){
        object.init();
        objects.push_back(&object);
    }
    
    void display(float time=0){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        const Mat4x4& view = trackball_camera.view_matrix();
        const Mat4x4& projection = trackball_camera.projection_matrix();

        for(SceneObject* obj: objects) {     
            obj->program.bind();
            {
                ///--- Upload Light Specs TODO
                // obj->program.setUniformValue("LDIR", _light_dir);
                
                ///--- Upload time information
                obj->program.set_uniform("time", time);
                
                ///--- Update Matrix Stack
                obj->program.set_uniform("M",   Mat4x4(obj->model));
                obj->program.set_uniform("MV",  Mat4x4(view*obj->model));
                obj->program.set_uniform("MVP", Mat4x4(projection*view*obj->model));
                
                ///--- Display
                obj->display();
            }
            obj->program.release();            
        }
    }

    void screen_resize(int width, int height) {
        glViewport(0, 0, width, height);
        trackball_camera.screen_resize(width, height);
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
