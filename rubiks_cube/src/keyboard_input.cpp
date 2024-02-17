#include "keyboard_input.h"

KeyboardInput::KeyboardInput(GLFWwindow* glfwWindow) : 
	keyNameMap{}, keyStatusMap{}, window{glfwWindow}
{}

void KeyboardInput::setKeybind(const char* keyName, int glfwKey)
{
	keyNameMap[keyName] = glfwKey;
	keyStatusMap[glfwKey] = {false, false};
}

void KeyboardInput::removeKeybind(const char* keyName)
{
	auto it = keyNameMap.find(keyName);
	if(it != keyNameMap.end())
	{
		keyStatusMap.erase(it->second);
		keyNameMap.erase(it);
	}
}

void KeyboardInput::update()
{
	for(auto& [key, status] : keyStatusMap)
	{
		status.previous = status.current;
		if(glfwGetKey(window, key) == GLFW_PRESS)
			status.current = true;
		else
			status.current = false;
	}
}

bool KeyboardInput::keyJustPressed(const char* keyName) const
{
	auto it = keyNameMap.find(keyName);
	if(it != keyNameMap.end())
	{
		const KeyStatus& status = keyStatusMap.at(it->second);
		return status.current && !status.previous;
	}
	else
	{
		return false;
	}
}

bool KeyboardInput::keyPressed(const char* keyName) const
{
	auto it = keyNameMap.find(keyName);
	if(it != keyNameMap.end())
	{
		const KeyStatus& status = keyStatusMap.at(it->second);
		return status.current;
	}
	else
	{
		return false;
	}
}

bool KeyboardInput::keyJustReleased(const char* keyName) const
{
	auto it = keyNameMap.find(keyName);
	if(it != keyNameMap.end())
	{
		const KeyStatus& status = keyStatusMap.at(it->second);
		return !status.current && status.previous;
	}
	else
	{
		return false;
	}
}

bool KeyboardInput::keyReleased(const char* keyName) const
{
	auto it = keyNameMap.find(keyName);
	if(it != keyNameMap.end())
	{
		const KeyStatus& status = keyStatusMap.at(it->second);
		return !status.current;
	}
	else
	{
		return false;
	}
}

bool KeyboardInput::anyKeyJustPressed() const
{
	for(auto& [key, status] : keyStatusMap)
	{
		if(status.current && !status.previous) 
			return true;
	}
	return false;
}

bool KeyboardInput::anyKeyPressed() const
{
	for(auto& [key, status] : keyStatusMap)
	{
		if(status.current)
			return true;
	}
	return false;
}

bool KeyboardInput::anyKeyJustReleased() const
{
	for(auto& [key, status] : keyStatusMap)
	{
		if(!status.current && status.previous)
			return true;
	}
	return false;
}

bool KeyboardInput::anyKeyReleased() const
{
	for(auto& [key, status] : keyStatusMap)
	{
		if(!status.current)
			return true;
	}
	return false;
}