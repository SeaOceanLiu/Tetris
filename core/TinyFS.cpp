#include "TinyFS.h"

vector<fs::path> TinyFS::getFileList(const fs::path dirName, const string& extName){
    vector<fs::path> result;
    /*
    //   for Android not supporting std::filesystem, we have to use SDL_EnumerateDirectory

    if(!fs::exists(dirName)){
        return result;
    } else {
        for (auto &file : fs::directory_iterator(dirName)) {
            if(fs::is_directory(file)){
                auto nextLevel = getFileList(file, extName);
                result.insert(result.end(), nextLevel.begin(), nextLevel.end());
            } else {
                if (extName.length() > 0) {
                    if (file.path().extension().string() == extName) {
                        result.push_back(file);
                    }
                } else {
                    result.push_back(file);
                }
            }
        }
    }
    return result;
    */

    SDL_Log("To SDL_EnumerateDirectory: %s", dirName.string().c_str());
    if (!SDL_EnumerateDirectory(dirName.string().c_str(), &TinyFS::fileEnumerationResult, this)) {
        SDL_Log("SDL_EnumerateDirectory: %s for %s", SDL_GetError(), dirName.string().c_str());
        return result;
    }

    // 走到这里时，m_recursionList已经由回调TinyFS::fileEnumerationResult填写了dirName这一层级中所有符合条件的文件路径，接下来需要递归遍历子目录
    for(auto &file : m_recursionList){
        // if(fs::is_directory(file)){
        //     auto nextLevel = getFileList(file, extName);
        //     result.insert(result.end(), nextLevel.begin(), nextLevel.end());
        // }

        // It seems that SDL_GetPathInfo is not available when the parameter of path is u8string
        // it hits as: "Can't stat: XXXXX", XXXX is the value of path parameter
        SDL_PathInfo pathInfo;
        if(SDL_GetPathInfo(file.string().c_str(), &pathInfo)){
            switch(pathInfo.type){
                case SDL_PATHTYPE_NONE:
                    SDL_Log("SDL_PATHTYPE_NONE: %s", file.string().c_str());
                    break;
                case SDL_PATHTYPE_FILE:
                    result.push_back(file);
                    break;
                case SDL_PATHTYPE_DIRECTORY:
                    {
                        TinyFS fs = TinyFS();
                        auto nextLevel = fs.getFileList(file.string().c_str(), extName);
                        result.insert(result.end(), nextLevel.begin(), nextLevel.end());
                    }
                    break;
                case SDL_PATHTYPE_OTHER:
                    SDL_Log("SDL_PATHTYPE_OTHER: %s", file.string().c_str());
                    break;
                default:
                    break;
            }
        } else {
            SDL_Log("SDL_GetPathInfo: %s", file.string().c_str());
            SDL_Log("SDL_GetPathInfo error: %s", SDL_GetError());
        }
    }

    return result;
}

enum SDL_EnumerationResult SDLCALL TinyFS::fileEnumerationResult(void *userdata, const char *dirname, const char *fname){
    TinyFS *fs = (TinyFS *)userdata;

    fs::path fullPath = fs::u8path(dirname);
    fs::path fileName = fs::u8path(fname);

    SDL_Log("To SDL_EnumerateDirectory: %s(%s)", dirname, fname);

    if(fs->m_extName.length() > 0){
        if (fileName.extension().string() == fs->m_extName) {
            fs->m_recursionList.push_back(fullPath / fileName);
        }
    } else {
        fs->m_recursionList.push_back(fullPath / fileName);
    }
    return  SDL_ENUM_CONTINUE;
}