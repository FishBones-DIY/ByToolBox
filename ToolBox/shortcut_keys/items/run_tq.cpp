#include <Windows.h>

#include "common/public/log.h"
#include "../funcs/tq_functions.h"

#include "run_tq.h"

namespace TOOLBOX
{
	bool RunTQ::call()
	{
		LOG(INFO) << "Start run: " << item_name_;
		TQFunctions::runTQ();
		return true;
	}

} //namespace TOOLBOX