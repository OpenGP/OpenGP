#if !defined(SURFACE_MESH_VECTOR_BACK_COMPATIBILITY) && defined(USE_EIGEN)
#define SURFACE_MESH_VECTOR_BACK_COMPATIBILITY

/** 1-byte signed vector */
typedef Eigen::Matrix<signed char,1,1> Vec1c;
/** 1-byte unsigned vector */
typedef Eigen::Matrix<unsigned char,1,1> Vec1uc;
/** 1-short signed vector */
typedef Eigen::Matrix<signed short int,1,1> Vec1s;
/** 1-short unsigned vector */
typedef Eigen::Matrix<unsigned short int,1,1> Vec1us;
/** 1-int signed vector */
typedef Eigen::Matrix<signed int,1,1> Vec1i;
/** 1-int unsigned vector */
typedef Eigen::Matrix<unsigned int,1,1> Vec1ui;
/** 1-float vector */
typedef Eigen::Matrix<float,1,1> Vec1f;
/** 1-double vector */
typedef Eigen::Matrix<double,1,1> Vec1d;


/** 2-byte signed vector */
typedef Eigen::Matrix<signed char,2,1> Vec2c;
/** 2-byte unsigned vector */
typedef Eigen::Matrix<unsigned char,2,1> Vec2uc;
/** 2-short signed vector */
typedef Eigen::Matrix<signed short int,2,1> Vec2s;
/** 2-short unsigned vector */
typedef Eigen::Matrix<unsigned short int,2,1> Vec2us;
/** 2-int signed vector */
typedef Eigen::Matrix<signed int,2,1> Vec2i;
/** 2-int unsigned vector */
typedef Eigen::Matrix<unsigned int,2,1> Vec2ui;
/** 2-float vector */
typedef Eigen::Matrix<float,2,1> Vec2f;
/** 2-double vector */
typedef Eigen::Matrix<double,2,1> Vec2d;

/** 3-byte signed vector */
typedef Eigen::Matrix<signed char,3,1> Vec3c;
/** 3-byte unsigned vector */
typedef Eigen::Matrix<unsigned char,3,1> Vec3uc;
/** 3-short signed vector */
typedef Eigen::Matrix<signed short int,3,1> Vec3s;
/** 3-short unsigned vector */
typedef Eigen::Matrix<unsigned short int,3,1> Vec3us;
/** 3-int signed vector */
typedef Eigen::Matrix<signed int,3,1> Vec3i;
/** 3-int unsigned vector */
typedef Eigen::Matrix<unsigned int,3,1> Vec3ui;
/** 3-float vector */
typedef Eigen::Matrix<float,3,1> Vec3f;
/** 3-double vector */
typedef Eigen::Matrix<double,3,1> Vec3d;

/** 4-byte signed vector */
typedef Eigen::Matrix<signed char,4,1> Vec4c;
/** 4-byte unsigned vector */
typedef Eigen::Matrix<unsigned char,4,1> Vec4uc;
/** 4-short signed vector */
typedef Eigen::Matrix<signed short int,4,1> Vec4s;
/** 4-short unsigned vector */
typedef Eigen::Matrix<unsigned short int,4,1> Vec4us;
/** 4-int signed vector */
typedef Eigen::Matrix<signed int,4,1> Vec4i;
/** 4-int unsigned vector */
typedef Eigen::Matrix<unsigned int,4,1> Vec4ui;
/** 4-float vector */
typedef Eigen::Matrix<float,4,1> Vec4f;
/** 4-double vector */
typedef Eigen::Matrix<double,4,1> Vec4d;

#if defined(TODO_SURFACEMESH_DOT_CROSS)
    /// compute the cross product of two vectors (only valid for 3D vectors)
    /// @todo why this doesn't work in the legacy Surface_mesh::compute_face_normal!?
    template <class Derived>
    inline Derived cross(const Derived& v0, const Derived& v1)
    {
        return v0.cross(v1);
    }    
#endif

/// compute the dot product of two vectors
template <typename Scalar, int N>
inline Scalar dot(const Eigen::Matrix<Scalar, N, 1>& v0, const Eigen::Matrix<Scalar, N, 1>& v1)
{
    return v0.dot(v1);
}

/// norm of a vector
template <class Derived>
inline typename Derived::Scalar norm(const Derived& v)
{
    return v.norm();
}

/// Basic types
typedef Vec3 Point;                     ///< Point type
typedef Vec3 Normal;                    ///< Normal type
typedef Vec3 Color;                     ///< Color type
typedef Vec3 Texture_coordinate;        ///< Texture coordinate type

#endif