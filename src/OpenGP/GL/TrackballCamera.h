#pragma once 
#include <OpenGP/types.h>
#include <Eigen/Geometry>
#include <Eigen/Dense>

//=============================================================================
namespace OpenGP {
//=============================================================================

class TrackballCamera 
{

public:
	TrackballCamera() 
	{ 
		radius_ = 1.0f; 

		SetCenter(Eigen::Vector3f(0, 0, 0));
		SetDefaults();
	}

	TrackballCamera(float radius, Eigen::Vector3f center)
	{
		radius_ = radius; 

		SetCenter(center);
		SetDefaults();
	}

    void BeginRotate(Eigen::Vector3f pos) 
    {
        current_pos_ = pos;
        ProjectOntoSurface(current_pos_);
 		anchor_pos_ = current_pos_;
    }

    void FinishRotate()
    {
    	old_rotation_ = rotation_;
    }

    void Rotate(Eigen::Vector3f pos) 
    {
        current_pos_= pos;
        ProjectOntoSurface(current_pos_);
 
        const float angle_boost = 5.0f;
        Eigen::Vector3f axis = anchor_pos_.cross(current_pos_);
        float angle = angle_boost * atan2(axis.norm(), anchor_pos_.dot(current_pos_));

        if(angle > 0.0f)
        {   
	        axis.normalize();
	        rotation_ = Eigen::Affine3f(Eigen::AngleAxisf(angle, axis)).matrix();
	       	rotation_ *= old_rotation_;

        	UpdateMatrices();
       	}
    }

    void Translate(Eigen::Vector3f pos, Eigen::Vector3f old_pos)
    {
    	//TODO: Should probably check for more degenerate cases
    	//TODO: Package lines and planes in structs?

    	//Construct plane perpendicular to view direction and passing through trackball center
    	Eigen::Vector3f camera_position = CameraPosition();
    	Eigen::Vector3f plane_normal = (camera_position - center_).normalized();

    	//Construct line from camera center to unprojected points
    	Eigen::Vector3f unprojected_pos = UnProject(pos);
    	Eigen::Vector3f unprojected_old_pos = UnProject(old_pos);

    	Eigen::Vector3f line_direction = unprojected_pos - camera_position;
    	Eigen::Vector3f line_direction_old = unprojected_old_pos - camera_position;

    	Eigen::Vector3f intersection_pos, intersection_old_pos;

    	bool found_intersections = true;
    	found_intersections &= PlaneLineIntersection(plane_normal, center_, line_direction, unprojected_pos, intersection_pos);
    	found_intersections &= PlaneLineIntersection(plane_normal, center_, line_direction_old, unprojected_old_pos, intersection_old_pos);
    	
    	if(found_intersections)
    	{
	    	Eigen::Vector3f translation_pos = intersection_pos - intersection_old_pos;
	        translation_ *= Eigen::Affine3f(Eigen::Translation3f(translation_pos)).matrix();
    	}

        UpdateMatrices();
    }

    void Scale(float offset_y)
    {
    	scale_ *= std::pow(1.2f, -offset_y);
    	translation_(0,0) = translation_(1,1) = translation_(2,2) = scale_;

    	UpdateMatrices();
    }

    void Focus(Eigen::Vector3f pos)
    {	
    	pos = UnProject(pos);
    	SetCenter(pos);

    	//TODO: Really should be a better way of reseting these matrices;
        view_matrix_ = OpenGP::lookAt(camera_position_, center_, camera_up_);

        translation_ = Eigen::Matrix4f::Identity();
        translation_(0,0) = translation_(1,1) = translation_(2,2) = scale_;

    	UpdateMatrices();
    }

