#include "ResourceLoader.h"
const string ResourceLoader::RT_CONFIG      = "config";     // 配置文件
const string ResourceLoader::RT_FONTS       = "fonts";      // 字体
const string ResourceLoader::RT_IMAGES      = "images";     // 图片
const string ResourceLoader::RT_BACKGROUND  = "background"; // 背景
const string ResourceLoader::RT_ANIMATIONS  = "animations"; // 动画
const string ResourceLoader::RT_MUSIC       = "music";      // 背景音乐
const string ResourceLoader::RT_SOUNDS      = "sounds";     // 音效

// 方块配置
const string ResourceLoader::RID_RBlock_jsonc                   = "config/RBlock.jsonc";
// 字体
const string ResourceLoader::RID_Asul_Bold_ttf                  = "fonts/Asul-Bold.ttf";
const string ResourceLoader::RID_HarmonyOS_Sans_SC_Regular_ttf  = "fonts/HarmonyOS_Sans_SC_Regular.ttf";
const string ResourceLoader::RID_HarmonyOS_Sans_SC_Thin_ttf     = "fonts/HarmonyOS_Sans_SC_Thin.ttf";
const string ResourceLoader::RID_MapleMono_NF_CN_Regular_ttf    = "fonts/MapleMono-NF-CN-Regular.ttf";
const string ResourceLoader::RID_Muyao_Softbrush_ttf            = "fonts/Muyao-Softbrush.ttf";
const string ResourceLoader::RID_Quando_Regular_ttf             = "fonts/Quando-Regular.ttf";
// 图片
const string ResourceLoader::RID_cross_down_png                 = "images/cross_down.png";
const string ResourceLoader::RID_cross_over_png                 = "images/cross_over.png";
const string ResourceLoader::RID_cross_up_png                   = "images/cross_up.png";
const string ResourceLoader::RID_down_png                       = "images/down.png";
const string ResourceLoader::RID_down_hover_png                 = "images/down_hover.png";
const string ResourceLoader::RID_down_pressed_png               = "images/down_pressed.png";
const string ResourceLoader::RID_icon_ico                       = "images/icon.ico";
const string ResourceLoader::RID_icon_png                       = "images/icon.png";
const string ResourceLoader::RID_left_png                       = "images/left.png";
const string ResourceLoader::RID_left_hover_png                 = "images/left_hover.png";
const string ResourceLoader::RID_left_pressed_png               = "images/left_pressed.png";
const string ResourceLoader::RID_pause_png                      = "images/pause.png";
const string ResourceLoader::RID_play_png                       = "images/play.png";
const string ResourceLoader::RID_rblock_png                     = "images/rblock.png";
const string ResourceLoader::RID_right_png                      = "images/right.png";
const string ResourceLoader::RID_right_hover_png                = "images/right_hover.png";
const string ResourceLoader::RID_right_pressed_png              = "images/right_pressed.png";
// 背景
const string ResourceLoader::RID_BACKGROUND_IMAGE00_jpg         = "background/2022_1204_15565500_mh1670141573251.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE01_jpg         = "background/IMG_20200903_202237.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE02_jpg         = "background/IMG_20200920_100257.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE03_jpg         = "background/IMG_20200920_100819.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE04_jpg         = "background/IMG_20201002_184914.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE05_jpg         = "background/IMG_20201003_201713.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE06_jpg         = "background/IMG_20201004_084644.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE07_jpg         = "background/IMG_20201004_084801.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE08_jpg         = "background/IMG_20201004_085622.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE09_jpg         = "background/IMG_20201004_183444.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE10_jpg         = "background/IMG_20201018_145828.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE11_jpg         = "background/IMG_20201025_180928.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE12_jpg         = "background/mmexport1599240022745.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE13_jpg         = "background/mmexport1599449092788.jpg";
const string ResourceLoader::RID_BACKGROUND_IMAGE14_jpg         = "background/mmexport1599449096347.jpg";

