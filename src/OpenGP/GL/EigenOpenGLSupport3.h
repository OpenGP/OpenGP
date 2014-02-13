// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2014 Andrea Tagliasacchi <andrea.tagliasacchi@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_OPENGL_MODULE
#define EIGEN_OPENGL_MODULE

/// HACK, as we are using GL3

#include <Eigen/Geometry>

#if defined(__APPLE_CC__)
 #if defined(EIGEN_INCLUDE_GL3)
  #include <OpenGL/gl3.h>
 #else
  #include <OpenGL/gl.h>
 #endif
#else
  #include <GL/gl.h>
#endif

#define EIGEN_INCLUDE_GL3

namespace Eigen {

/**
  * \defgroup OpenGLSUpport_Module OpenGL Support module
  *
  * This module provides wrapper functions for a couple of OpenGL functions
  * which simplify the way to pass Eigen's object to OpenGL. 
  * 
  * Below is a usage example for OpenGL 2.0:
  * 
  * \code
  * #include <Eigen/OpenGLSupport>
  * // ...
  * Vector3f x, y;
  * Matrix3f rot;
  * 
  * glVertex(y + x * rot);
  * 
  * Quaternion q;
  * glRotate(q);
  * 
  * // ...
  * \endcode
  *
  * And an example for shader-based OpenGL 3.0
  *
  * \code
  * // @note you need to add Eigen/unsupported to your include path.
  * #define EIGEN_INCLUDE_GL3
  * #include <Eigen/OpenGLSupport>
  * // ...
  * Matrix4f matrix = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  * GLuint matrix_id = glGetUniformLocation(programID, "matrix");
  * glUniform(matrix_id, matrix);
  * // ...
  * \endcode
  */
  
//@{

#define EIGEN_GL_FUNC_DECLARATION(FUNC)                                                                             \
namespace internal {                                                                                                \
  template< typename XprType,                                                                                       \
            typename Scalar = typename XprType::Scalar,                                                             \
            int Rows = XprType::RowsAtCompileTime,                                                                  \
            int Cols = XprType::ColsAtCompileTime,                                                                  \
            bool IsGLCompatible = bool(XprType::Flags&LinearAccessBit)                                              \
                              && bool(XprType::Flags&DirectAccessBit)                                               \
                              && (XprType::IsVectorAtCompileTime || (XprType::Flags&RowMajorBit)==0)>               \
  struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl);                                                                      \
                                                                                                                    \
  template<typename XprType, typename Scalar, int Rows, int Cols>                                                   \
  struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType,Scalar,Rows,Cols,false> {                                     \
    inline static void run(const XprType& p) {                                                                      \
      EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<typename plain_matrix_type_column_major<XprType>::type>::run(p); }       \
  };                                                                                                                \
}                                                                                                                   \
                                                                                                                    \
template<typename Derived> inline void FUNC(const Eigen::DenseBase<Derived>& p) {                                   \
  EIGEN_CAT(EIGEN_CAT(internal::gl_,FUNC),_impl)<Derived>::run(p.derived());                                        \
}


