#pragma once

#include <TGUI/TGUI.hpp>

class ServerConnection;

class LoginUI
{
	public:
	void build(sf::RenderWindow* window, ServerConnection* server);
	void handleEvent(const sf::Event& event);
	void draw();

	void setConnectionStatus(bool connected);
	void enableLoginForm(bool enable);

	private:
	tgui::Gui m_gui;
};