// winform-cppclr-2010-skelton.cpp : main project file.

#include "stdafx.h"
#include "../common/common.h"

#include "FormMain.h"

using namespace DPITesterCppCLR;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	std::wstring dpi;
	getDpiFromCommandLine(&dpi);

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew FormMain(gcnew String(dpi.c_str())));
	return 0;
}
