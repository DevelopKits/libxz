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

	void ShowMainWindow();

	//��û�д���ʱ�˳�
	void DetermineQuit();

	void CheckAndApplyProperties();
public:
	LRESULT HandleMessage(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);


private:
	void TranslateNotify(WPARAM w_param, LPARAM l_param);
	std::vector<WindowNode*> windows_;
};