// 动画配置文件
const string ResourceLoader::RID_rotateBtn_jsonc                = "animations/rotateBtn/rotateBtn.jsonc";
const string ResourceLoader::RID_bombBlock_jsonc                = "animations/bombBlock/bombBlock.jsonc";
const string ResourceLoader::RID_pierceBlock_jsonc              = "animations/pierceBlock/pierceBlock.jsonc";
const string ResourceLoader::RID_cyanBlock_jsonc                = "animations/cyanBlock/cyanBlock.jsonc";
const string ResourceLoader::RID_darkGreenBlock_jsonc           = "animations/darkGreenBlock/darkGreenBlock.jsonc";
const string ResourceLoader::RID_deepBlueBlock_jsonc            = "animations/deepBlueBlock/deepBlueBlock.jsonc";
const string ResourceLoader::RID_greenBlock_jsonc               = "animations/greenBlock/greenBlock.jsonc";
const string ResourceLoader::RID_purpleBlock_jsonc              = "animations/purpleBlock/purpleBlock.jsonc";
const string ResourceLoader::RID_redBlock_jsonc                 = "animations/redBlock/redBlock.jsonc";
const string ResourceLoader::RID_grayBlock_jsonc                = "animations/grayBlock/grayBlock.jsonc";
const string ResourceLoader::RID_yellowBlock_jsonc              = "animations/yellowBlock/yellowBlock.jsonc";

// 动画元素文件
const string ResourceLoader::RID_rotateBtn_svg                  = "animations/rotateBtn/rotateBtn.svg";
const string ResourceLoader::RID_bomb_svg                       = "animations/bombBlock/bomb.svg";
const string ResourceLoader::RID_marker_svg                     = "animations/bombBlock/marker.svg";
const string ResourceLoader::RID_grayBlock_svg                  = "animations/grayBlock/grayBlock.svg";
const string ResourceLoader::RID_chisel_svg                     = "animations/chisel/chisel.svg";
const string ResourceLoader::RID_cyanBlock_svg                  = "animations/cyanBlock/cyanBlock.svg";
const string ResourceLoader::RID_darkGreenBlock_svg             = "animations/darkGreenBlock/darkGreenBlock.svg";
const string ResourceLoader::RID_deepBlueBlock_svg              = "animations/deepBlueBlock/deepBlueBlock.svg";
const string ResourceLoader::RID_greenBlock_svg                 = "animations/greenBlock/greenBlock.svg";
const string ResourceLoader::RID_purpleBlock_svg                = "animations/purpleBlock/purpleBlock.svg";
const string ResourceLoader::RID_redBlock_svg                   = "animations/redBlock/redBlock.svg";
const string ResourceLoader::RID_yellowBlock_svg                = "animations/yellowBlock/yellowBlock.svg";

const string ResourceLoader::RID_lightBand_svg                  = "animations/lightBand/lightBand.svg";

// 背景音乐
const string ResourceLoader::RID_3_am_West_End_wav              = "music/3 am West End.wav";
const string ResourceLoader::RID_Beat_One_wav                   = "music/Beat One.wav";
const string ResourceLoader::RID_Fright_Night_Twist_wav         = "music/Fright Night Twist.wav";
const string ResourceLoader::RID_Goodnightmare_wav              = "music/Goodnightmare.wav";
const string ResourceLoader::RID_Palm_and_Soul_wav              = "music/Palm and Soul.wav";
const string ResourceLoader::RID_Take_the_Ride_wav              = "music/Take the Ride.wav";
// 音效
const string ResourceLoader::RID_Bomb_wav                       = "sounds/Bomb.wav";
const string ResourceLoader::RID_BombExplode_wav                = "sounds/BombExplode.wav";
const string ResourceLoader::RID_CantDo_wav                     = "sounds/CantDo.wav";
const string ResourceLoader::RID_Excellent_wav                  = "sounds/Excellent.wav";
const string ResourceLoader::RID_GameOver_wav                   = "sounds/GameOver.wav";
const string ResourceLoader::RID_Go_wav                         = "sounds/Go.wav";
const string ResourceLoader::RID_Good_wav                       = "sounds/Good.wav";
const string ResourceLoader::RID_LevelComplete_wav              = "sounds/LevelComplete.wav";
const string ResourceLoader::RID_Move_wav                       = "sounds/Move.wav";
const string ResourceLoader::RID_MultiShot_wav                  = "sounds/MultiShot.wav";
const string ResourceLoader::RID_Pierce_wav                     = "sounds/Pierce.wav";
const string ResourceLoader::RID_Save_wav                       = "sounds/Save.wav";
const string ResourceLoader::RID_Speedup_wav                    = "sounds/Speedup.wav";
const string ResourceLoader::RID_Warning_wav                    = "sounds/Warning.wav";

const string ResourceLoader::RID_tempSaved_jsonc                 = "saved/tempSaved.jsonc";

