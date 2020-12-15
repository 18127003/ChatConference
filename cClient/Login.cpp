#include "Login.h"

using namespace MyClient;

System::Void Login::butSignup_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^ username = txtUsername->Text;
	String^ password = txtPassword->Text;

	Client::getObject()->strUsername = username;
	Client::getObject()->signup(username, password);
}
System::Void Login::Login_Load(System::Object^  sender, System::EventArgs^  e) {
	if (Client::getObject()->cli_socket->connectToServer() != 0) //Connect to server
	{
		Application::Exit(); //Exit app
	}

	Client::getObject()->createThreadListenMessageFromServer();
}

System::Void Login::butLogin_Click(System::Object^  sender, System::EventArgs^  e) {
	String^ username = txtUsername->Text;
	String^ password = txtPassword->Text;

	Client::getObject()->strUsername = username;
	Client::getObject()->login(username, password);
}
