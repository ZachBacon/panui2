#include <nall/nall.hpp>
#include <hiro/hiro.hpp>

#include "mainwindow.hpp"
#include "boot.hpp"
#include "loadrom.hpp"

//if(API::LoadFunction<ptr_CoreGetAPIVersions>(&API::CoreGetAPIVersions, "CoreGetAPIVersions", core))
using namespace nall;
using namespace hiro;

void HelloHiro::setloaded()
{
    btn_load.onActivate(do_stop);
    btn_load.setText("Stop Emulation");
}
void HelloHiro::setunpaused()
{
    btn_pauser.setIcon(img_pause);
    btn_pauser.onActivate(do_pause);
}
void HelloHiro::setpaused()
{
    btn_pauser.setIcon(img_play);
    btn_pauser.onActivate(do_play);
}
void HelloHiro::setstopped()
{
    btn_load.onActivate(do_loadrom);
    btn_load.setText("Load ROM");
    corethread = NULL;
}

HelloHiro::HelloHiro()
{
    vanilla_wd = true;
    do_play = [this]()
    {
        if(corethread and this->paused)
        {
            API::CoreDoCommand(M64CMD_RESUME, 0, NULL);
            this->setunpaused();
        }
        else
            std::cout << "UI: No corethread in do_play\n";
    };
    do_pause = [this]()
    {
        if(corethread and !this->paused)
        {
            API::CoreDoCommand(M64CMD_PAUSE, 0, NULL);
            this->setpaused();
        }
        else
            std::cout << "UI: No corethread in do_play\n";
    };
    do_loadrom  = [this]()
    {
        if(romthread or corethread)
            return;
        
        if(!getcwd(working_dir, PATH_MAX))
        {
            vanilla_wd = false;
            std::cout << "UI: Error caching current directory.";
            return;
        }
        
        romname = BrowserDialog().setTitle("Load ROM").setFilters({"n64 roms|*.z64"}).openFile();
        loadrom(romname);
        
        corethread = SDL_CreateThread(bootscript, "BootScript", this);
        SDL_DetachThread(corethread);
		
		auto startthread = SDL_CreateThread(subbootscript, "SubBootScript", this);
        SDL_DetachThread(startthread);
    };
    do_stop = [this]()
    {
        if(corethread)
        {
            API::CoreDoCommand(M64CMD_STOP, 0, NULL);
            API::CoreDoCommand(M64CMD_ROM_CLOSE, 0, NULL);
            m64p_video_mode val = M64VIDEO_NONE;
            API::CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_MODE, &val);
            this->btn_load.setText("Load ROM");
			//if(win_debugger)
			//	delete win_debugger;
        }
        else
            std::cout << "UI: No corethread in do_stop\n";
    };
    setTitle("Panui");
    setSize({10+128+4+80+10, 10+24*3+4*2+10});
    append(layout);
    
    setCentered();
    
    onClose(&Application::quit);
    
    layout.append(btn_load,    Geometry{10        , 10         , 128 , 24});
    layout.append(btn_save,    Geometry{10        , 10+ 24+4   , 64-2, 24});
    layout.append(btn_restore, Geometry{10+64+2   , 10+ 24+4   , 64-2, 24});
    layout.append(btn_windows, Geometry{10        , 10+(24+4)*2, 128 , 24});
    //layout.append(cnv_pauser,  Geometry{10+128+4+8, 10+8       , 64  , 64});
    layout.append(btn_pauser,  Geometry{10+128+4  , 10         , 80  , 80});
    
    img_play.load("play.png");
    img_pause.load("pause.png");
    
    btn_windows.setText("Interfaces");
    btn_save.setText("Save");
    btn_restore.setText("Load");
    
    menubtn_memory.setText("Memory Viewer").onActivate([&]()
    {
        memory.setVisible(true);
    });
    interfaces.append(menubtn_memory);
    
    btn_windows.onActivate([&](){
        interfaces.setVisible();
    });
    //btn_pauser.setFont(Font::sans(24)).setText("❚❚"); //setText("▶");
    //cnv_pauser.setIcon(img_play);
    //btn_pauser.setIcon(img_play);
    //btn_pauser.setBordered(false);
    
    setpaused();
    setstopped();
    
    setVisible();
}
