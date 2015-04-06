#include "loadrom.hpp"

int loadrom (string arg_romname)
{
    if(!vanilla_wd)
    {
        vanilla_wd = true;
        chdir((const char *)(working_dir));
    }
    
    if(!arg_romname or arg_romname.equals(""))
    {
        std::cout << "UI: Bad romname, returning.\n";
        return -1;
    }
    romname = arg_romname;
    
    const char * cstr_romname = arg_romname.data();
    std::cout << "UI: ROM cstring: " << cstr_romname << "\n";
    file romfile(cstr_romname, file::mode::read);
    
    API::romdata = (uint8_t *)malloc(romfile.size());
    API::romsize = romfile.size();
    
    romfile.read(API::romdata, API::romsize);
    return 0;
}