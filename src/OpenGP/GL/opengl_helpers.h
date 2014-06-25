#pragma once
namespace opengl{
    inline bool is_depth_test_enabled(){
        GLboolean val = 0;
        glGetBooleanv(GL_DEPTH_TEST,&val);
        return val;        
    }
}