#pragma once

#include <vector>

#include "node/Node.h"
#include "math/Geometry.h"


class Camera;
class Renderer;

class Scene : public Node
{

	friend class Camera;

public:

	Scene();
	virtual ~Scene();

	static Scene* create();
	virtual bool init() override;

	virtual void render(Renderer* renderer);
	virtual void removeAllChildren() override;

	void setClipSize(const Size& size);
	Size getClipSize() const { return _clipSize; }

private:

	Vector<Camera*>			_cameras;
	Camera*					_defaultCamera;
	Size					_clipSize;

};