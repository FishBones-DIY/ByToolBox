#pragma once

#include <string>

#include "common/public/nocopyable.h"

namespace TOOLBOX
{
	class Core : private NoCopyable
	{
	public:
		// ������Ψһ���
		static Core& get()
		{
			static Core core;
			return core;
		}
	
	public:
		bool init();
	
	private:
		void loadShortcutKeyItems();

	protected:
		Core() = default;
		virtual ~Core() = default;

	};
} //namespace TOOLBOX

#define CoreMgr TOOLBOX::Core::get()