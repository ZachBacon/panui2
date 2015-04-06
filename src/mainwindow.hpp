#ifndef INCLUDED_MAINWINDOW
#define INCLUDED_MAINWINDOW

#include <nall/nall.hpp>
#include <hiro/hiro.hpp>

//if(API::LoadFunction<ptr_CoreGetAPIVersions>(&API::CoreGetAPIVersions, "CoreGetAPIVersions", core))
using namespace nall;
using namespace hiro;

struct HelloHiro : Window
{
    Button btn_load;
    Button btn_windows;
    Button btn_save;
    Button btn_restore;
    Button btn_pauser;
    //Canvas cnv_pauser;
    FixedLayout layout;
    VerticalLayout sublayout;
    
    image img_pause;
    image img_play;
    bool paused;
    
    nall::function<void()> do_play;
    nall::function<void()> do_pause;
    nall::function<void()> do_stop;
    nall::function<void()> do_loadrom;
    
    void setloaded();
    void setunpaused();
    void setpaused();
    void setstopped();
    
    HelloHiro();
};
#endif
