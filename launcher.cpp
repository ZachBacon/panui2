#include <nall/platform.hpp>
#include <iostream>

char temp[PATH_MAX];

int main()
{
    std::string PATH(getenv("PATH"));
    std::string cwd(getcwd(temp, MAX_PATH));
    #if defined(PLATFORM_WINDOWS)
        putenv((std::string("PATH=")+cwd+"\\bin;"+PATH).c_str());
    #else
        putenv((std::string("PATH=")+cwd+"/bin:"+PATH).c_str());
    #endif
    system("panui.dll");
}