unordered_map<FontName, string> ResourceLoader::m_fontFiles = {
    {FontName::Asul_Bold, ResourceLoader::RID_Asul_Bold_ttf},
    // {FontName::Asul_Regular, "Asul-Regular.ttf"},   // 未使用
    // {FontName::HarmonyOS_Sans_Condensed_Regular, "HarmonyOS_Sans_Condensed_Regular.ttf"},    // 未使用
    // {FontName::HarmonyOS_Sans_Condensed_Thin, "HarmonyOS_Sans_Condensed_Thin.ttf"}, // 未使用
    // {FontName::HarmonyOS_Sans_SC_Black, "HarmonyOS_Sans_SC_Black.ttf"},  // 未使用
    // {FontName::HarmonyOS_Sans_SC_Bold, "HarmonyOS_Sans_SC_Bold.ttf"},    // 未使用
    // {FontName::HarmonyOS_Sans_SC_Light, "HarmonyOS_Sans_SC_Light.ttf"}, // 未使用
    // {FontName::HarmonyOS_Sans_SC_Medium, "HarmonyOS_Sans_SC_Medium.ttf"},   // 未使用
    {FontName::HarmonyOS_Sans_SC_Regular, ResourceLoader::RID_HarmonyOS_Sans_SC_Regular_ttf},
    {FontName::HarmonyOS_Sans_SC_Thin, ResourceLoader::RID_HarmonyOS_Sans_SC_Thin_ttf},
    // {FontName::MapleMono_NF_CN_Bold, "MapleMono-NF-CN-Bold.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_BoldItalic, "MapleMono-NF-CN-BoldItalic.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_ExtraBold, "MapleMono-NF-CN-ExtraBold.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_ExtraBoldItalic, "MapleMono-NF-CN-ExtraBoldItalic.ttf"}, // 未使用
    // {FontName::MapleMono_NF_CN_ExtraLight, "MapleMono-NF-CN-ExtraLight.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_ExtraLightItalic, "MapleMono-NF-CN-ExtraLightItalic.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_Italic, "MapleMono-NF-CN-Italic.ttf"},    // 未使用
    // {FontName::MapleMono_NF_CN_Light, "MapleMono-NF-CN-Light.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_LightItalic, "MapleMono-NF-CN-LightItalic.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_Medium, "MapleMono-NF-CN-Medium.ttf"},    // 未使用
    // {FontName::MapleMono_NF_CN_MediumItalic, "MapleMono-NF-CN-MediumItalic.ttf"},    // 未使用
    {FontName::MapleMono_NF_CN_Regular, ResourceLoader::RID_MapleMono_NF_CN_Regular_ttf},
    // {FontName::MapleMono_NF_CN_SemiBold, "MapleMono-NF-CN-SemiBold.ttf"},    // 未使用
    // {FontName::MapleMono_NF_CN_SemiBoldItalic, "MapleMono-NF-CN-SemiBoldItalic.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_Thin, "MapleMono-NF-CN-Thin.ttf"},   // 未使用
    // {FontName::MapleMono_NF_CN_ThinItalic, "MapleMono-NF-CN-ThinItalic.ttf"},    // 未使用
    {FontName::Muyao_Softbrush, ResourceLoader::RID_Muyao_Softbrush_ttf},
    {FontName::Quando_Regular, ResourceLoader::RID_Quando_Regular_ttf}
};

shared_ptr<Resource> ResourceLoader::getResource(string& resourceId){
    if (m_resourceMap.find(resourceId) != m_resourceMap.end()) {
        return m_resourceMap[resourceId];
    }
    return nullptr;
}

void ResourceLoader::loadConfigFile(fs::path configPath){
    SDL_Log("Open config file: %s", configPath.string().c_str());

    // 将资源版本号初始化为空，避免打开文件失败时获得早前的资源版本号
    m_resourceVersion = "";
    SDL_IOStream *jsonFIleStream = SDL_IOFromFile(configPath.string().c_str(), "r");
    if (jsonFIleStream == nullptr) {
        SDL_Log("Open config file error: %s", SDL_GetError());
        return;
    }

    m_jsonFileSize = SDL_GetIOSize(jsonFIleStream);
    if (m_jsonFileSize <= 0) {
        SDL_Log("Get config file size error: %s", SDL_GetError());
        return;
    }
    m_pJsonFileContent = shared_ptr<char[]>(new char[m_jsonFileSize + 1]);    // json file content
    // m_pJsonFileContent = make_shared<char[]>(m_jsonFileSize + 1);    // json file content
    if (SDL_ReadIO(jsonFIleStream, (void *)m_pJsonFileContent.get(), m_jsonFileSize) != m_jsonFileSize) {
        SDL_Log("Read config file content error: %s", SDL_GetError());
        return;
    }

    SDL_CloseIO(jsonFIleStream);

    m_pJsonFileContent[m_jsonFileSize] = '\0';
    m_jsonObject = json::parse(m_pJsonFileContent.get(), nullptr, false, true);  // json object

    // 获得资源版本号或SHA256值
    m_resourceVersion = m_jsonObject["resourceVersion"].get<string>();
}