#define EIGEN_GL_FUNC_SPECIALIZATION_MAT(FUNC,SCALAR,ROWS,COLS,SUFFIX)                                              \
namespace internal {                                                                                                \
  template< typename XprType> struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType, SCALAR, ROWS, COLS, true> {      \
    inline static void run(const XprType& p) { FUNC##SUFFIX(p.data()); }                                            \
  };                                                                                                                \
}

  
#define EIGEN_GL_FUNC_SPECIALIZATION_VEC(FUNC,SCALAR,SIZE,SUFFIX)                                                   \
namespace internal {                                                                                                \
  template< typename XprType> struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType, SCALAR, SIZE, 1, true> {         \
    inline static void run(const XprType& p) { FUNC##SUFFIX(p.data()); }                                            \
  };                                                                                                                \
  template< typename XprType> struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType, SCALAR, 1, SIZE, true> {         \
    inline static void run(const XprType& p) { FUNC##SUFFIX(p.data()); }                                            \
  };                                                                                                                \
}

  
EIGEN_GL_FUNC_DECLARATION       (glVertex)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,int,    2,2iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,short,  2,2sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,float,  2,2fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,double, 2,2dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,int,    3,3iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,short,  3,3sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,float,  3,3fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,double, 3,3dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,int,    4,4iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,short,  4,4sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,float,  4,4fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glVertex,double, 4,4dv)

EIGEN_GL_FUNC_DECLARATION       (glTexCoord)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,int,    2,2iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,short,  2,2sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,float,  2,2fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,double, 2,2dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,int,    3,3iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,short,  3,3sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,float,  3,3fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,double, 3,3dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,int,    4,4iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,short,  4,4sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,float,  4,4fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTexCoord,double, 4,4dv)

EIGEN_GL_FUNC_DECLARATION       (glColor)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,int,    2,2iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,short,  2,2sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,float,  2,2fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,double, 2,2dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,int,    3,3iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,short,  3,3sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,float,  3,3fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,double, 3,3dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,int,    4,4iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,short,  4,4sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,float,  4,4fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glColor,double, 4,4dv)

EIGEN_GL_FUNC_DECLARATION       (glNormal)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glNormal,int,    3,3iv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glNormal,short,  3,3sv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glNormal,float,  3,3fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glNormal,double, 3,3dv)

#ifndef EIGEN_INCLUDE_GL3

inline void glScale2fv(const float*  v) { glScalef(v[0], v[1], 1.f);  }
inline void glScale2dv(const double* v) { glScaled(v[0], v[1], 1.0);  }
inline void glScale3fv(const float*  v) { glScalef(v[0], v[1], v[2]); }
inline void glScale3dv(const double* v) { glScaled(v[0], v[1], v[2]); }

EIGEN_GL_FUNC_DECLARATION       (glScale)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glScale,float,  2,2fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glScale,double, 2,2dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glScale,float,  3,3fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glScale,double, 3,3dv)

template<typename Scalar> void glScale(const UniformScaling<Scalar>& s)  { glScale(Matrix<Scalar,3,1>::Constant(s.factor())); }

inline void glTranslate2fv(const float*  v) { glTranslatef(v[0], v[1], 0.f);  }
inline void glTranslate2dv(const double* v) { glTranslated(v[0], v[1], 0.0);  }
inline void glTranslate3fv(const float*  v) { glTranslatef(v[0], v[1], v[2]); }
inline void glTranslate3dv(const double* v) { glTranslated(v[0], v[1], v[2]); }

EIGEN_GL_FUNC_DECLARATION       (glTranslate)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTranslate,float,  2,2fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTranslate,double, 2,2dv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTranslate,float,  3,3fv)
EIGEN_GL_FUNC_SPECIALIZATION_VEC(glTranslate,double, 3,3dv)

template<typename Scalar> void glTranslate(const Translation<Scalar,2>& t)  { glTranslate(t.vector()); }
template<typename Scalar> void glTranslate(const Translation<Scalar,3>& t)  { glTranslate(t.vector()); }

EIGEN_GL_FUNC_DECLARATION       (glMultMatrix)
EIGEN_GL_FUNC_SPECIALIZATION_MAT(glMultMatrix,float,  4,4,f)
EIGEN_GL_FUNC_SPECIALIZATION_MAT(glMultMatrix,double, 4,4,d)

template<typename Scalar> void glMultMatrix(const Transform<Scalar,3,Affine>& t)        { glMultMatrix(t.matrix()); }
template<typename Scalar> void glMultMatrix(const Transform<Scalar,3,Projective>& t)    { glMultMatrix(t.matrix()); }
template<typename Scalar> void glMultMatrix(const Transform<Scalar,3,AffineCompact>& t) { glMultMatrix(Transform<Scalar,3,Affine>(t).matrix()); }

