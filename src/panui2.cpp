#include "global.cpp"
#include "boot.cpp"
#include "loadrom.cpp"
#include "mainwindow.cpp"
#include "initemu.cpp"

int main(int argc, char *argv[])
{
    /*if(argc > 2)
    {
        std::cout << "UI: Found ROM on command line, loading: " << argv[2] << "\n";
        loadrom(string(argv[2]));
        corethread = SDL_CreateThread(bootscript, "BootScript", w);
    }*/
    
    HelloHiro helloHiro;
    if(initemu(0,0,0,0) != 0)
        return 0;
    //Application::onMain([] { Keyboard::poll(); usleep(5000); });
    
    Application::run();
}
