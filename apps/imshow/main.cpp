#include <OpenGP/Eigen/Image.h>
using namespace OpenGP;

int main_show_gray(){
    EigenImage<float> I(480, 640);
    for (int row = 0; row < I.rows(); row++)
        for (int col = 0; col < I.cols(); col++)
            I(row,col) = .5 + .5*std::sin( 2/*period*/*2*3.14*row/I.rows() );
    
    OpenGP::imshow(I); 
    return 0;
}

int main_test_color(){
    /// Part 1: create image on disk
    {
        EigenImage<Vec3> I(480, 640);
        for (int row = 0; row < I.rows(); row++){
            for (int col = 0; col < I.cols(); col++){
                Scalar r = (float) row / I.rows();
                Scalar g = (float) col / I.cols();
                I(row, col) = Vec3(r,0,0);
            }
        }
        
        /// Test: save + read + show
        imwrite("test.tga", I);
    }   
    
    /// Part 2: load image and show it
    {
        EigenImage<Vec3> I_new;
        imread("test.tga", I_new);
        imshow(I_new);    
    }
   
    return 0;
}

int main_read_color(){
    EigenImage<Vec3> I;
    imread("world_day.tga", I);
    imshow(I);
    return 0;
}

int main(int, char**){
    /// note: multi-window not supported, can only run one of the three below!
//    main_show_gray();
    main_test_color();
//    main_read_color();
    return EXIT_SUCCESS;
}