void ResourceLoader::loadConfig(void){
    loadConfigFile(ConstDef::workforldPath / ResourceLoader::RT_CONFIG / "config.jsonc");
    // string currentResourceVersion = "";
    string currentResourceVersion = m_resourceVersion;
    SDL_Log("currentResourceVersion: %s", currentResourceVersion.c_str());

    loadConfigFile(ConstDef::pathPrefix / ResourceLoader::RT_CONFIG / "config.jsonc");
    string targetResourceVersion = m_resourceVersion;
    SDL_Log("targetResourceVersion: %s", targetResourceVersion.c_str());
    if (currentResourceVersion != targetResourceVersion){
        SDL_Log("!!!!!!!!!!!!!!!!!!!!Resource version changed, need to overwrite resource!!!!!!!!!!!!!!!!!!!!");

        // 将新的config.jsonc文件保存到工作目录
        if(!SDL_CreateDirectory((ConstDef::workforldPath / ResourceLoader::RT_CONFIG).string().c_str())){
            SDL_Log("Create config directory error: %s", SDL_GetError());
            throw "Create config directory error";
        }
        if(!SDL_SaveFile((ConstDef::workforldPath / ResourceLoader::RT_CONFIG / "config.jsonc").string().c_str(),
            (void *)m_pJsonFileContent.get(),
            m_jsonFileSize)){

            SDL_Log("Save resource to file error: %s", SDL_GetError());
        }
        // 标志需要将资源保存到工作目录
        m_isRewriteNeeded = true;
        SDL_Log("Set resource loading path to pathPrefix");
        // 从pathPrefix目录下加载资源
        m_resourceLoadingPath = ConstDef::pathPrefix;
    } else {
        SDL_Log("--------------------Resource version NO changed, NO need to overwrite resource--------------------");

        SDL_Log("Set resource loading path to workforldPath");
        // 从工作目录加载资源（速度更快）
        m_resourceLoadingPath = ConstDef::workforldPath;
    }

    // 获得方块配置文件路径
    string def = m_jsonObject["blockDefine"].get<string>();
    m_resourceMap[def] = make_shared<Resource>(ResourceLoader::RT_CONFIG, 0, nullptr);

    // 获得字体文件路径
    getResourcePath(m_jsonObject, ResourceLoader::RT_FONTS);
    // 获得图片文件路径
    getResourcePath(m_jsonObject, ResourceLoader::RT_IMAGES);
    // 获得背景文件路径
    getResourcePath(m_jsonObject, ResourceLoader::RT_BACKGROUND);
    // 获得动画文件路径
    getResourcePath(m_jsonObject, ResourceLoader::RT_ANIMATIONS);
    // 获得背景音乐文件路径
    getResourcePath(m_jsonObject, ResourceLoader::RT_MUSIC);
    // 获得音效文件路径
    getResourcePath(m_jsonObject, ResourceLoader::RT_SOUNDS);


    m_totalResourceCount = (float)m_resourceMap.size();
    m_loadedResourceCount = 0;
    m_loadingThread = SDL_CreateThread(&ResourceLoader::loadingThread, "loadingThread", this);
    if(m_loadingThread == nullptr){
        SDL_Log("Create loading thread error: %s", SDL_GetError());
        // throw "Create loading thread error";
        return;
    }
}

void ResourceLoader::getResourcePath(json jsonBLockGroup, string resourceType){
    for(auto& resourceItem : jsonBLockGroup[resourceType]){
        if(!resourceItem.is_string()) {
            SDL_Log("font '%s' item in config file is not string", resourceItem.items().begin().key().c_str());
            // throw "font item in config file is not string";
            return;
        }

        m_resourceMap[resourceItem] = make_shared<Resource>(resourceType, 0, nullptr);
    }
}

