#include <iostream>

#include <SDL2/SDL.h>
#undef main
#include <hiro/hiro.hpp>
#define SDL_DYNLIB
#include <mupen/m64p_common.h>
#include <mupen/m64p_frontend.h>
#include <mupen/m64p_types.h>
#include <mupen/m64p_debugger.h>

#include <atomic>

void debug (void * ctx, int level, const char * msg)
{
    if ( level != M64MSG_VERBOSE )
        std::cout << (const char *)ctx << ": " << msg << "\n";
}

namespace API
{
    ptr_CoreGetAPIVersions CoreGetAPIVersions;
    ptr_CoreStartup CoreStartup;
    ptr_CoreAttachPlugin CoreAttachPlugin;
    ptr_CoreDetachPlugin CoreDetachPlugin;
    ptr_CoreDoCommand CoreDoCommand;
	
    ptr_DebugSetCallbacks DebugSetCallbacks;
    ptr_DebugSetRunState DebugSetRunState;
    ptr_DebugGetState DebugGetState;
	ptr_DebugStep DebugStep;
	ptr_DebugMemGetPointer DebugMemGetPointer;
    ptr_DebugMemRead32 DebugMemRead32;
    
    void * Video;
    ptr_PluginGetVersion VideoVersion; 
    ptr_PluginStartup VideoStartup; 
    
    void * Audio;
    ptr_PluginGetVersion AudioVersion; 
    ptr_PluginStartup AudioStartup; 
    
    void * RSP;
    ptr_PluginGetVersion RSPVersion; 
    ptr_PluginStartup RSPStartup; 
    
    void * Input;
    ptr_PluginGetVersion InputVersion; 
    ptr_PluginStartup InputStartup;
    
	template<typename funcptr>
	bool LoadFunction ( funcptr * function, const char * funcname, void * object )
	{
		*function = (funcptr)SDL_LoadFunction(object, funcname);
		if(!function)
		{
			std::cout << "Could not find reference " << SDL_GetError()
					  << " in "
					  << (object == Video?"Video":
						  object == Audio?"Audio":
						  object == RSP  ?"RSP":
					 	  object == Input?"Input":
						  "Core?")
					  << " dynamic library\n";
			return 1;
		}
		return 0;
	}
	
    uint8_t * romdata;
    unsigned romsize;
}

char working_dir[PATH_MAX];
bool vanilla_wd; // false if working_dir contains a dir to set the wd back to

string romname;
std::atomic<SDL_Thread *> corethread;
std::atomic<SDL_Thread *> romthread;
