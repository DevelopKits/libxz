#pragma once

#include "node.h"

class WindowNode;
class RootNode : public Node
{
public:
	RootNode();
	virtual ~RootNode() override;

public:
	void AddWindow(WindowNode* node);
	void RemoveWindow(WindowNode* node);
	WindowNode* FindWindow(HWND hwnd);

	//��û�д���ʱ�˳�
	void DetermineQuit();
public:
	LRESULT HandleMessage(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);


private:
	std::vector<WindowNode*> windows_;
};