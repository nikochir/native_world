#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H
/// This file has to be included into an application cpp.
/// In this dll project we only need to include it into engine header

#ifdef CN_ENTRY_POINT

#ifdef CN_PLATFORM_WINDOWS
namespace CN
{
	extern Application* cnCreateApp();
	std::string EXE_DIR;
}


/// Entry point of any application
/// It can be included in cpp file of any project only once
int main(int argc, const char* argv[])
{
	{
		std::string arguments = (std::string)argv[0];
		CN::EXE_DIR = CN_STR_PART_L(arguments, '\\');
		CN_LOG(CN::EXE_DIR);
	}
	auto app = CN::cnCreateApp();
	app->run();
	delete app;
	CN_LOG("Programm is going out");
	CN::MemoryMaster::printMem();
}
#endif


#endif
#endif