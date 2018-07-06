////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();

	void SetPosition(float x, float y, float z);
	void AdjustPosition(XMVECTOR posOffset);
	void SetRotation(float xRot, float yRot, float zRot);
	void AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset);

	XMVECTOR GetPosition();
	XMFLOAT3 GetRotation();


	const XMMATRIX GetViewMatrix();
	const XMMATRIX GetProjectionMatrix();
	void SetProjectionValues(float FOV, float width, float height, float nearZ, float farZ);
	const XMVECTOR & GetForwardVector();
	const XMVECTOR & GetRightVector();
	const XMVECTOR & GetBackwardVector();
	const XMVECTOR & GetLeftVector();
private:
	void UpdateViewMatrix();
	XMVECTOR m_pos;
	XMFLOAT3 m_rot;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;

	const XMVECTOR m_DEFAULT_FORWARD_VECTOR = { 0, 0, 1 };
	const XMVECTOR m_DEFAULT_BACKWARD_VECTOR = { 0, 0, -1 };
	const XMVECTOR m_DEFAULT_UP_VECTOR = { 0, 1, 0 };
	const XMVECTOR m_DEFAULT_LEFT_VECTOR = { -1, 0, 0 };
	const XMVECTOR m_DEFAULT_RIGHT_VECTOR = { 1, 0, 0 };

	XMVECTOR m_vec_forward;
	XMVECTOR m_vec_left;
	XMVECTOR m_vec_right;
	XMVECTOR m_vec_backward;


};

#endif