#include "learn_dx11.h"
#include "HelloWorldScene.h"


HelloWorldScene::HelloWorldScene()
{
}


HelloWorldScene::~HelloWorldScene()
{
}


HelloWorldScene * HelloWorldScene::create()
{
	auto ret = new (std::nothrow) HelloWorldScene();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


Scene* HelloWorldScene::createScene()
{
	auto scene = Scene::create();
	auto node = HelloWorldScene::create();
	scene->addChild(node);
	return scene;
}


bool HelloWorldScene::init()
{
	if (!ImGuiLayer::init())
		return false;

	addImGui([this] {

		auto director = Director::getInstance();
		auto runningScene = director->getRunningScene();

		static bool alwaysShow = true;
		ImGui::SetNextWindowPos(ImVec2(200, 50), ImGuiSetCond_Once);
		ImGui::Begin(" Scene", &alwaysShow, ImVec2(director->getWindowSize().width - 400, director->getWindowSize().height - 172), 0.0f,
			ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoCollapse | 
			ImGuiWindowFlags_NoResize);

		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

		auto x = ImGui::GetWindowPos().x;
		auto y = ImGui::GetWindowPos().y;

		runningScene->setPosition(x, -y, 0.0);
		runningScene->setClipSize(Size(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, 100), ImGuiSetCond_Once);
		ImGui::Begin(" Hierarchy", &alwaysShow, ImVec2(200, director->getWindowSize().height - 172), 0.3f,
			ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoCollapse | 
			ImGuiWindowFlags_NoResize);

		for (auto & child : _children)
			ImGui::Selectable(child->getName().c_str());

		ImGui::End();
	});

	auto defaultShader = DefaultShader::createWithFileName("shaders/shader.shader");

	for (int i = 0; i < 3000; ++i)
	{
		auto cube = Model3D::createWithFile("res/default_cube.xml");
		cube->setName("Cube" + std::to_string(i));
		cube->setShaderProgram(defaultShader);
		cube->setPosition(RandomHelper::random_int(-800, 300), RandomHelper::random_int(-300, 300), RandomHelper::random_int(-300, 300));
		cube->setRotation(RandomHelper::random_int(-300, 300), RandomHelper::random_int(-300, 300), RandomHelper::random_int(-300, 300));
		cube->setScale(5, 5, 5);
		addChild(cube);
	}

	auto texture = TextureCache::getInstance().addImage("res/wood.png");

	return true;
}


void HelloWorldScene::update(float dt)
{
	static float speed = 0.0f;
	speed = 360 * dt;

	for (auto & child : _children)
		child->setRotation( child->getRotation().x + speed, child->getRotation().y + speed, child->getRotation().z + speed);
}
