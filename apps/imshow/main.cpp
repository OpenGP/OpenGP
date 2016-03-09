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
    EigenImage<Vec3> I(200, 600);
    for (int row = 0; row < I.rows(); row++){
        for (int col = 0; col < I.cols(); col++){
            Scalar r = (float) row / I.rows();
            Scalar g = (float) col / I.cols();
            I(row, col) = Vec3(r,0,0);
        }
    }
    
    /// Test: save + read + show
    imwrite("test.tga", I);   
    EigenImage<Vec3> I_new;
    imread("world_day.tga", I_new);
    imshow(I_new);    
   
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
//    return main_show_gray();
//    return main_test_color();
    return main_read_color();
}

