#include "ServerInterface.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Server::getObject()->mainScreen = gcnew MyServer::ServerInterface;
	Application::Run(Server::getObject()->mainScreen);

	//Server::MainScreen MainForm;
	//Application::Run(%MainForm);
}