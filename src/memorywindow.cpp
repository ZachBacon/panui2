#include <nall/nall.hpp>
#include <hiro/hiro.hpp>

#include "mainwindow.hpp"
#include "boot.hpp"
#include "loadrom.hpp"

//if(API::LoadFunction<ptr_CoreGetAPIVersions>(&API::CoreGetAPIVersions, "CoreGetAPIVersions", core))
using namespace nall;
using namespace hiro;
MemoryWindow::MemoryWindow()
{
	auto font = Font::monospace(10);
	
	auto prefixsize = Font::size(font, "0x");
	auto delimsize = Font::size(font, ": ");
	auto wordsize = Font::size(font, "12345678");
	auto chunksize = Font::size(font, "12345678 12345678 12345678 12345678");
	
	auto viewwidth = prefixsize.width()+wordsize.width()+delimsize.width()+chunksize.width()+6+24;
	auto entrywidth = wordsize.width()+prefixsize.width()+6;
	auto entryheight = wordsize.height()+6;
	auto viewheight = wordsize.height()*0x30+6;
    
	setTitle("Memory");
	setSize({viewwidth+20, entryheight + 30 + viewheight});
	append(layout);
	
	auto update = [this]()
	{
		auto size = sizeof(Uint32);
		if(this->address.text().length() <= 8 or (this->address.text().beginsWith("0x") and this->address.text().length() <= 10))
			inputnum = hex(this->address.text());
		else
		{
			puts("UI: Invalid address input in memory viewer.");
			return;
		}
		inputnum = inputnum/size*size;
		
		auto numwide = 4;
		auto sizewide = numwide*size;
		auto sizetall = 0x30;
		
		nall::string displaytext("");
		
		//Uint32 buffer;
		for (unsigned j = 0; j < sizetall*sizewide; j += sizewide)
		{
			displaytext.append("0x", hex<8, '0'>(inputnum+j), ": ");
			for (unsigned i = 0; i < sizewide; i += size)
			{
				displaytext.append(hex<8, '0'>(API::DebugMemRead32(inputnum+i+j)));
				if(i+size < sizewide)
					displaytext.append(" ");
			}
			if(j+sizewide < sizetall*sizewide)
				displaytext.append("\n");
		}
		display.setText(displaytext);
		return;
	};
	display.setFont(font);
	address.setFont(font);
	address.setText("0x80000000");
	address.onChange(update);
	
	layout.append(address, Geometry{10, 10            , entrywidth, entryheight});
	layout.append(display, Geometry{10, entryheight+20, viewwidth , viewheight });
	
	setVisible(false);
}
