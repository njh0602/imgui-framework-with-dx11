#include "node/ImGuiLayer.h"
#include "base/Director.h"
#include "renderer/Renderer.h"


ImGuiLayer* ImGuiLayer::_sharedInstance = nullptr;

ImGuiLayer::ImGuiLayer()
{
}


ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplDX11_Shutdown();
}


ImGuiLayer* ImGuiLayer::create()
{
	auto ret = new (std::nothrow) ImGuiLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


bool ImGuiLayer::init()
{
	if (!Node::init())
		return false;

	auto renderer = Director::getInstance()->getRenderer();
	ImGui_ImplDX11_Init(renderer->getWindowHandle(), renderer->getDevice(), renderer->getDeviceContext());

	return true;
}


void ImGuiLayer::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Node::visit(renderer, parentTransform, parentFlags);

	_customCommand.init(0);
	_customCommand.func = std::bind(&ImGuiLayer::onDraw, this);
	renderer->addCommand(&_customCommand);
}


void ImGuiLayer::onDraw()
{
	ImGui_ImplDX11_NewFrame();
	for (const auto& func : _imguiFuncs)
	{
		func();
	}
	ImGui::Render();
}


void ImGuiLayer::addImGui(const std::function<void()>& func)
{
	_imguiFuncs.push_back(func);
}
