#include "Camera.h"
Camera::Camera()
{
	m_pos.m128_f32[0] = 0;
	m_pos.m128_f32[1] = 0;
	m_pos.m128_f32[2] = 0;
	m_pos.m128_f32[3] = 0;
	m_rot.x = 0; 
	m_rot.y = 0; 
	m_rot.z = 0;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	m_pos.m128_f32[0] = x;
	m_pos.m128_f32[1] = y;
	m_pos.m128_f32[2] = z;
	m_pos.m128_f32[3] = 0;
	UpdateViewMatrix();
}

void Camera::AdjustPosition(XMVECTOR posOffset)
{
	m_pos += posOffset;
	m_pos.m128_f32[3] = 0;
	UpdateViewMatrix();
}

void Camera::SetRotation(float xRot, float yRot, float zRot)
{
	m_rot.x = xRot/180*3.14159;
	m_rot.y = yRot / 180 * 3.14159;
	m_rot.z = zRot / 180 * 3.14159;
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset)
{
	m_rot.x += xRotOffset;
	m_rot.y += yRotOffset;
	m_rot.z += zRotOffset;
	if (m_rot.x < -XM_PIDIV2)
		m_rot.x = -XM_PIDIV2;
	else if (m_rot.x > XM_PIDIV2)
		m_rot.x = XM_PIDIV2;
	if (m_rot.z < -XM_PIDIV2)
		m_rot.z = -XM_PIDIV2;
	else if (m_rot.z > XM_PIDIV2)
		m_rot.z = XM_PIDIV2;
	if (m_rot.y < 0)
		m_rot.y += XM_2PI;
	else if (m_rot.y > XM_2PI)
		m_rot.y -= XM_2PI;
	UpdateViewMatrix();
}

XMVECTOR Camera::GetPosition()
{
	return m_pos;
}

XMFLOAT3 Camera::GetRotation()
{
	return m_rot;
}

const XMMATRIX Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

const XMMATRIX Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void Camera::SetProjectionValues(float FOV, float width, float height, float nearZ, float farZ)
{
	m_projectionMatrix = XMMatrixPerspectiveFovLH((FOV/180)*3.14f, (float)width / height, nearZ, farZ); //90 degree field of view
}

const XMVECTOR & Camera::GetForwardVector()
{
	m_vec_forward.m128_f32[1] = 0;
	return m_vec_forward;
}

const XMVECTOR & Camera::GetRightVector()
{
	m_vec_right.m128_f32[1] = 0;
	return m_vec_right;
}

const XMVECTOR & Camera::GetBackwardVector()
{
	m_vec_backward.m128_f32[1] = 0;
	return m_vec_backward;
}

const XMVECTOR & Camera::GetLeftVector()
{
	m_vec_left.m128_f32[1] = 0;
	return m_vec_left;
}

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(m_DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);
	//Adjust cam target to be offset by the camera's current position
	camTarget += m_pos;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(m_DEFAULT_UP_VECTOR, camRotationMatrix);

	m_viewMatrix = XMMatrixLookAtLH(m_pos, camTarget, upDir);

	XMMATRIX camRotationMatrix2 = XMMatrixRotationRollPitchYaw(0, m_rot.y, m_rot.z);
	m_vec_forward = XMVector3TransformCoord(m_DEFAULT_FORWARD_VECTOR, camRotationMatrix2);
	m_vec_backward = XMVector3TransformCoord(m_DEFAULT_BACKWARD_VECTOR, camRotationMatrix2);
	m_vec_left = XMVector3TransformCoord(m_DEFAULT_LEFT_VECTOR, camRotationMatrix2);
	m_vec_right = XMVector3TransformCoord(m_DEFAULT_RIGHT_VECTOR, camRotationMatrix2);

}
