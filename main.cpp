#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "Z:/CS32/WonkyKong/Assets" or "/Users/fred/cs32/WonkyKong/Assets"

const string assetDirectory = "/Users/christopherrodriguez/Desktop/Project3dsk/WonkyKong/DerivedData/WonkyKong/Build/Products/Debug/Assets";
const int msPerTick = 10;  // 10ms per tick; increase this if game moves too fast

#ifdef _MSC_VER
#include <windows.h>
bool is_directory(string path)
{
    DWORD result = GetFileAttributesA(path.c_str());
    return result != INVALID_FILE_ATTRIBUTES  &&  (result & FILE_ATTRIBUTE_DIRECTORY);
}
#else
#include <sys/stat.h>
bool is_directory(string path)
{
    struct stat statbuf;
    return stat(path.c_str(), &statbuf) == 0  &&  S_ISDIR(statbuf.st_mode);
}
#endif

class GameWorld;

GameWorld* createStudentWorld(string assetPath = "");

int main(int argc, char* argv[])
{
     setenv("DISPLAY", "/private/tmp/com.apple.launchd.OZLMQD3jMN/org.xquartz:0", 1);

    string assetPath = assetDirectory;
    if (!assetPath.empty())
    {
        if (!is_directory(assetPath))
        {
            cout << "Cannot find directory " << assetPath << endl;
            return 1;
        }
        assetPath += '/';
    }
    {
		const string someAsset = "ladder.tga";
		ifstream ifs(assetPath + someAsset);
		if (!ifs)
		{
			cout << "Cannot find " << someAsset << " in ";
			cout << (assetDirectory.empty() ? "current directory" : assetDirectory) << endl;
			return 1;
		}
	}

	GameWorld* gw = createStudentWorld(assetPath);
	Game().run(argc, argv, gw, "Wonky Kong", msPerTick);
}
