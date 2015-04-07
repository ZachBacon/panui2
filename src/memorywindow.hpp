#ifndef INCLUDED_MEMORYWINDOW
#define INCLUDED_MEMORYWINDOW

#include <nall/nall.hpp>
#include <hiro/hiro.hpp>

//if(API::LoadFunction<ptr_CoreGetAPIVersions>(&API::CoreGetAPIVersions, "CoreGetAPIVersions", core))
using namespace nall;
using namespace hiro;

struct MemoryWindow : Window
{
	FixedLayout layout;
	TextEdit display;
	LineEdit address;
	
	bool editing;
	Uint32 inputnum;
	MemoryWindow();
};
#endif
