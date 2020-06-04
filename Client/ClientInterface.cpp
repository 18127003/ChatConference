#include "ClientInterface.h"

using namespace MyClient;


System::Void ClientInterface::MainScreen_Load(System::Object^  sender, System::EventArgs^  e)
{
	txtUsername->Text = Client::getObject()->strUsername;
	Client::getObject()->createThreadListenMessageFromServer();
	Client::getObject()->requestListOnlineUsers();
}

System::Void ClientInterface::butSend_Click(System::Object^  sender, System::EventArgs^  e)
{
	Client::getObject()->sendPublicMessage(txtMessage->Text);
	txtMessage->Text = nullptr;
}

System::Void ClientInterface::MainScreen_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	Client::getObject()->logout(); //Logout from server before exit
}

void ClientInterface::AddTextToContent(String^ text)
{
	DisplayChatBox->AppendText(text);
	DisplayChatBox->AppendText("\n");
}

void ClientInterface::UpdateOnlineUsers()
{

}

void ClientInterface::AddAnOnlineUser(String^ username)
{
	lbOnlineUser->Items->Add(username);
}

void ClientInterface::DeleteAnOnlineUser(String^ username)
{
	for (int i = 0; i < lbOnlineUser->Items->Count; ++i)
		if (lbOnlineUser->Items[i]->ToString() == username)
		{
			lbOnlineUser->Items->Remove(lbOnlineUser->Items[i]);
			break;
		}
}

void ClientInterface::SetOnlineUsers(array<String^>^ arrOnlineUsers)
{
	lbOnlineUser->Items->Clear();

	String^ myUsername = Client::getObject()->strUsername;
	for each (String^ user in arrOnlineUsers)
	{
		if (user != myUsername)
			lbOnlineUser->Items->Add(user);
	}
}

System::Void ClientInterface::lbOnlineUser_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	int index = this->lbOnlineUser->IndexFromPoint(e->Location);
	if (index != System::Windows::Forms::ListBox::NoMatches)
	{
		String^ friendUsername = lbOnlineUser->Items[index]->ToString();
		PrivateChannel^ Prf = gcnew PrivateChannel(Client::getObject()->strUsername, friendUsername);
		Client::getObject()->lstPrivateChatForm->Add(Prf);
		Prf->Show();
	}
}

System::Void ClientInterface::butDisconnect_Click(System::Object^  sender, System::EventArgs^  e)
{
	Application::Restart();
}

System::Void ClientInterface::MainScreen_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
{
	Application::Exit();
}