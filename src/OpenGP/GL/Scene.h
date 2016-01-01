#pragma once

#include <vector>
#include <Eigen/Core>
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>

#include <OpenGP/GL/Eigen.h> 
#include <OpenGP/GL/glfw_helpers.h>
#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>

//=============================================================================
namespace opengp {
//=============================================================================

/// TODO rename into scenegraph
class Scene{
public:
    class Object{
    public:
        ShaderProgram program;
        /// Model Transformation Matrix
        Mat4x4 model = Mat4x4::Identity();

        virtual ~Object(){}
        virtual void init() = 0;
        virtual void display() = 0;
    };
    
public:
    /// TODO: remove
    typedef Eigen::Vector3f vec3;
    typedef Eigen::Matrix4f mat4;
    
    Eigen::Matrix4f _projection;
    Eigen::Matrix4f _view;  
    vec3 _light_dir = vec3(0,0,1);
    
    ///--- Set of objects that needs to be rendered
    std::vector<Object*> objects;

    Scene(){
        /// Define projection matrix (FOV, aspect, near, far)
        _projection = opengp::perspective(45.0f, 4.0f/3.0f, 0.1f, 10.f);
        // cout << projection << endl;

        /// Define the view matrix (camera extrinsics)
        vec3 cam_pos(0,0,2);
        vec3 cam_look(0,0,-1); /// Remember: GL swaps viewdir
        vec3 cam_up(0,1,0);
        _view = opengp::lookAt(cam_pos, cam_look, cam_up);
        // cout << view << endl;
    }

    void add(Object& object){
        object.init();
        objects.push_back(&object);
    }
    
    void display(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(Object* obj: objects) {
            // ///--- Update Light Specs
            // obj->program.setUniformValue("LDIR", _light_dir);
            // check_error_gl();
                    
            ///--- Update Matrix Stack
            obj->program.set_uniform("M",   Mat4x4(obj->model));
            obj->program.set_uniform("MV",  Mat4x4(_view*obj->model));
            obj->program.set_uniform("MVP", Mat4x4(_projection*_view*obj->model));
            
            ///--- Display
            obj->display();
        }
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
