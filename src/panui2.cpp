#include "global.cpp"
#include "boot.cpp"
#include "loadrom.cpp"
#include "mainwindow.cpp"
#include "memorywindow.cpp"
#include "initemu.cpp"

#include <nall/platform.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    char temp[PATH_MAX];
    std::string PATH(getenv("PATH"));
    std::string cwd(getcwd(temp, MAX_PATH));
    #if defined(PLATFORM_WINDOWS)
        putenv((std::string("PATH=")+cwd+"\\bin;"+PATH).c_str());
    #else
        putenv((std::string("PATH=")+cwd+"/bin:"+PATH).c_str());
    #endif
    
    
    HelloHiro helloHiro;
    if(initemu(0,0,0,0) != 0)
        return 0;
    
    Application::run();
}
