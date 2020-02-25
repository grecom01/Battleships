#include "LoginUI.h"
#include "Network/ServerConnection.h"

void LoginUI::build(sf::RenderWindow* window, ServerConnection* server)
{
	m_gui.setTarget(*window);

	auto vbox = tgui::VerticalLayout::create({ "25%", "50%" });
	vbox->setPosition({ "37,5%", "25%" });

	auto title = tgui::Label::create("Battleships");
	title->getRenderer()->setTextColor(tgui::Color::White);
	title->setTextSize(28);
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);

	vbox->add(title);
	vbox->addSpace(1.0f);

	auto loginForm = tgui::VerticalLayout::create();

	auto nameField = tgui::EditBox::create();
	nameField->setMaximumCharacters(32);
	nameField->setDefaultText("Enter your name");
	nameField->setTextSize(12);

	auto playButton = tgui::Button::create();
	playButton->setText("Play");

	auto connectionStatus = tgui::Label::create();
	connectionStatus->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	connectionStatus->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	connectionStatus->setText("Connecting to server...");
	connectionStatus->setTextSize(12);
	connectionStatus->getRenderer()->setTextColor(tgui::Color::White);

	auto connectFunc = [=]() {
		if (!server->isConnected()) {
		}
		else {
			server->sendLoginRequest(nameField->getText());
		}
	};

	nameField->connect("ReturnKeyPressed", connectFunc);
	playButton->connect("pressed", connectFunc);

	loginForm->add(nameField);
	loginForm->addSpace(0.5f);

	auto hbox = tgui::HorizontalLayout::create();

	auto exitButton = tgui::Button::create();
	exitButton->setText("Exit");
	exitButton->connect("pressed", [=]() {
		server->disconnect();
		window->close();
	});

	hbox->add(playButton);
	hbox->addSpace(0.75f);
	hbox->add(exitButton);

	loginForm->add(hbox);
	loginForm->setVisible(false);

	vbox->add(loginForm, "loginForm");
	vbox->addSpace(1.f);
	vbox->add(connectionStatus, "connectionStatus");

	m_gui.add(vbox);
}

void LoginUI::handleEvent(const sf::Event& event)
{
	m_gui.handleEvent(event);
}

void LoginUI::draw()
{
	m_gui.draw();
}

void LoginUI::setConnectionStatus(bool connected)
{
	auto connectionStatus = m_gui.get<tgui::Label>("connectionStatus");

	if (connected) {
		connectionStatus->setText("Connected.");
		connectionStatus->getRenderer()->setTextColor(tgui::Color::Green);
	}
	else {
		connectionStatus->setText("Could not connect to server.\nPress \"Play\" to start a game against a bot.");
		connectionStatus->getRenderer()->setTextColor(tgui::Color::Red);
	}
}

void LoginUI::enableLoginForm(bool enable)
{
	m_gui.get<tgui::VerticalLayout>("loginForm")->setVisible(enable);
}