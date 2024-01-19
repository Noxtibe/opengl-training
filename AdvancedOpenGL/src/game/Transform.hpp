#pragma once

#include "../engine/maths/Matrix4.h"

class Transform
{
public:

	void set_position( const Vector3 pos )
	{
		_pos = pos;
		_is_dirty = true;
	}
	void set_scale( const Vector3& scale )
	{
		_scale = scale;
		_is_dirty = true;
	}
	void set_rotation( const Quaternion& rotation )
	{
		_rotation = rotation;
		_is_dirty = true;
	}
	void set_angles( const Vector3& angles )
	{
		_angles = angles;
		_is_dirty = true;
	}

	const Vector3& get_position() { return _pos; }
	const Vector3& get_scale() { return _scale; }
	const Quaternion& get_rotation() { return _rotation; }
	const Vector3& get_angles() { return _angles; }

	const Matrix4& get_matrix()
	{
		if ( _is_dirty )
		{
			_matrix =
				Matrix4::createTranslation( _pos )
				* Matrix4::createScale( _scale )
				* Matrix4::createRotationZ( _angles.z )
				* Matrix4::createRotationY( _angles.y )
				* Matrix4::createRotationX( _angles.x );

			_is_dirty = false;
		}

		return _matrix;
	}

private:

	bool _is_dirty = true;
	Matrix4 _matrix;

	Vector3 _pos { 0.0f, 0.0f, 0.0f };
	Vector3 _scale { 1.0f, 1.0f, 1.0f };
	Vector3 _angles { 0.0f, 0.0f, 0.0f };
	Quaternion _rotation = Quaternion::identity;
};