EIGEN_GL_FUNC_DECLARATION       (glLoadMatrix)
EIGEN_GL_FUNC_SPECIALIZATION_MAT(glLoadMatrix,float,  4,4,f)
EIGEN_GL_FUNC_SPECIALIZATION_MAT(glLoadMatrix,double, 4,4,d)

template<typename Scalar> void glLoadMatrix(const Transform<Scalar,3,Affine>& t)        { glLoadMatrix(t.matrix()); }
template<typename Scalar> void glLoadMatrix(const Transform<Scalar,3,Projective>& t)    { glLoadMatrix(t.matrix()); }
template<typename Scalar> void glLoadMatrix(const Transform<Scalar,3,AffineCompact>& t) { glLoadMatrix(Transform<Scalar,3,Affine>(t).matrix()); }

static void glRotate(const Rotation2D<float>& rot)
{
  glRotatef(rot.angle()*180.f/float(M_PI), 0.f, 0.f, 1.f);
}
static void glRotate(const Rotation2D<double>& rot)
{
  glRotated(rot.angle()*180.0/M_PI, 0.0, 0.0, 1.0);
}

template<typename Derived> void glRotate(const RotationBase<Derived,3>& rot)
{  
  Transform<typename Derived::Scalar,3,Projective> tr(rot);
  glMultMatrix(tr.matrix());
}

#else

/// @brief Returns a perspective transformation matrix like the one from gluPerspective
/// @see http://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
/// @see glm::perspective
template<typename Scalar>
Eigen::Matrix<Scalar,4,4> perspective(Scalar fovy, Scalar aspect, Scalar zNear, Scalar zFar){
  Transform<Scalar,3,Projective> tr; 
  tr.matrix().setZero();
  assert(aspect > 0);
  assert(zFar > zNear);
  assert(zNear > 0);
  Scalar radf = M_PI * fovy / 180.0;
  Scalar tan_half_fovy = std::tan(radf / 2.0);
  tr(0,0) = 1.0 / (aspect * tan_half_fovy);
  tr(1,1) = 1.0 / (tan_half_fovy);
  tr(2,2) = - (zFar + zNear) / (zFar - zNear);
  tr(3,2) = - 1.0;
  tr(2,3) = - (2.0 * zFar * zNear) / (zFar - zNear);
  return tr.matrix();
}

template<typename Scalar>
Eigen::Matrix<Scalar,4,4> scale(Scalar x, Scalar y, Scalar z){
  Transform<Scalar,3,Affine> tr;
  tr.matrix().setZero();
  tr(0,0) = x;
  tr(1,1) = y;
  tr(2,2) = z;
  tr(3,3) = 1;
  return tr.matrix();
}

template<typename Scalar>
Eigen::Matrix<Scalar,4,4> translate(Scalar x, Scalar y, Scalar z){
  Transform<Scalar,3,Affine> tr;
  tr.matrix().setIdentity();
  tr(0,3) = x;
  tr(1,3) = y;
  tr(2,3) = z;
  return tr.matrix();
}

/// @brief Returns a view transformation matrix like the one from glu's lookAt
/// @see http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
/// @see glm::lookAt
template<typename Derived>
Eigen::Matrix<typename Derived::Scalar,4,4> lookAt(Derived const & eye, Derived const & center, Derived const & up){
  typedef Eigen::Matrix<typename Derived::Scalar,4,4> Matrix4;
  typedef Eigen::Matrix<typename Derived::Scalar,3,1> Vector3;
  Vector3 f = (center - eye).normalized();
  Vector3 u = up.normalized();
  Vector3 s = f.cross(u).normalized();
  u = s.cross(f);
  Matrix4 mat = Matrix4::Zero();
  mat(0,0) = s.x();
  mat(0,1) = s.y();
  mat(0,2) = s.z();
  mat(0,3) = -s.dot(eye);
  mat(1,0) = u.x();
  mat(1,1) = u.y();
  mat(1,2) = u.z();
  mat(1,3) = -u.dot(eye);
  mat(2,0) = -f.x();
  mat(2,1) = -f.y();
  mat(2,2) = -f.z();
  mat(2,3) = f.dot(eye);
  mat.row(3) << 0,0,0,1; 
  return mat;
}

