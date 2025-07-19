#ifndef ResourceLoaderH
#define ResourceLoaderH

#include <unordered_map>

#include "nlohmann/json.hpp"
#include "ConstDef.h"
#include "TinyFS.h"

using namespace std;
using json = nlohmann::json;

enum class FontName {
    Asul_Bold,
    Asul_Regular,
    HarmonyOS_Sans_Condensed_Regular,
    HarmonyOS_Sans_Condensed_Thin,
    HarmonyOS_Sans_SC_Black,
    HarmonyOS_Sans_SC_Bold,
    HarmonyOS_Sans_SC_Light,
    HarmonyOS_Sans_SC_Medium,
    HarmonyOS_Sans_SC_Regular,
    HarmonyOS_Sans_SC_Thin,
    MapleMono_NF_CN_Bold,
    MapleMono_NF_CN_BoldItalic,
    MapleMono_NF_CN_ExtraBold,
    MapleMono_NF_CN_ExtraBoldItalic,
    MapleMono_NF_CN_ExtraLight,
    MapleMono_NF_CN_ExtraLightItalic,
    MapleMono_NF_CN_Italic,
    MapleMono_NF_CN_Light,
    MapleMono_NF_CN_LightItalic,
    MapleMono_NF_CN_Medium,
    MapleMono_NF_CN_MediumItalic,
    MapleMono_NF_CN_Regular,
    MapleMono_NF_CN_SemiBold,
    MapleMono_NF_CN_SemiBoldItalic,
    MapleMono_NF_CN_Thin,
    MapleMono_NF_CN_ThinItalic,
    Muyao_Softbrush,
    Quando_Regular
};

class Resource{
public:
    string resourceType;
    size_t resourceSize;
    shared_ptr<char[]>pMem;

    Resource(string type, size_t size, shared_ptr<char[]>p):resourceType(type), resourceSize(size), pMem(p) {}
    // 拷贝构造函数
    Resource(const Resource &r):resourceType(r.resourceType), resourceSize(r.resourceSize), pMem(r.pMem) {}
    // 移动构造函数
    Resource(Resource &&r):resourceType(r.resourceType), resourceSize(r.resourceSize), pMem(r.pMem) {}
    // 拷贝赋值运算符
    Resource& operator=(const Resource &r){
        if(this != &r){
            resourceType = r.resourceType;
            resourceSize = r.resourceSize;
            pMem = r.pMem;
        }
        return *this;
    }
};

class ResourceLoader{
private:
    shared_ptr<char[]>m_pJsonFileContent; // json file content
    size_t m_jsonFileSize; // json file size
    json m_jsonObject; // json object

    string m_resourceVersion;

    unordered_map<string, shared_ptr<Resource>> m_resourceMap;
    shared_ptr<TinyFS> tinyFS;

    float m_totalResourceCount;
    float m_loadedResourceCount;

    bool m_isRewriteNeeded;
    fs::path m_resourceLoadingPath;
    SDL_Thread *m_loadingThread;

    ResourceLoader():m_totalResourceCount(0), m_loadedResourceCount(0), m_isRewriteNeeded(false) {};
    // ~ResourceLoader();
public:
    // 资源类型
    static const string RT_CONFIG;      // 配置文件
    static const string RT_FONTS;       // 字体
    static const string RT_IMAGES;      // 图片
    static const string RT_BACKGROUND;  // 背景
    static const string RT_ANIMATIONS;  // 动画
    static const string RT_MUSIC;       // 背景音乐
    static const string RT_SOUNDS;      // 音效
    static const string RT_SAVED;       // 保存文件

