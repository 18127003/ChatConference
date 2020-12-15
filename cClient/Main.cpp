#include "Login.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Client::getObject()->loginScreen = gcnew MyClient::Login;
	Application::Run(Client::getObject()->loginScreen);
}