#include "common.h"
#include "../../../lsMisc/CommandLineParser.h"
#include "../../../lsMisc/DebugMacro.h"

using namespace Ambiesoft;

bool getDpiFromCommandLine(std::wstring* pDpi)
{
    CCommandLineParser parser;

    parser.AddOption({ L"-dpi" },
        ArgCount::ArgCount_One,
        pDpi,
        ArgEncodingFlags_Default,
        L"DPI type: 'none', 'system', 'pmv2'");

    parser.Parse();

    if (!pDpi->empty())
    {
        if (*pDpi == L"none")
        {

        }
        else if (*pDpi == L"system")
        {
            DVERIFY(SetProcessDPIAware());
        }
        else if (*pDpi == L"pmv2")
        {
            DVERIFY(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2));
        }
        else
        {
            return false;
        }
    }
    return true;
}