#endif 

#define EIGEN_GL_MAKE_CONST_const const
#define EIGEN_GL_MAKE_CONST__ 
#define EIGEN_GL_EVAL(X) X

#define EIGEN_GL_FUNC1_DECLARATION(FUNC,ARG1,CONST)                                                                             \
namespace internal {                                                                                                            \
  template< typename XprType,                                                                                                   \
            typename Scalar = typename XprType::Scalar,                                                                         \
            int Rows = XprType::RowsAtCompileTime,                                                                              \
            int Cols = XprType::ColsAtCompileTime,                                                                              \
            bool IsGLCompatible = bool(XprType::Flags&LinearAccessBit)                                                          \
                              && bool(XprType::Flags&DirectAccessBit)                                                           \
                              && (XprType::IsVectorAtCompileTime || (XprType::Flags&RowMajorBit)==0)>                           \
  struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl);                                                                                  \
                                                                                                                                \
  template<typename XprType, typename Scalar, int Rows, int Cols>                                                               \
  struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType,Scalar,Rows,Cols,false> {                                                 \
    inline static void run(ARG1 a,EIGEN_GL_EVAL(EIGEN_GL_MAKE_CONST_##CONST) XprType& p) {                                      \
      EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<typename plain_matrix_type_column_major<XprType>::type>::run(a,p); }                 \
  };                                                                                                                            \
}                                                                                                                               \
                                                                                                                                \
