#include "EigenImage.h"
#include <stdio.h>
using namespace OpenGP;

void main_color(){
    struct Vec3b{ 
        uchar rgb[3]; 
        operator Vec3() const{return Vec3(rgb[2]/255.0,rgb[1]/255.0,rgb[0]/255.0);} 
    };
    // typedef Eigen::Vector3f vec3;
    EigenImage<Vec3b> I(240, 320);
    for (int row = 0; row < I.rows(); row++){
        for (int col = 0; col < I.cols(); col++){
            uchar r = 255 * (row / (float) I.rows());
            uchar g = 255 * (col / (float) I.cols());
            // Colors are swapped
            I(row, col) = {0,g,r}; //vec3(r,g,0);
        }
    }
    
    // http://geekchef.com/using-targa-files
    // http://stackoverflow.com/questions/16538945/writing-uncompressed-tga-to-file-in-c
    // https://github.com/nlguillemot/JustGL/blob/f8566ed540413e7b0503a6a08396df7363891a62/ext/justgl_image.cpp#L30
    // http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001/
    {
        int _frameWidth = I.cols();
        int _frameHeight = I.rows();
        FILE* fid = fopen("test.tga","wb");
        GLubyte header[18]={0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        header[12] = _frameWidth          & 0xFF;
        header[13] = ( _frameWidth >> 8)  & 0xFF;
        header[14] = (_frameHeight)       & 0xFF; 
        header[15] = (_frameHeight >> 8)  & 0xFF;
        header[16] = 24;
        fwrite(header,sizeof(GLubyte),18,fid);
        fwrite(I.data(),sizeof(GLubyte), _frameWidth * _frameHeight * 3 ,fid);
        fclose(fid);
    }
    
    {
        FILE* fid = fopen("test.tga","rb");
        GLubyte header[18];
        if( fread(header, sizeof(header), 1, fid) != 1 ) 
            mFatal() << "Cannot read TGA header";
        int width = (header[13] << 8) + header[12];
        int height = (header[15] << 8) + header[14];        
        /// TODO test sizes
        if(header[16]!=24)
            mFatal() << "Only 24bits TGA supported";
        int channels = header[16] / 8;
        mDebug("%d x %d @ %d", width, height, channels);
        typedef Eigen::Matrix<Vec3b, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Image;
        Image I(height, width);
        if( fread(I.data(), channels * width * height, 1, fid) != 1 )
            mFatal() << "Failed to read TGA data";
        EigenImage<Vec3> I2 = I.cast<Vec3>();
        OpenGP::imshow(I2);    
    }
    
    //OpenGP::imshow(I);    
    exit(EXIT_SUCCESS);
}

void main_gray(){
    EigenImage<float> I(480, 640);
    for (int row = 0; row < I.rows(); row++)
        for (int col = 0; col < I.cols(); col++)
            I(row,col) = .5 + .5*std::sin( 2/*period*/*2*3.14*row/I.rows() );
    OpenGP::imshow(I); 
    exit(EXIT_SUCCESS);
}

int main(int, char**){
    main_color();
    // main_gray();
    return EXIT_SUCCESS;
}

