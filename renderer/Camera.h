#pragma once

#include "node/Node.h"
#include "math/Mat4.h"
#include "math/Vec3.h"


class Scene;


enum CameraFlag
{
	DEFAULT = 1,
	USER1 = 1 << 1,
	USER2 = 1 << 2,
	USER3 = 1 << 3,
	USER4 = 1 << 4,
	USER5 = 1 << 5,
	USER6 = 1 << 6,
	USER7 = 1 << 7,
	USER8 = 1 << 8
};

class Camera : public Node
{

	friend class Scene;

public:

	Camera();
	virtual ~Camera();

	static Camera* createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	bool initWithPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
	static const Camera* getVisitingCamera() { return _visitingCamera; }

	void setScene(Scene* scene);

	virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

	virtual void onEnter() override;
	virtual void onExit() override;

	CameraFlag getCameraFlag() const { return _cameraFlag; }
	void setCameraFlag(CameraFlag flag) { _cameraFlag = flag; }

	bool isViewProjectionUpdated() const { return _viewProjectionUpdated; }

	void apply();
	void lookAt(const Vec3& target, const Vec3& up = Vec3::UNIT_Y);

	const Mat4& getProjectionMatrix() const;
	const Mat4& getViewMatrix() const;
	const Mat4& getViewProjectionMatrix() const;

private:

	Scene*				_scene;

	static Camera*		_visitingCamera;

	float				_fieldOfView;
	float				_acpectRatio;
	float				_nearPlane;
	float				_farPlane;

	Vec3				_position;
	Vec3				_rotation;

	Mat4				_projection;
	mutable Mat4		_viewProjection;
	mutable Mat4		_view;
	mutable Mat4		_viewInv;
	mutable bool		_viewProjectionDirty;
	bool				_viewProjectionUpdated;

	CameraFlag			_cameraFlag;

};