#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class Gui
{
public:
	~Gui();
	static void Setup(GLFWwindow* window);
	static void NewFrame();
	static void Render();
};