float ResourceLoader::getLoadingProgress(void){
    // 除0保护
    if(m_totalResourceCount == 0){
        return 1.0f;
    }

    return (float)m_loadedResourceCount / m_totalResourceCount;
}

int SDLCALL ResourceLoader::loadingThread(void *rlInstance){
    ResourceLoader *rl = (ResourceLoader *)rlInstance;

    fs::path rootPath = rl->m_resourceLoadingPath;
    for(auto kv : rl->m_resourceMap){
        SDL_Log("Open config file: %s", kv.first.c_str());
        SDL_IOStream *resourceFileStream = SDL_IOFromFile((rootPath / kv.first).string().c_str(), "r");
        if (resourceFileStream == nullptr) {
            SDL_Log("Open resource file error: %s", SDL_GetError());
            // throw "Open resource file error";
            return 1;
        }

        size_t iFileLen = SDL_GetIOSize(resourceFileStream);
        if (iFileLen <= 0) {
            SDL_Log("Get resource file size error: %s", SDL_GetError());
            // throw "Get resource file size error";
            return 2;
        }
        shared_ptr<char[]> resourceContent(new char[iFileLen]);
        // shared_ptr<char[]> resourceContent = make_shared<char[]>(iFileLen);
        if (SDL_ReadIO(resourceFileStream, (void *)resourceContent.get(), iFileLen) != iFileLen) {
            SDL_Log("Read resource file content error: %s", SDL_GetError());
            // throw "Read config file content error";
            return 3;
        }
        rl->m_resourceMap[kv.first]->pMem = resourceContent;
        rl->m_resourceMap[kv.first]->resourceSize = iFileLen;

        SDL_CloseIO(resourceFileStream);
        rl->m_loadedResourceCount++;
    }
    return 0;
}

void ResourceLoader::detachLoadingThread(void){
    if (m_loadingThread != nullptr)
    {
        SDL_DetachThread(m_loadingThread);
        m_loadingThread = nullptr;
    }
}

void ResourceLoader::saveResourceToPrefPath(string resourceId){
    if (m_resourceMap.find(resourceId) == m_resourceMap.end() || m_resourceMap[resourceId] == nullptr || m_resourceMap[resourceId]->pMem == nullptr) {
        SDL_Log("Resource %s not found", resourceId.c_str());
        // throw "Resource not found";
        return;
    }

    fs::path directory = ConstDef::workforldPath / resourceId;
    directory = directory.parent_path();
    if(!SDL_CreateDirectory(directory.string().c_str())){
        SDL_Log("Create directory error: %s", SDL_GetError());
        // throw "Create directory error";
        return;
    }

    fs::path filePath = ConstDef::workforldPath / resourceId;
    const void *pMem = (void *)m_resourceMap[resourceId]->pMem.get();
    if(!SDL_SaveFile(filePath.string().c_str(), (void *)pMem, m_resourceMap[resourceId]->resourceSize)){
        SDL_Log("Save resource to file error: %s", SDL_GetError());
        // throw "Save resource to file error";
        return;
    }
    SDL_Log("Saved resource '%s' to file '%s' success!!!!!!", resourceId.c_str(), filePath.string().c_str());
}

