#pragma once
#include <tuple>
#include <string>

namespace TQFunctions
{
	bool closeAllTQProcess(bool& have_tq_process) { return true; };
	bool closeSP() { return true; };
	bool closeSPIfOpend() { return true; };
	bool isSPOpend() { return true; };
	bool closeTQ() { return true; };
	bool runTQ() { return true; };
	bool replaceSafeBase() { return true; };

} //namespace TOOLBOX