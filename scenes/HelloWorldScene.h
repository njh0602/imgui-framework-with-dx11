#pragma once

#include "learn_dx11.h"

class HelloWorldScene : public ImGuiLayer
{

public:

	HelloWorldScene();
	virtual ~HelloWorldScene();

	static HelloWorldScene* create();
	static Scene* createScene();

	virtual bool init() override;
	virtual void update(float dt) override;

private:

	Model3D* _cube1;
	Model3D* _cube2;

};