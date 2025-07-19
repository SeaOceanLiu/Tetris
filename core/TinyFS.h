#ifndef TinyFSH
#define TinyFSH
#include <vector>
#include <filesystem>

#include <SDL3/SDL.h>

using namespace std;
namespace fs = std::filesystem;

class TinyFS
{
private:
    // vector<int> fileList;
    vector<fs::path> m_recursionList;
    // vector<fs::path> m_result;
    string m_extName;
    string m_orgName;
    string m_appName;
public:
    TinyFS(void):m_extName(""), m_orgName("SeaOcean.Ltd."), m_appName("Tetris"){};
    TinyFS(string appName, string orgName):m_extName(""), m_orgName(orgName), m_appName(appName){}
    vector<fs::path> getFileList(const fs::path dirName, const string& extName="");
// private:
    // typedef SDL_EnumerationResult (SDLCALL *SDL_EnumerateDirectoryCallback)(void *userdata, const char *dirname, const char *fname);
    static enum SDL_EnumerationResult (SDLCALL fileEnumerationResult)(void *userdata, const char *dirname, const char *fname);
};

#endif  // TinyFSH
