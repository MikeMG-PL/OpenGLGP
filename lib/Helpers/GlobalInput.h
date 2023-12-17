#pragma once

struct GLFWwindow;

void globalInput(GLFWwindow* window);
void setKeyCallbacks();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);