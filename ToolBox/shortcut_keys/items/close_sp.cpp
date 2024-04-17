#include <Windows.h>

#include "common/public/log.h"
#include "../funcs/tq_functions.h"

#include "close_sp.h"

namespace TOOLBOX
{
	bool CloseSP::call()
	{
		LOG(INFO) << "Start run: " << item_name_;
		TQFunctions::closeSPIfOpend();
		return true;
	}

} //namespace TOOLBOX