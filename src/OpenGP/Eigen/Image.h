#pragma once

#include <Eigen/Core>
#include <OpenGP/GL/glfw.h>
#include <OpenGP/GL/SceneObject.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <stdio.h> //< to read targa file

//=============================================================================
namespace OpenGP{
//=============================================================================

template <class PixelType>
using EigenImage = Eigen::Matrix< PixelType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor >;

template <class PixelType>
class ImageRenderer : public SceneObject{
protected:
    EigenImage<PixelType>& _image;
    VertexArrayObject _vao;  
    ArrayBuffer<Vec3> _buffer_vpos; ///< per-vertex position
    ArrayBuffer<Vec2> _buffer_vtex; ///< per-vertex uv coords
    GLuint _tex; ///< Texture ID

public:
    ImageRenderer(EigenImage<PixelType>& image) : _image(image){}
    ~ImageRenderer(){
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &_tex);
    }

public: 
    ///--- Generic vertex shader
    const GLchar* vshader = R"GLSL(
        #version 330 core
        in vec3 vpoint;
        in vec2 vtexcoord;
        out vec2 uv;
        void main() { 
            uv = vtexcoord;
            gl_Position = vec4(vpoint, 1.0);
        }
    )GLSL";
    
    ///--- Fragment shader for RGB textures
    const GLchar* fshader_rgb = R"GLSL(
        #version 330 core
        uniform sampler2D tex;
        in vec2 uv;
        out vec4 color;
        void main() { color = vec4(texture(tex,uv).rgb,1); }
    )GLSL";

    ///--- Fragment shader for grayscale textures
    const GLchar* fshader_gray = R"GLSL(
        #version 330 core
        uniform sampler2D tex;
        in vec2 uv;
        out vec4 color;
        void main() { color = vec4(vec3(texture(tex,uv).r),1); }
    )GLSL";          

public:
    Box3 bounding_box(){ return Box3( Vec3(-1,-1,0), Vec3(+1,+1,0) ); }    
    void init(){
        ///--- Pick the right shader    
        const GLchar* fshader = NULL;
        fshader = (std::is_same<PixelType, Eigen::Vector3f >::value) ? fshader_rgb  : fshader;
        fshader = (std::is_same<PixelType, float >::value) ? fshader_gray : fshader;  
        assert(fshader!=NULL);
        
        ///--- Compile program
        program.add_vshader_from_source(vshader);
        program.add_fshader_from_source(fshader);
        program.link();
        
        ///--- Quad coordinates
        const GLfloat vpoint[] = { /*V0*/ -1.0f, -1.0f, 0.0f, 
                                   /*V1*/ +1.0f, -1.0f, 0.0f, 
                                   /*V2*/ -1.0f, +1.0f, 0.0f,
                                   /*V3*/ +1.0f, +1.0f, 0.0f }; 
        _buffer_vpos.upload_raw(vpoint, 4);
        
        ///--- Texture coordinates
        const GLfloat vtexcoord[] = { /*v0*/ 0.0f, 0.0f, 
                                      /*v1*/ 1.0f, 0.0f,
                                      /*v2*/ 0.0f, 1.0f, 
                                      /*v3*/ 1.0f, 1.0f };
        _buffer_vtex.upload_raw(vtexcoord, 4);
                
        ///--- Load texture
        {
            glGenTextures(1, &_tex);
            glBindTexture(GL_TEXTURE_2D, _tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            if ( std::is_same<PixelType, Eigen::Vector3f>::value ){
                glTexImage2D(GL_TEXTURE_2D, /*level*/ 0, GL_RGB32F, 
                             _image.cols(), _image.rows(), 0, 
                             GL_RGB, GL_FLOAT, _image.data());
            }
            else if( std::is_same<PixelType, float>::value ){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 
                             _image.cols(), _image.rows(), 0, 
                             GL_RED, GL_FLOAT, _image.data());
            }
        }        
        
        ///--- Specify attributes
        program.bind();
        _vao.bind();
            program.set_attribute("vpoint", _buffer_vpos);  
            program.set_attribute("vtexcoord", _buffer_vtex);
            glUniform1i(glGetUniformLocation(program.programId(), "tex"), 0 /*GL_TEXTURE0*/);            
        _vao.release();
        program.release();
    }
    void display(){
        _vao.bind();
        program.bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _tex);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        program.release();
        _vao.release();
    }
    
};

template <class PixelType>
inline int imshow( EigenImage<PixelType>& I, const char* title="" ){
    GlfwWindow window(title, I.cols(), I.rows());
    ImageRenderer<PixelType> renderer(I);
    window.scene.add(renderer); 
    return window.run();
}

// http://geekchef.com/using-targa-files
// http://stackoverflow.com/questions/16538945/writing-uncompressed-tga-to-file-in-c
// https://github.com/nlguillemot/JustGL/blob/f8566ed540413e7b0503a6a08396df7363891a62/ext/justgl_image.cpp#L30
// http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001
/// A pixel is represented as a 3-channel [0,256] tuple.
struct TargaVec3b{ 
    uchar rgb[3]; 
    operator Vec3() const{return Vec3(rgb[2]/255.0,rgb[1]/255.0,rgb[0]/255.0);} 
};

/// Targa 24bits (3 channels) internal image representation    
typedef Eigen::Matrix<TargaVec3b, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> TargaImage;
    
static void imwrite(const char* path, const EigenImage<Vec3>& _I){
    TargaImage I(_I.rows(), _I.cols());
    for (int row = 0; row < I.rows(); row++){
        for (int col = 0; col < I.cols(); col++){
            uchar r = 255 * _I(row,col)[0];
            uchar g = 255 * _I(row,col)[1];
            uchar b = 255 * _I(row,col)[2];
            // Note: R<==>B colors are swapped in file format
            I(row,col) = {b, g, r};
        }
    }
    
    FILE* fid = fopen(path,"wb");
    GLubyte header[18]={0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    header[12] = (I.cols())       & 0xFF;
    header[13] = (I.cols() >> 8)  & 0xFF;
    header[14] = (I.rows())       & 0xFF; 
    header[15] = (I.rows() >> 8)  & 0xFF;
    header[16] = 24;
    fwrite(header,sizeof(GLubyte),18,fid);
    fwrite(I.data(),sizeof(GLubyte), I.rows() * I.cols() * 3 ,fid);
    fclose(fid);
}

static void imread(const char* path, EigenImage<Vec3>& I_out){
    /// TODO: TGA images are stored on a per row basis. *However* image can be flipped vertically. 
    /// Its actually an option in the image format. You can encode images both from top to bottom 
    /// or from bottom to top... And even though most programs seem to output TGA files in bottom 
    /// to top encoding, your loader should still make sure to support both
    FILE* fid = fopen(path,"rb");
        GLubyte header[18];
        if( fread(header, sizeof(header), 1, fid) != 1 ) 
            mFatal() << "Cannot read TGA header";
        int width  = (header[13] << 8) + header[12];
        int height = (header[15] << 8) + header[14];
        if(header[16]!=24)
            mFatal() << "Only 24bits TGA supported";
        int channels = header[16] / 8;
        // mDebug("%d x %d @ %d", width, height, channels);
        TargaImage I(height, width);
        if( fread(I.data(), channels * width * height, 1, fid) != 1 )
            mFatal() << "Failed to read TGA data";
    fclose(fid);   
    I_out = I.cast<Vec3>();
}

//=============================================================================
} // OpenGP::
//=============================================================================
