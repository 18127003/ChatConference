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

System::Void ClientInterface::butFile_Click(System::Object^  sender, System::EventArgs^  e) {
	Thread^ newThread = gcnew Thread(gcnew ThreadStart(this, &ClientInterface::ThreadChooseFile));
	newThread->SetApartmentState(ApartmentState::STA);
	newThread->Start();
}

System::Void ClientInterface::ThreadChooseFile()
{
	OpenFileDialog^ ofd = gcnew OpenFileDialog;
	if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ _info = ofd->SafeFileName;
		String^ info = ofd->FileName;
		//this->txtfName->Text = _info;
		//this->txtPath->Text = info;
		int size = IO::FileInfo(ofd->FileName).Length;

		fileNameToSend = _info;
		filePathToSend = info;
		fileSizeToSend = size;
		//Client::getObject()->requestSendFile(strFriendUsername, _info, size);
		Client::getObject()->sendPublicFile(fileNameToSend, filePathToSend);
	}
}

System::Void ClientInterface::MainScreen_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	Client::getObject()->logout(); //Logout from server before exit
}

void ClientInterface::AddTextToContent(String^ text)
{
	DisplayChatBox->AppendText(text);
	DisplayChatBox->AppendText(Environment::NewLine);
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