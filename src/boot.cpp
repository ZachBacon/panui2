#include "mainwindow.hpp"

int subbootscript( void * ptr )
{
	std::cout << "UI: Started core threads.\n";
	
	auto time = SDL_GetTicks();
    while(API::CoreDoCommand(M64CMD_RESUME, 0, NULL) != M64ERR_SUCCESS and SDL_GetTicks()-time < 2000)
		SDL_Delay(10);
	if(SDL_GetTicks()-time >= 2000)
		std::cout << "UI: Core locked up for two seconds, you're on your own.\n";
	
	if(API::DebugSetRunState(2) == M64ERR_SUCCESS)
	{
		std::cout << "UI: Core seemed to allow setting debug run state.\n";
		if(API::DebugStep() != M64ERR_SUCCESS)
			std::cout << "UI: Failed to finish resuming emulation.\n";
		else
		{
			std::cout << "UI: Finished loading ROM and initializing emulator.\n";
			//((MainWindow*)ptr)->win_debugger->setVisible(true);
			//((MainWindow*)ptr)->win_debugger->visible = true;
		}
	}
	else
		std::cout << "UI: Core does not seem to be built with debugging enabled.\n";
	
	return 0;
}

int bootscript( void * ptr )
{
    if(!ptr)
    {
        std::cout << "UI: Bad window in core boot.\n";
        return 0;
    }
    HelloHiro * mainwin = (HelloHiro *)ptr;
    
    if(romthread)
    {
        std::cout << "UI: Waiting for rom...\n";
        SDL_WaitThread(romthread, NULL); // safe to pass potential NULL (data race) to SDL_WaitThread
        std::cout << "UI: ROM got: " << std::string(romname) << "\n";
    }
    if(romname.equals(""))
    {
        std::cout << "UI: Bad ROM name, leaving boot script.";
        corethread = NULL;
        return 0;
    }
    m64p_error err = API::CoreDoCommand(M64CMD_ROM_OPEN, API::romsize, API::romdata);
    if(err)
    {
        std::cout << "Error loading ROM: " << err;
        corethread = NULL;
        return 0;
    }
    std::cout << "UI: Did load ROM; attaching plugins.\n";
    
    #define LOADPLUGIN(x) \
        m64p_plugin_type x##Type; \
        int x##Version; \
        const char * x##Name; \
        API::x##Version(&x##Type, &x##Version, NULL, &x##Name, NULL); \
        err = API::CoreAttachPlugin(x##Type, API::x); \
        if(err != M64ERR_SUCCESS) \
        { \
            std::cout << #x " plugin errored while attaching: " << err; \
            corethread = NULL; \
            return 0; \
        }
    LOADPLUGIN(Video)
    LOADPLUGIN(Audio)
    LOADPLUGIN(Input)
    LOADPLUGIN(RSP)
    #undef LOADPLUGIN
    std::cout << "UI: Did attach all plugins; running ROM.\n";
    // no more returns until tail of function
    
    mainwin->setloaded();
    mainwin->setunpaused();
	
    API::CoreDoCommand(M64CMD_EXECUTE, 0, NULL);
    std::cout << "UI: Emulation ended.\n";
    API::CoreDoCommand(M64CMD_ROM_CLOSE, 0, NULL);
    std::cout << "UI: Did close ROM.\n";
    
    auto err2 = M64ERR_SUCCESS;
    #define DETACHPLUGIN(x) \
        err = API::CoreDetachPlugin(x##Type); \
        if(err != M64ERR_SUCCESS) \
        { \
            std::cout << "RSP plugin errored while detaching: " << err; \
            err2 = err; \
        }
    DETACHPLUGIN(RSP)
    DETACHPLUGIN(Input)
    DETACHPLUGIN(Audio)
    DETACHPLUGIN(Video)
    #undef DETACHPLUGIN
    //m64p_error CoreDoCommand(m64p_command Command, int ParamInt, void *ParamPtr)
    
    if(err2 == M64ERR_SUCCESS)
        std::cout << "UI: Did detach all plugins; cleaning state, closing thread.\n";
    
    mainwin->setpaused();
    mainwin->setstopped();
    
    std::cout << "UI: Core quit.\n";
    
    return 0;
}