	void SetDefaults()
    {
    	//TODO: Maybe make default parameters adjustable
    	model_matrix_ = Eigen::Matrix4f::Identity();

    	rotation_ = Eigen::Matrix4f::Identity();
		old_rotation_ = Eigen::Matrix4f::Identity();
		translation_ = Eigen::Matrix4f::Identity();
		scale_ = 1.0f;

		//TODO: This should get resized
		projection_matrix_ = OpenGP::perspective(45.0f, 4.0/3.0, 0.1f, 10.f);

    	camera_position_ = Eigen::Vector3f(0,0,5);
        camera_up_= Eigen::Vector3f(0,1,0);
        view_matrix_ = OpenGP::lookAt(camera_position_, center_, camera_up_);

        view_model_matrix_ = view_matrix_ * model_matrix_;
    }

	void ResizeProjection(int width, int height)
    {
    	projection_matrix_ = OpenGP::perspective(45.0f, (float)width/(float)height, 0.1f, 10.0f);
    }

    void SetCenter(Eigen::Vector3f center)
    {
    	center_ = center;
    	translate_center_ = Eigen::Affine3f(Eigen::Translation3f(center_)).matrix();
    	translate_minus_center_ = Eigen::Affine3f(Eigen::Translation3f(-center)).matrix();
    }

    const Eigen::Matrix4f& getViewModelMatrix()
    {
    	return view_model_matrix_;
    }

    const Eigen::Matrix4f& getProjectionMatrix()
    {
    	return projection_matrix_;
    } 
 
private:
	void UpdateMatrices()
	{
		model_matrix_ =  translate_center_ * rotation_ * translation_ * translate_minus_center_;
		view_model_matrix_ = view_matrix_ * model_matrix_;
	}	

	//TODO: Maybe don't pass by value/return by value
	Eigen::Vector3f CameraPosition()
	{
		Eigen::Vector4f result(0.0f, 0.0f, 0.0f, 1.0f);
		result = view_model_matrix_.inverse() * result;
		return Eigen::Vector3f(result(0), result(1), result(2));
	}

	Eigen::Vector3f UnProject(Eigen::Vector3f pos)
	{
		Eigen::Vector4f result(pos(0), pos(1), pos(2), 1.0f);
		result = (projection_matrix_ * view_model_matrix_).inverse() * result;
		return Eigen::Vector3f(result(0), result(1), result(2)) / result(3);;
	}

	void ProjectOntoSurface(Eigen::Vector3f& pos) 
	{
        const float radius2 = radius_ * radius_;

        const float length2 = pos.squaredNorm();

        if (length2 <= radius2 * 0.5f) 
        {
            pos[2] = std::sqrt(radius2 - length2);
        } 
        else 
        {
            pos[2] = radius2 / (2.0f * std::sqrt(length2));
        }

        float length = pos.norm();
        pos /= length;
    }

    //TODO: These geometric functions should really be somewhere else 
    bool PlaneLineIntersection(const Eigen::Vector3f plane_normal, const Eigen::Vector3f plane_point, 
    						   const Eigen::Vector3f line_direction, const Eigen::Vector3f line_point,
    						   Eigen::Vector3f &intersection_point)
    {
    	const float epsilon = 1e-8;
    	float k = plane_normal.dot(line_direction);

    	if(k > -epsilon && k < epsilon)
    		return false;

    	float s = plane_normal.dot(plane_point - line_point) / k;
    	intersection_point = line_point + s * line_direction;

    	return true;
    }

	float radius_;
	float scale_;
	Eigen::Vector3f center_;

    Eigen::Vector3f anchor_pos_;
    Eigen::Vector3f current_pos_;

    Eigen::Vector3f camera_position_;
    Eigen::Vector3f camera_up_;

    Eigen::Matrix4f rotation_;
    Eigen::Matrix4f old_rotation_;

    Eigen::Matrix4f translation_;
    Eigen::Matrix4f translate_center_;
    Eigen::Matrix4f translate_minus_center_;

    Eigen::Matrix4f model_matrix_;
    Eigen::Matrix4f view_matrix_;
    Eigen::Matrix4f view_model_matrix_;
    Eigen::Matrix4f projection_matrix_;
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