void ResourceLoader::saveAllResourceToPrefPath(void){
    for (auto kv : m_resourceMap){
        if (kv.second == nullptr || kv.second->pMem == nullptr) {
            SDL_Log("Resource %s not found", kv.first.c_str());
            // throw "Resource not found";
            continue;
        }
        saveResourceToPrefPath(kv.first);
    }
    // // 方块配置
    // saveResourceToPrefPath(ResourceLoader::RID_RBlock_jsonc);
    // // 字体
    // saveResourceToPrefPath(ResourceLoader::RID_Asul_Bold_ttf);
    // saveResourceToPrefPath(ResourceLoader::RID_HarmonyOS_Sans_SC_Regular_ttf);
    // saveResourceToPrefPath(ResourceLoader::RID_HarmonyOS_Sans_SC_Thin_ttf);
    // saveResourceToPrefPath(ResourceLoader::RID_MapleMono_NF_CN_Regular_ttf);
    // saveResourceToPrefPath(ResourceLoader::RID_Muyao_Softbrush_ttf);
    // saveResourceToPrefPath(ResourceLoader::RID_Quando_Regular_ttf);
    // // 图片
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap1_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap2_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap3_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap4_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap5_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap6_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap7_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap8_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_bitmap9_bmp);
    // saveResourceToPrefPath(ResourceLoader::RID_cross_down_png);
    // saveResourceToPrefPath(ResourceLoader::RID_cross_over_png);
    // saveResourceToPrefPath(ResourceLoader::RID_cross_up_png);
    // saveResourceToPrefPath(ResourceLoader::RID_down_png);
    // saveResourceToPrefPath(ResourceLoader::RID_down_hover_png);
    // saveResourceToPrefPath(ResourceLoader::RID_down_pressed_png);
    // saveResourceToPrefPath(ResourceLoader::RID_icon_ico);
    // saveResourceToPrefPath(ResourceLoader::RID_icon_png);
    // saveResourceToPrefPath(ResourceLoader::RID_left_png);
    // saveResourceToPrefPath(ResourceLoader::RID_left_hover_png);
    // saveResourceToPrefPath(ResourceLoader::RID_left_pressed_png);
    // saveResourceToPrefPath(ResourceLoader::RID_pause_png);
    // saveResourceToPrefPath(ResourceLoader::RID_play_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rblock_png);
    // saveResourceToPrefPath(ResourceLoader::RID_right_png);
    // saveResourceToPrefPath(ResourceLoader::RID_right_hover_png);
    // saveResourceToPrefPath(ResourceLoader::RID_right_pressed_png);
    // // 背景
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE00_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE01_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE02_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE03_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE04_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE05_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE06_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE07_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE08_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE09_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE10_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE11_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE12_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE13_jpg);
    // saveResourceToPrefPath(ResourceLoader::RID_BACKGROUND_IMAGE14_jpg);
    // // 旋转动画
    // saveResourceToPrefPath(ResourceLoader::RID_rotate000_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate005_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate010_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate015_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate020_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate025_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate030_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate035_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate040_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate045_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate050_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate055_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate060_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate065_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate070_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate075_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate080_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate085_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate090_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate095_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate100_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate105_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate110_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate115_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate120_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate125_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate130_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate135_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate140_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate145_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate150_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate155_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate160_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate165_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate170_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate175_png);
    // saveResourceToPrefPath(ResourceLoader::RID_rotate180_png);
    // // 背景音乐
    // saveResourceToPrefPath(ResourceLoader::RID_3_am_West_End_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Beat_One_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Fright_Night_Twist_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Goodnightmare_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Palm_and_Soul_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Take_the_Ride_wav);
    // // 音效
    // saveResourceToPrefPath(ResourceLoader::RID_Bomb_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_BombExplode_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_CantDo_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Excellent_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_GameOver_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Go_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Good_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_LevelComplete_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Move_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_MultiShot_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Pierce_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Save_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Speedup_wav);
    // saveResourceToPrefPath(ResourceLoader::RID_Warning_wav);
}

SDL_IOStream * ResourceLoader::openTempSavedFile(char mode){
    if (mode != 'r' && mode != 'w') {
        throw "ResourceLoader::openSavedFile: Invalid mode";
    }
    fs::path tempSavedFilePath = ConstDef::workforldPath / ResourceLoader::RID_tempSaved_jsonc;
    SDL_Log("Open temp saved file: %s", tempSavedFilePath.string().c_str());

    fs::path directory = tempSavedFilePath.parent_path();
    if(!SDL_CreateDirectory(directory.string().c_str())){
        SDL_Log("Create directory error: %s", SDL_GetError());
        // throw "Create directory error";
        return nullptr;
    }

    char rwMode[2];
    rwMode[0] = mode;
    rwMode[1] = '\0';

    SDL_IOStream *resourceFileStream = SDL_IOFromFile(tempSavedFilePath.string().c_str(), rwMode);
    if (resourceFileStream == nullptr) {
        SDL_Log("Open temp saved file error: %s", SDL_GetError());
        // throw "Open resource file error";
        return nullptr;
    }

    return resourceFileStream;
}

void ResourceLoader::closeTempSavedFile(SDL_IOStream *fileStream){
    if (fileStream == nullptr) {
        return;
    }

    if(!SDL_CloseIO(fileStream)){
        SDL_Log("Close temp saved file error: %s", SDL_GetError());
        // throw "Close resource file error";
    }
}