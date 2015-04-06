int initemu(const char * Videoplugin, const char * Audioplugin, const char * Inputplugin, const char * RSPplugin)
{
    void * core = SDL_LoadObject("mupen64plus.dll");
    std::cout << SDL_GetError();
    if(!core)
        return -1;
    
	if(API::LoadFunction<ptr_CoreGetAPIVersions>(&API::CoreGetAPIVersions, "CoreGetAPIVersions", core))
		return -1;
	
    int VersionConfig, VersionDebug, VersionVidext, VersionExtra;
    API::CoreGetAPIVersions(&VersionConfig, &VersionDebug, &VersionVidext, &VersionExtra);
    
	if(API::LoadFunction<ptr_CoreStartup>(&API::CoreStartup, "CoreStartup", core))
        return -1;
    
    API::CoreStartup(0x020000, ".", NULL, (void *)"Core", &debug, NULL, NULL);
    
    if(API::LoadFunction<ptr_CoreAttachPlugin>(&API::CoreAttachPlugin, "CoreAttachPlugin", core))
        return -1;
    
    if(API::LoadFunction<ptr_CoreDetachPlugin>(&API::CoreDetachPlugin, "CoreDetachPlugin", core))
        return -1;
    
    if(API::LoadFunction<ptr_CoreDoCommand>(&API::CoreDoCommand, "CoreDoCommand", core))
        return -1;
        
    if(API::LoadFunction<ptr_DebugSetCallbacks>(&API::DebugSetCallbacks, "DebugSetCallbacks", core))
        return -1;
    API::DebugSetCallbacks(NULL, NULL, NULL);
	
    if(API::LoadFunction<ptr_DebugSetRunState>(&API::DebugSetRunState, "DebugSetRunState", core))
        return -1;
    if(API::LoadFunction<ptr_DebugGetState>(&API::DebugGetState, "DebugGetState", core))
        return -1;
    if(API::LoadFunction<ptr_DebugStep>(&API::DebugStep, "DebugStep", core))
        return -1;
    if(API::LoadFunction<ptr_DebugMemGetPointer>(&API::DebugMemGetPointer, "DebugMemGetPointer", core))
        return -1;
    if(API::LoadFunction<ptr_DebugMemRead32>(&API::DebugMemRead32, "DebugMemRead32", core))
        return -1;
    
    if(!Videoplugin)
        Videoplugin = "mupen64plus-video-glide64mk2.dll";
    if(!Audioplugin)
        Audioplugin = "mupen64plus-audio-sdl.dll";
    if(!Inputplugin)
        Inputplugin = "mupen64plus-input-sdl.dll";
    if(!RSPplugin)
        RSPplugin = "mupen64plus-rsp-hle.dll";
	
    m64p_error err;
    #define LoadPlugin(type) \
    API::type = SDL_LoadObject(type##plugin); \
    std::cout << SDL_GetError(); \
    if(!API::type) \
        return -1; \
     \
    if(API::LoadFunction<ptr_PluginStartup>(&API::type##Startup, "PluginStartup", API::type)) \
    { \
        std::cout << #type " plugin is not a valid m64p plugin (no startup).\n"; \
        return -1; \
    } \
     \
    if(API::LoadFunction<ptr_PluginGetVersion>(&API::type##Version, "PluginGetVersion", API::type)) \
    { \
        std::cout << #type " plugin is not a valid m64p plugin (no version).\n"; \
        return -1; \
    } \
     \
    err = API::type##Startup(core, (void *) #type, &debug); \
    if(err) \
    { \
        std::cout << #type " plugin errored while starting up: " << err << "\n"; \
        return -1; \
    } \
    else \
        std::cout << #type " plugin loaded successfully.\n";
    LoadPlugin(Video);
    LoadPlugin(Audio);
    LoadPlugin(Input);
    LoadPlugin(RSP);
    /*
    // Video
    const char * dllname = "";
    if(argc > 1)
        dllname = argv[1];
    else
        dllname = "mupen64plus-video-glide64mk2.dll";
    API::Video = SDL_LoadObject(dllname);
    std::cout << SDL_GetError();
    if(!API::Video)
        return -1;
    
    if(API::LoadFunction<ptr_PluginStartup>(&API::VideoStartup, "PluginStartup", API::Video))
    {
        std::cout << "Video plugin is not a valid m64p plugin (no startup).";
        return -1;
    }
    
    if(API::LoadFunction<ptr_PluginGetVersion>(&API::VideoVersion, "PluginGetVersion", API::Video))
    {
        std::cout << "Video plugin is not a valid m64p plugin (no version).";
        return -1;
    }
    
    m64p_error err = API::VideoStartup(core, (void *)"Video", &debug);
    if(err)
    {
        std::cout << "Video plugin errored while starting up: " << err;
        return -1;
    }
    
    // Audio
    
    API::Audio = SDL_LoadObject("mupen64plus-audio-sdl.dll");
    std::cout << SDL_GetError();
    if(!API::Audio)
        return -1;
    
    if(API::LoadFunction<ptr_PluginStartup>(&API::AudioStartup, "PluginStartup", API::Audio))
    {
        std::cout << "Audio plugin is not a valid m64p plugin (no startup).";
        return -1;
    }
    
    if(API::LoadFunction<ptr_PluginGetVersion>(&API::AudioVersion, "PluginGetVersion", API::Audio))
    {
        std::cout << "Audio plugin is not a valid m64p plugin (no version).";
        return -1;
    }
    
    err = API::AudioStartup(core, (void *)"Audio", &debug);
    if(err)
    {
        std::cout << "Audio plugin errored while starting up: " << err;
        return -1;
    }
    
    // Input
    
    API::Input = SDL_LoadObject("mupen64plus-Input-sdl.dll");
    std::cout << SDL_GetError();
    if(!API::Input)
        return -1;
    
    if(API::LoadFunction<ptr_PluginStartup>(&API::InputStartup, "PluginStartup", API::Input))
    {
        std::cout << "Input plugin is not a valid m64p plugin (no startup).";
        return -1;
    }
    
    if(API::LoadFunction<ptr_PluginGetVersion>(&API::InputVersion, "PluginGetVersion", API::Input))
    {
        std::cout << "Input plugin is not a valid m64p plugin (no version).";
        return -1;
    }
    
    
    err = API::InputStartup(core, (void *)"Input", &debug);
    if(err)
    {
        std::cout << "Input plugin errored while starting up: " << err;
        return -1;
    }
    
    // RSP
    
    API::RSP = SDL_LoadObject("mupen64plus-rsp-hle.dll");
    std::cout << SDL_GetError();
    if(!API::RSP)
        return -1;
    
    if(API::LoadFunction<ptr_PluginStartup>(&API::RSPStartup, "PluginStartup", API::RSP))
    {
        std::cout << "RSP plugin is not a valid m64p plugin (no startup).";
        return -1;
    }
    
    if(API::LoadFunction<ptr_PluginGetVersion>(&API::RSPVersion, "PluginGetVersion", API::RSP))
    {
        std::cout << "RSP plugin is not a valid m64p plugin (no version).";
        return -1;
    }
    
    err = API::RSPStartup(core, (void *)"RSP", &debug);
    if(err)
    {
        std::cout << "RSP plugin errored while starting up: " << err;
        return -1;
    }*/
    
    std::cout << "UI: Did startup all plugins.\n";
    return 0;
}