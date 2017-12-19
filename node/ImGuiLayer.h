#pragma once

#include <vector>
#include <functional>

#include "node/Node.h"
#include "renderer/CustomCommand.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"


class Renderer;

class ImGuiLayer : public Node
{

public:

	ImGuiLayer();
	virtual ~ImGuiLayer();

	static ImGuiLayer* create();
	virtual bool init() override;

	virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

	void onDraw();
	void addImGui(const std::function<void()>& func);

private:

	static ImGuiLayer*					_sharedInstance;
	CustomCommand						_customCommand;
	std::vector<std::function<void()>>	_imguiFuncs;

};