    // 方块配置
    static const string RID_RBlock_jsonc;
    // 字体
    static const string RID_Asul_Bold_ttf;
    static const string RID_HarmonyOS_Sans_SC_Regular_ttf;
    static const string RID_HarmonyOS_Sans_SC_Thin_ttf;
    static const string RID_MapleMono_NF_CN_Regular_ttf;
    static const string RID_Muyao_Softbrush_ttf;
    static const string RID_Quando_Regular_ttf;
    // 图片
    static const string RID_bitmap1_bmp;
    static const string RID_bitmap2_bmp;
    static const string RID_bitmap3_bmp;
    static const string RID_bitmap4_bmp;
    static const string RID_bitmap5_bmp;
    static const string RID_bitmap6_bmp;
    static const string RID_bitmap7_bmp;
    static const string RID_bitmap8_bmp;
    static const string RID_bitmap9_bmp;
    static const string RID_cross_down_png;
    static const string RID_cross_over_png;
    static const string RID_cross_up_png;
    static const string RID_down_png;
    static const string RID_down_hover_png;
    static const string RID_down_pressed_png;
    static const string RID_icon_ico;
    static const string RID_icon_png;
    static const string RID_left_png;
    static const string RID_left_hover_png;
    static const string RID_left_pressed_png;
    static const string RID_pause_png;
    static const string RID_play_png;
    static const string RID_rblock_png;
    static const string RID_right_png;
    static const string RID_right_hover_png;
    static const string RID_right_pressed_png;
    // 背景
    static const string RID_BACKGROUND_IMAGE00_jpg;
    static const string RID_BACKGROUND_IMAGE01_jpg;
    static const string RID_BACKGROUND_IMAGE02_jpg;
    static const string RID_BACKGROUND_IMAGE03_jpg;
    static const string RID_BACKGROUND_IMAGE04_jpg;
    static const string RID_BACKGROUND_IMAGE05_jpg;
    static const string RID_BACKGROUND_IMAGE06_jpg;
    static const string RID_BACKGROUND_IMAGE07_jpg;
    static const string RID_BACKGROUND_IMAGE08_jpg;
    static const string RID_BACKGROUND_IMAGE09_jpg;
    static const string RID_BACKGROUND_IMAGE10_jpg;
    static const string RID_BACKGROUND_IMAGE11_jpg;
    static const string RID_BACKGROUND_IMAGE12_jpg;
    static const string RID_BACKGROUND_IMAGE13_jpg;
    static const string RID_BACKGROUND_IMAGE14_jpg;
    // 旋转动画
    static const string RID_rotate000_png;
    static const string RID_rotate005_png;
    static const string RID_rotate010_png;
    static const string RID_rotate015_png;
    static const string RID_rotate020_png;
    static const string RID_rotate025_png;
    static const string RID_rotate030_png;
    static const string RID_rotate035_png;
    static const string RID_rotate040_png;
    static const string RID_rotate045_png;
    static const string RID_rotate050_png;
    static const string RID_rotate055_png;
    static const string RID_rotate060_png;
    static const string RID_rotate065_png;
    static const string RID_rotate070_png;
    static const string RID_rotate075_png;
    static const string RID_rotate080_png;
    static const string RID_rotate085_png;
    static const string RID_rotate090_png;
    static const string RID_rotate095_png;
    static const string RID_rotate100_png;
    static const string RID_rotate105_png;
    static const string RID_rotate110_png;
    static const string RID_rotate115_png;
    static const string RID_rotate120_png;
    static const string RID_rotate125_png;
    static const string RID_rotate130_png;
    static const string RID_rotate135_png;
    static const string RID_rotate140_png;
    static const string RID_rotate145_png;
    static const string RID_rotate150_png;
    static const string RID_rotate155_png;
    static const string RID_rotate160_png;
    static const string RID_rotate165_png;
    static const string RID_rotate170_png;
    static const string RID_rotate175_png;
    static const string RID_rotate180_png;
    // 背景音乐
    static const string RID_3_am_West_End_wav;
    static const string RID_Beat_One_wav;
    static const string RID_Fright_Night_Twist_wav;
    static const string RID_Goodnightmare_wav;
    static const string RID_Palm_and_Soul_wav;
    static const string RID_Take_the_Ride_wav;
    // 音效
    static const string RID_Bomb_wav;
    static const string RID_BombExplode_wav;
    static const string RID_CantDo_wav;
    static const string RID_Excellent_wav;
    static const string RID_GameOver_wav;
    static const string RID_Go_wav;
    static const string RID_Good_wav;
    static const string RID_LevelComplete_wav;
    static const string RID_Move_wav;
    static const string RID_MultiShot_wav;
    static const string RID_Pierce_wav;
    static const string RID_Save_wav;
    static const string RID_Speedup_wav;
    static const string RID_Warning_wav;
    // 保存文件
    static const string RID_tempSaved_jsonc;

    static unordered_map<FontName, string> m_fontFiles;

    static ResourceLoader* getInstance(void){
        static ResourceLoader instance; // 静态局部变量，程序运行期间只会被初始化一次
        return &instance;
    }

    shared_ptr<Resource> getResource(string& resourceId);
    void loadConfigFile(fs::path configPath);
    void loadConfig(void);
    void getResourcePath(json jsonBLockGroup, string resourceName);
    float getLoadingProgress(void);

    void saveResourceToPrefPath(string resourceId);
    void saveAllResourceToPrefPath(void);
    bool isRewriteNeeded(void){return m_isRewriteNeeded;}

    static int (SDLCALL loadingThread)(void *rlInstance);
    void detachLoadingThread(void);

    SDL_IOStream * openTempSavedFile(char mode);    //  "r" or "w"
    void closeTempSavedFile(SDL_IOStream *fileStream);
};
#endif // ResourceLoaderH