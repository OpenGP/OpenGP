#include "mex.h"
#include "nnsearch.h"
#include "eigenmex.h"
#include <iostream>

using namespace Eigen;
using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    /// Prepare input
    if(nrhs!=3){
        mexErrMsgTxt("three error");
        return;
    }
    ConstMatrixWrap vertices = wrap(prhs[0]);
    ConstMatrixWrap faces = wrap(prhs[1]);
    ConstMatrixWrap queries = wrap(prhs[2]);

    /// Allocate output vectors
    plhs[0]=mxCreateDoubleMatrix(3,queries.cols(),mxREAL);
    plhs[1]=mxCreateDoubleMatrix(1,queries.cols(),mxREAL);
    plhs[2]=mxCreateDoubleMatrix(3,queries.cols(),mxREAL);
    MatrixWrap footpoints = wrap(plhs[0]);    
    MatrixWrap findex = wrap(plhs[1]);
    MatrixWrap barycentric = wrap(plhs[2]);
    
    /// Search
    TrimeshSearcher<MatrixWrap,MatrixWrap> searcher;
    searcher.build(vertices, faces);
    searcher.closest_point(queries, footpoints, findex);
    searcher.barycentric(footpoints, findex, barycentric);
}


