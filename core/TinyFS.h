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
public:
    // static FileEnum& getInstance(void){
    //     static FileEnum instance; // 静态局部变量，程序运行期间只会被初始化一次
    //     return instance;
    // }
    TinyFS(void){ m_extName = "";};
    vector<fs::path> getFileList(const fs::path dirName, const string& extName="");
// private:
    // typedef SDL_EnumerationResult (SDLCALL *SDL_EnumerateDirectoryCallback)(void *userdata, const char *dirname, const char *fname);
    static enum SDL_EnumerationResult (SDLCALL fileEnumerationResult)(void *userdata, const char *dirname, const char *fname);
};

#endif  // TinyFSH
