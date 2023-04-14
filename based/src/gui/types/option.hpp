#pragma once
#include <string>

namespace based::gui
{
	

	class option {
	public:

		explicit option(const char* name);
		virtual void render(const int index);

	protected:

		const char* m_name;
	};

}