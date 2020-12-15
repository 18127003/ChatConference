#include "PrivateChannel.h"
using namespace MyClient;

System::Void PrivateChannel::ThreadChooseFile()
{
	OpenFileDialog^ ofd = gcnew OpenFileDialog;
	if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ _info = ofd->SafeFileName;
		String^ info = ofd->FileName;
		this->txtfName->Text = _info;
		this->txtPath->Text = info;
		int size = IO::FileInfo(ofd->FileName).Length;

		fileNameToSend = _info;
		filePathToSend = info;
		fileSizeToSend = size;
		Client::getObject()->requestSendFile(strFriendUsername, _info, size);
	}
}

System::Void PrivateChannel::butFile_Click(System::Object^  sender, System::EventArgs^  e) {
	Thread^ newThread = gcnew Thread(gcnew ThreadStart(this, &PrivateChannel::ThreadChooseFile));
	newThread->SetApartmentState(ApartmentState::STA);
	newThread->Start();
}
System::Void PrivateChannel::button1_Click(System::Object^  sender, System::EventArgs^  e) {
}

void PrivateChannel::SetCaption(String^ txtCaption)
{
	this->Text = txtCaption;
}

void PrivateChannel::AddTextToDisplayChatbox(String^ text)
{
	DisplayChatBox->AppendText(text);
	DisplayChatBox->AppendText("\n");
}
System::Void PrivateChannel::butSend_Click(System::Object^  sender, System::EventArgs^  e) {

	Client::getObject()->sendPrivateMessage(strFriendUsername, txtMessage->Text);
	AddTextToDisplayChatbox(strMyUsername + ": " + txtMessage->Text);
	txtMessage->Text = nullptr;
}

System::Void PrivateChannel::PrivateChat_Load(System::Object^  sender, System::EventArgs^  e)
{
	Client::getObject()->createThreadListenMessageFromServer();
}

System::Void PrivateChannel::PrivateChat_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	Client::getObject()->lstPrivateChatForm->Remove(this);
}

System::Void PrivateChannel::PrivateChat_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
{
}
