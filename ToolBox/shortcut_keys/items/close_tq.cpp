#include <Windows.h>

#include "common/public/log.h"
#include "../funcs/tq_functions.h"

#include "close_tq.h"

namespace TOOLBOX
{
	bool CloseTQ::call()
	{
		LOG(INFO) << "Start run: " << item_name_;
		TQFunctions::closeTQ();
		return true;
	}

} //namespace TOOLBOX