template<typename Derived> inline void FUNC(ARG1 a,EIGEN_GL_EVAL(EIGEN_GL_MAKE_CONST_##CONST) Eigen::DenseBase<Derived>& p) {   \
  EIGEN_CAT(EIGEN_CAT(internal::gl_,FUNC),_impl)<Derived>::run(a,p.derived());                                                  \
}


#define EIGEN_GL_FUNC1_SPECIALIZATION_MAT(FUNC,ARG1,CONST,SCALAR,ROWS,COLS,SUFFIX)                                              \
namespace internal {                                                                                                            \
  template< typename XprType> struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType, SCALAR, ROWS, COLS, true> {                  \
    inline static void run(ARG1 a, EIGEN_GL_EVAL(EIGEN_GL_MAKE_CONST_##CONST) XprType& p) { FUNC##SUFFIX(a,p.data()); }         \
  }; \
}

  
#define EIGEN_GL_FUNC1_SPECIALIZATION_VEC(FUNC,ARG1,CONST,SCALAR,SIZE,SUFFIX)                                                   \
namespace internal {                                                                                                            \
  template< typename XprType> struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType, SCALAR, SIZE, 1, true> {                     \
    inline static void run(ARG1 a, EIGEN_GL_EVAL(EIGEN_GL_MAKE_CONST_##CONST) XprType& p) { FUNC##SUFFIX(a,p.data()); }         \
  };                                                                                                                            \
  template< typename XprType> struct EIGEN_CAT(EIGEN_CAT(gl_,FUNC),_impl)<XprType, SCALAR, 1, SIZE, true> {                     \
    inline static void run(ARG1 a, EIGEN_GL_EVAL(EIGEN_GL_MAKE_CONST_##CONST) XprType& p) { FUNC##SUFFIX(a,p.data()); }         \
  };                                                                                                                            \
}

EIGEN_GL_FUNC1_DECLARATION       (glGet,GLenum,_)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glGet,GLenum,_,float,  4,4,Floatv)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glGet,GLenum,_,double, 4,4,Doublev)

// glUniform API

#ifdef GL_VERSION_2_0

static void glUniform2fv_ei  (GLint loc, const float* v)         { glUniform2fv(loc,1,v); }
static void glUniform2iv_ei  (GLint loc, const int* v)           { glUniform2iv(loc,1,v); }

static void glUniform3fv_ei  (GLint loc, const float* v)         { glUniform3fv(loc,1,v); }
static void glUniform3iv_ei  (GLint loc, const int* v)           { glUniform3iv(loc,1,v); }

static void glUniform4fv_ei  (GLint loc, const float* v)         { glUniform4fv(loc,1,v); }
static void glUniform4iv_ei  (GLint loc, const int* v)           { glUniform4iv(loc,1,v); }

static void glUniformMatrix2fv_ei  (GLint loc, const float* v)         { glUniformMatrix2fv(loc,1,false,v); }
static void glUniformMatrix3fv_ei  (GLint loc, const float* v)         { glUniformMatrix3fv(loc,1,false,v); }
static void glUniformMatrix4fv_ei  (GLint loc, const float* v)         { glUniformMatrix4fv(loc,1,false,v); }


EIGEN_GL_FUNC1_DECLARATION       (glUniform,GLint,const)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,float,        2,2fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,int,          2,2iv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,float,        3,3fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,int,          3,3iv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,float,        4,4fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,int,          4,4iv_ei)

EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        2,2,Matrix2fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        3,3,Matrix3fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        4,4,Matrix4fv_ei)

#endif

#ifdef GL_VERSION_2_1

static void glUniformMatrix2x3fv_ei(GLint loc, const float* v)         { glUniformMatrix2x3fv(loc,1,false,v); }
static void glUniformMatrix3x2fv_ei(GLint loc, const float* v)         { glUniformMatrix3x2fv(loc,1,false,v); }
static void glUniformMatrix2x4fv_ei(GLint loc, const float* v)         { glUniformMatrix2x4fv(loc,1,false,v); }
static void glUniformMatrix4x2fv_ei(GLint loc, const float* v)         { glUniformMatrix4x2fv(loc,1,false,v); }
static void glUniformMatrix3x4fv_ei(GLint loc, const float* v)         { glUniformMatrix3x4fv(loc,1,false,v); }
static void glUniformMatrix4x3fv_ei(GLint loc, const float* v)         { glUniformMatrix4x3fv(loc,1,false,v); }

EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        2,3,Matrix2x3fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        3,2,Matrix3x2fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        2,4,Matrix2x4fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        4,2,Matrix4x2fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        3,4,Matrix3x4fv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_MAT(glUniform,GLint,const,float,        4,3,Matrix4x3fv_ei)

#endif

#ifdef GL_VERSION_3_0

static void glUniform2uiv_ei (GLint loc, const unsigned int* v)  { glUniform2uiv(loc,1,v); }
static void glUniform3uiv_ei (GLint loc, const unsigned int* v)  { glUniform3uiv(loc,1,v); }
static void glUniform4uiv_ei (GLint loc, const unsigned int* v)  { glUniform4uiv(loc,1,v); }

EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,unsigned int, 2,2uiv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,unsigned int, 3,3uiv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,unsigned int, 4,4uiv_ei)

#endif

#ifdef GL_ARB_gpu_shader_fp64
static void glUniform2dv_ei  (GLint loc, const double* v)        { glUniform2dv(loc,1,v); }
static void glUniform3dv_ei  (GLint loc, const double* v)        { glUniform3dv(loc,1,v); }
static void glUniform4dv_ei  (GLint loc, const double* v)        { glUniform4dv(loc,1,v); }

EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,double,       2,2dv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,double,       3,3dv_ei)
EIGEN_GL_FUNC1_SPECIALIZATION_VEC(glUniform,GLint,const,double,       4,4dv_ei)
#endif


//@}

}

#endif // EIGEN_OPENGL_MODULE
