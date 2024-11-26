#include "stdafx.h"
#include "FormMain.h"

using namespace System::Diagnostics;

namespace DPITesterCppCLR {
	System::Void FormMain::FormMain_Load(System::Object^ sender, System::EventArgs^ e)
	{
		timerDpi->Enabled = true;
	}

	System::Void FormMain::timerDpi_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		txtInfo->Text = String::Format(L"SystemDPI={0}, WindowDPI={1}",
			GetDpiForSystem(), GetDpiForWindow((HWND)this->Handle.ToPointer()));
	}
	System::Void FormMain::tsmiLaunchWithCommon_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (sender == tsmiLaunchWithNone)
		{
			Process::Start(Application::ExecutablePath);
		}
		else if (sender == tsmiLaunchWithSystem)
		{
			Process::Start(Application::ExecutablePath, L"-dpi system");
		}
		else if (sender == tsmiLaunchWithPmv2)
		{
			Process::Start(Application::ExecutablePath, L"-dpi pmv2");
		}
		else
		{
			DASSERT(false);
		}
	}
}
