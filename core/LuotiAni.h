#ifndef LuotiAniH
#define LuotiAniH

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>
#include <cmath>
#include <algorithm>

#include <SDL3/SDL.h>
// #include <SDL3_image/SDL_image.h>
// #include <SDL3_ttf/SDL_ttf.h>
#include "nlohmann/json.hpp"
#include "MainWindow.h"
#include "Control.h"
#include "ResourceLoader.h"
#include "Actor.h"

using json = nlohmann::json;

class Operation{
public:
    enum OPERATION_TYPE: uint8_t{
        TRANSLATE,
        SCALE,
        ROTATE,
        OPACITY,
        VISIBLE,
        NULL_OPERATION
    };
    static OPERATION_TYPE strToOperationType(string str){
        if (str == "translate") return OPERATION_TYPE::TRANSLATE;
        else if (str == "scale") return OPERATION_TYPE::SCALE;
        else if (str == "rotate") return OPERATION_TYPE::ROTATE;
        else if (str == "opacity") return OPERATION_TYPE::OPACITY;
        else if (str == "visible") return OPERATION_TYPE::VISIBLE;
        else return OPERATION_TYPE::NULL_OPERATION;
    }
    static string operationTypeToStr(OPERATION_TYPE type){
        switch(type){
            case OPERATION_TYPE::TRANSLATE: return "translate";
            case OPERATION_TYPE::SCALE: return "scale";
            case OPERATION_TYPE::ROTATE: return "rotate";
            case OPERATION_TYPE::OPACITY: return "opacity";
            case OPERATION_TYPE::VISIBLE: return "visible";
            default: return "null_operation";
        }
    }
private:
    OPERATION_TYPE m_type;
    float m_param0;
    float m_param1;
    float m_param2;
public:
    OPERATION_TYPE getType(void) { return m_type; };
    float getP0(void) { return m_param0; };
    float getP1(void) { return m_param1; };
    float getP2(void) { return m_param2; };
    Operation():m_type(OPERATION_TYPE::NULL_OPERATION), m_param0(0), m_param1(0), m_param2(0) {}
    Operation(OPERATION_TYPE type, float p0, float p1=0, float p2=0):m_type(type), m_param0(p0), m_param1(p1), m_param2(p2) {};
    // 赋值构造函数
    Operation(const Operation &other):m_type(other.m_type), m_param0(other.m_param0), m_param1(other.m_param1), m_param2(other.m_param2) {};
    Operation &operator=(const Operation &other){
        if (this != &other){
            m_type = other.m_type;
            m_param0 = other.m_param0;
            m_param1 = other.m_param1;
            m_param2 = other.m_param2;
        }
        return *this;
    }
    // 移动构造函数
    Operation(const Operation &&other):m_type(other.m_type), m_param0(other.m_param0), m_param1(other.m_param1), m_param2(other.m_param2) {};
    Operation &operator=(const Operation &&other){
        if (this != &other){
            m_type = other.m_type;
            m_param0 = other.m_param0;
            m_param1 = other.m_param1;
            m_param2 = other.m_param2;
        }
        return *this;
    }
};
class KeyFrame{
private:
    vector<shared_ptr<Operation>>m_operations;
public:
    KeyFrame(){};
    void addOperation(shared_ptr<Operation>operation){
        m_operations.push_back(operation);
    };
    shared_ptr<Operation> operator[](int index){
        if (index < 0 || index >= (int)m_operations.size()){
            return nullptr;
        }
        return m_operations[index];
    };
    size_t size(void) { return m_operations.size(); };
};
class Layer: public enable_shared_from_this<Layer>{
public:
    enum class LAYER_TYPE: uint8_t{
        IMAGE,
        SHAPE,
        TEXT,
        NULL_LAYER
    };
    static LAYER_TYPE strToLayerType(string str){
        if (str == "image") return LAYER_TYPE::IMAGE;
        else if (str == "shape") return LAYER_TYPE::SHAPE;
        else if (str == "text") return LAYER_TYPE::TEXT;
        else return LAYER_TYPE::NULL_LAYER;
    }
    static string layerTypeToStr(LAYER_TYPE type){
        switch(type){
            case LAYER_TYPE::IMAGE: return "image";
            case LAYER_TYPE::SHAPE: return "shape";
            case LAYER_TYPE::TEXT: return "text";
            default: return "null_layer";
        }
    }
    static uint32_t blendModeStrToSDLBlendMode(string str){
        if (str == "normal") return SDL_BLENDMODE_NONE;
        else if (str == "additive") return SDL_BLENDMODE_ADD;
        else if (str == "additivePremultiplied") return SDL_BLENDMODE_ADD_PREMULTIPLIED;
        else if (str == "modulate") return SDL_BLENDMODE_MOD;
        else if (str == "blend") return SDL_BLENDMODE_BLEND;
        else if (str == "blendPremultiplied") return SDL_BLENDMODE_BLEND_PREMULTIPLIED;
        else if (str == "multiply") return SDL_BLENDMODE_MUL;
        else return SDL_BLENDMODE_NONE;
    }
private:
    string m_name;
    LAYER_TYPE m_type;
    string m_src;           // 图片资源ID，实际上是图片在资源包中的完整路径
    SSize m_size;           // 图片尺寸
    float m_opacity;        // 不透明度，0.0~1.0
    uint32_t m_blendMode;   // 混合模式，normal映射为SDL_BLENDMODE_NONE……

    map<uint32_t, shared_ptr<KeyFrame>>m_keyFrames; // 后面需要按帧号顺序访问，所以使用了map而不是unordered_map
public:
    Layer():
        m_name(""),
        m_type(LAYER_TYPE::NULL_LAYER),
        m_src(""),
        m_size(SSize(0,0)),
        m_opacity(1.0f),
        m_blendMode(SDL_BLENDMODE_NONE)
    {
    }
    shared_ptr<Layer> setName(string name){
        m_name = name;
        return shared_from_this();
    };
    shared_ptr<Layer> setType(LAYER_TYPE type){
        m_type = type;
        return shared_from_this();
    };
    shared_ptr<Layer> setSrc(string src){
        m_src = src;
        return shared_from_this();
    };
    shared_ptr<Layer> setSize(SSize size){
        m_size = size;
        return shared_from_this();
    };
    shared_ptr<Layer> setOpacity(float opacity){
        m_opacity = opacity;
        return shared_from_this();
    };
    shared_ptr<Layer> setBlendMode(uint32_t blendMode){
        m_blendMode = blendMode;
        return shared_from_this();
    };
    shared_ptr<Layer> addKeyFrame(uint32_t frameNumber, shared_ptr<KeyFrame> keyFrame){
        m_keyFrames[frameNumber] = keyFrame;
        return shared_from_this();
    };
    string getName(void) { return m_name; };
    LAYER_TYPE getType(void) { return m_type; };
    string getSrc(void) { return m_src; };
    SSize getSize(void) { return m_size; };
    float getOpacity(void) { return m_opacity; };
    uint32_t getBlendMode(void) { return m_blendMode; };
    shared_ptr<KeyFrame> operator[](uint32_t frameNumber) {
        if (m_keyFrames.find(frameNumber) != m_keyFrames.end()) {
            return m_keyFrames[frameNumber];
        } else {
            return nullptr;
        }
    };
    size_t size(void) { return m_keyFrames.size(); };
    uint32_t nextKeyFrameNumber(uint32_t currentKeyFrame) {
        auto it = m_keyFrames.upper_bound(currentKeyFrame);
        if (it != m_keyFrames.end()) {
            return it->first;
        } else {
            return 0; //没有下一个关键帧了
        }
    };
};

class LuotiAni: public Material{
friend class LuotiAniBuilder;
    class OpData{
    public:
        SRect dRect;
        SPoint translate;
        SMultipleSize m;
        float rotate; // 暂不支持旋转
        SPoint centerPos;
        uint8_t opacity;
        bool visible;

        SDL_Surface *surface; // 源图像，即图层图片资源，不应该被修改

        OpData():dRect(), translate(0,0), m(1, 1), rotate(0), centerPos({0, 0}), opacity(255), visible(true), surface(nullptr) {}
    };
private:
    int m_id;

    Uint64 m_lastFrameMsTick;   // 下一次切换帧的时间戳（毫秒）
    Uint64 m_frameMSDuration;   // 动画每帧的持续时间（毫秒）
    bool m_isLoaded;        // 动画描述是否已加载
    bool m_isPrepared;      // 动画是否已准备好播放
    bool m_isPlaying;       // 动画是否正在播放

    shared_ptr<char[]>m_pJsonFileContent; // json file content
    json m_jsonAniDesc; // json object

    string m_version;   // 动画描述版本号
    string m_name;      // 动画名称
    SSize m_canvasSize; // 画布尺寸
    uint16_t m_frameRate;    // 帧率
    uint32_t m_totalFrames;  // 总帧数
    bool m_loop;        // 是否循环播放

    uint32_t m_frameToDraw;

    vector<shared_ptr<Layer>>m_layers; // 图层列表

    vector<shared_ptr<Actor>>m_frames; // 纹理列表，根据动画描述生成的逐帧纹理

    // 下面矩阵运算可以考虑换成SIMD指令来加速，但SIMD有CPU依赖，等确实有需要时再换
    // 定义2x2矩阵结构
    struct Matrix2D{
        float m[2][2];
    };
    // 创建旋转矩阵
    static Matrix2D createRotationMatrix(float angle) {
        Matrix2D mat;
        float rad = angle * M_PI / 180.0f;
        float cos_angle = cosf(rad);
        float sin_angle = sinf(rad);

        mat.m[0][0] = cos_angle;
        mat.m[0][1] = -sin_angle;
        mat.m[1][0] = sin_angle;
        mat.m[1][1] = cos_angle;

        return mat;
    }
    // 矩阵向量乘法
    static SPoint transformPoint(const Matrix2D *mat, SPoint point) {
        return SPoint{
            mat->m[0][0] * point.x + mat->m[0][1] * point.y,
            mat->m[1][0] * point.x + mat->m[1][1] * point.y
        };
    }

    // 辅助函数：获取像素颜色
    static uint32_t getPixel(SDL_Surface *surface, int x, int y) {
        if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
            return 0; // 返回透明色
        }

        const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
        uint8_t bpp = format_details->bytes_per_pixel;
        uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

        switch (bpp) {
            case 1:
                return *p;
            case 2:
                return *(Uint16 *)p;
            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    return p[0] << 16 | p[1] << 8 | p[2];
                else
                    return p[0] | p[1] << 8 | p[2] << 16;
            case 4:
                return *(uint32_t *)p;
            default:
                return 0;
        }
    }

    // 辅助函数：设置像素颜色
    static void setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel) {
        if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
            return;
        }

        const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
        uint8_t bpp = format_details->bytes_per_pixel;
        uint8_t *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch (bpp) {
            case 1:
                *p = pixel;
                break;
            case 2:
                *(Uint16 *)p = pixel;
                break;
            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } else {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;
            case 4:
                *(uint32_t *)p = pixel;
                break;
        }
    }

    // 双线性插值
    static uint32_t bilinearInterpolation(SDL_Surface *surface, float x, float y) {
        int x1 = std::floor(x);//(int)x;
        int y1 = std::floor(y);//(int)y;
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        // 边界检查
        x1 = std::clamp(x1, 0, surface->w - 1);//SDL_max(0, SDL_min(x1, surface->w - 1));
        y1 = std::clamp(y1, 0, surface->h - 1);//SDL_max(0, SDL_min(y1, surface->h - 1));
        x2 = std::clamp(x2, 0, surface->w - 1);//SDL_max(0, SDL_min(x2, surface->w - 1));
        y2 = std::clamp(y2, 0, surface->h - 1);//SDL_max(0, SDL_min(y2, surface->h - 1));

        // 获取四个邻近像素
        uint32_t p11 = getPixel(surface, x1, y1);
        uint32_t p12 = getPixel(surface, x1, y2);
        uint32_t p21 = getPixel(surface, x2, y1);
        uint32_t p22 = getPixel(surface, x2, y2);

        // 获取颜色分量
        uint8_t r11, g11, b11, a11;
        uint8_t r12, g12, b12, a12;
        uint8_t r21, g21, b21, a21;
        uint8_t r22, g22, b22, a22;

        const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
        SDL_GetRGBA(p11, format_details, nullptr, &r11, &g11, &b11, &a11);
        SDL_GetRGBA(p12, format_details, nullptr, &r12, &g12, &b12, &a12);
        SDL_GetRGBA(p21, format_details, nullptr, &r21, &g21, &b21, &a21);
        SDL_GetRGBA(p22, format_details, nullptr, &r22, &g22, &b22, &a22);

        // 计算插值权重
        float dx = x - x1;
        float dy = y - y1;

        // 对每个颜色通道进行双线性插值，参考https://zhuanlan.zhihu.com/p/77496615/
        float r = (1 - dx) * (1 - dy) * r11 + dx * (1 - dy) * r21 + (1 - dx) * dy * r12 + dx * dy * r22;
        float g = (1 - dx) * (1 - dy) * g11 + dx * (1 - dy) * g21 + (1 - dx) * dy * g12 + dx * dy * g22;
        float b = (1 - dx) * (1 - dy) * b11 + dx * (1 - dy) * b21 + (1 - dx) * dy * b12 + dx * dy * b22;
        float a = (1 - dx) * (1 - dy) * a11 + dx * (1 - dy) * a21 + (1 - dx) * dy * a12 + dx * dy * a22;

        return SDL_MapRGBA(format_details, nullptr, (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a);
    }

    // 旋转Surface并应用双线性插值和反走样
    bool normalRotateSurface(SDL_Surface *src, SDL_Surface *dst, float angle, SPoint centerPos) {
        if (!src || !dst) {
            return false;
        }

        // 锁定表面
        if (!SDL_LockSurface(src) || !SDL_LockSurface(dst)) {
            return false;
        }

        // 将角度转换为弧度
        float rad = -angle * M_PI / 180.0f;
        float cos_angle = cosf(rad);
        float sin_angle = sinf(rad);

        // 反向映射每个目标像素到源图像
        for (int y = 0; y < dst->h; y++) {
            for (int x = 0; x < dst->w; x++) {
                // 转换到旋转前的坐标系
                float src_x = (x - centerPos.x) * cos_angle + (y - centerPos.y) * sin_angle + centerPos.x;
                float src_y = -(x - centerPos.x) * sin_angle + (y - centerPos.y) * cos_angle + centerPos.y;

                // 检查源坐标是否在源图像范围内
                if (src_x >= 0 && src_x < src->w && src_y >= 0 && src_y < src->h) {
                    // 使用双线性插值获取像素值
                    uint32_t pixel = bilinearInterpolation(src, src_x, src_y);
                    setPixel(dst, x, y, pixel);
                }
                // 如果源坐标不在源图像范围内，则不修改目标像素，保持原样
            }
        }

        // 解锁表面
        SDL_UnlockSurface(src);
        SDL_UnlockSurface(dst);

        return true;
    }
    // 旋转Surface并应用三线性插值和反走样
    bool matrixRotateSurface(SDL_Surface *src, SDL_Surface *dst, float angle, SPoint centerPos) {
        if (!src || !dst) {
            return false;
        }

        // 锁定表面
        if (!SDL_LockSurface(src) || !SDL_LockSurface(dst)){
            return false;
        }

        // 创建旋转矩阵
        Matrix2D rotationMat = createRotationMatrix(angle);

        // 反向映射每个目标像素到源图像
        for (int y = 0; y < dst->h; y++) {
            for (int x = 0; x < dst->w; x++) {
                // 计算相对于中心点坐标
                SPoint relCenterPos = SPoint(x, y) - centerPos;

                // 使用旋转矩阵进行反向变换
                SPoint srcRelPos = transformPoint(&rotationMat, relCenterPos);

                // 转换回源图像坐标系
                SPoint srcPos = srcRelPos + centerPos;

                // 检查源坐标是否在源图像范围内
                if (srcPos.x >= 0 && srcPos.x < src->w && srcPos.y >= 0 && srcPos.y < src->h) {
                    // 使用双线性插值获取像素值
                    uint32_t pixel = bilinearInterpolation(src, srcPos.x, srcPos.y);
                    setPixel(dst, x, y, pixel);
                }
                // 如果源坐标不在源图像范围内，则不修改目标像素，保持原样
            }
        }

        // 解锁表面
        SDL_UnlockSurface(src);
        SDL_UnlockSurface(dst);
        return true;
    }

    // GPU加速旋转Surface
    bool gpuRotateSurface(SDL_Surface *src, SDL_Surface *dst, float angle, int center_x, int center_y) {
        if (!src || !dst) {
            return false;
        }

        SDL_Window *hiddenWin = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Texture *src_texture = nullptr;
        SDL_Texture *dst_texture = nullptr;
        SDL_Surface *rendered_surface = nullptr;

        // 通过Lambda表达式定义的finally捕获所有需要释放的局部变量完成退出时的资源清理，参考https://blog.51cto.com/u_13682052/5982850
        auto finalRelease = finally([&]{
            // if (!hiddenWin) {
            //     SDL_DestroyWindow(hiddenWin);
            //     hiddenWin = nullptr;
            // }
            // if (!renderer) {
            //     SDL_DestroyRenderer(renderer);
            //     renderer = nullptr;
            // }
            if (!src_texture) {
                SDL_DestroyTexture(src_texture);
                src_texture = nullptr;
            }
            if (!dst_texture) {
                SDL_DestroyTexture(dst_texture);
                dst_texture = nullptr;
            }
            if (!rendered_surface) {
                SDL_DestroySurface(rendered_surface);
            }
        });

        // hiddenWin = SDL_CreateWindow("hiddenWin", 1, 1, SDL_WINDOW_HIDDEN);
        // if(hiddenWin == nullptr){
        //     SDL_Log("创建窗口失败: %s\n", SDL_GetError());
        //     return false;
        // }
        // // 创建SDL渲染器和纹理
        // renderer = SDL_CreateRenderer(hiddenWin, nullptr);
        // if (renderer == nullptr) {
        //     SDL_Log("创建渲染器失败: %s\n", SDL_GetError());
        //     return false;
        // }

        renderer = MainWindow::getInstance()->getRenderer();
        // 创建源纹理
        src_texture = SDL_CreateTextureFromSurface(renderer, src);
        if (!src_texture) {
            SDL_Log("创建源纹理失败: %s\n", SDL_GetError());
            return false;
        }

        // 创建目标纹理
        dst_texture = SDL_CreateTexture(renderer, src->format, SDL_TEXTUREACCESS_TARGET, dst->w, dst->h);
        if (dst_texture == nullptr) {
            SDL_Log("创建目标纹理失败: %s\n", SDL_GetError());
            return false;
        }

        // 设置渲染目标为目标纹理
        if (!SDL_SetRenderTarget(renderer, dst_texture)) {
            SDL_Log("设置渲染目标失败: %s\n", SDL_GetError());
            return false;
        }

        // 使用透明黑色清空渲染器
        if(!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)){
            SDL_Log("SDL_SetRenderDrawColor failure: %s\n", SDL_GetError());

            return false;
        };
        if(!SDL_RenderClear(renderer)){
            SDL_Log("SDL_RenderClear failure: %s\n", SDL_GetError());
            return false;
        }

        // 计算旋转后的纹理位置，使其中心点与原始中心点一致
        SDL_FRect dst_rect;
        dst_rect.w = (float)src->w;
        dst_rect.h = (float)src->h;
        dst_rect.x = (float)(center_x - src->w / 2);
        dst_rect.y = (float)(center_y - src->h / 2);

        // 设置纹理混合模式，支持透明度
        if(!SDL_SetTextureBlendMode(src_texture, SDL_BLENDMODE_BLEND)){
            SDL_Log("SDL_SetTextureBlendMode failure: %s\n", SDL_GetError());
            return false;
        }

        // 计算旋转中心（相对于纹理）
        SDL_FPoint rotation_center;
        rotation_center.x = (float)(src->w / 2);
        rotation_center.y = (float)(src->h / 2);

        // 使用GPU加速旋转纹理
        if (!SDL_RenderTextureRotated(renderer, src_texture, NULL, &dst_rect, -angle, &rotation_center, SDL_FLIP_NONE)) {
            SDL_Log("旋转纹理失败: %s\n", SDL_GetError());
            return false;
        }

        // 将渲染结果复制回目标surface
        if(!SDL_RenderPresent(renderer)){
            SDL_Log("SDL_RenderPresent failure: %s\n", SDL_GetError());
            return false;
        }

        // 从目标纹理读取像素数据到目标surface
        rendered_surface = SDL_RenderReadPixels(renderer, NULL);
        if (!rendered_surface) {
            SDL_Log("读取像素数据失败: %s\n", SDL_GetError());
            return false;
        }

        // 将渲染结果复制到目标surface
        if(!SDL_BlitSurface(rendered_surface, NULL, dst, NULL)){
            SDL_Log("读取像素数据失败: %s\n", SDL_GetError());
            return false;
        }

        // 恢复渲染目标为屏幕
        if(!SDL_SetRenderTarget(renderer, NULL)){
            SDL_Log("SDL_SetRenderTarget failure: %s\n", SDL_GetError());
            return false;
        }

        return true;
    }

    SDL_Surface *getImageFromResource(string resourceId){
        shared_ptr<Resource> resource = ResourceLoader::getInstance()->getResource(resourceId);
        if (resource == nullptr || (resource->resourceType != ResourceLoader::RT_IMAGES
                                && resource->resourceType != ResourceLoader::RT_ANIMATIONS
                                && resource->resourceType != ResourceLoader::RT_BACKGROUND)
            || resource->pMem == nullptr) {

            SDL_Log("LuotiAni::getImageFromResource Error: '%s' is not an animation related resource.\n", resourceId.c_str());
            throw "LuotiAni::getImageFromResource Error: resource is not an animation related resource.";
        }

        SDL_IOStream *resourceStream = SDL_IOFromConstMem(resource->pMem.get(), resource->resourceSize);
        if( resourceStream == nullptr){
            SDL_Log("LuotiAni::SDL_IOFromConstMem Error: %s\n", SDL_GetError());
            throw "LuotiAni::getImageFromResource Error: SDL_IOFromConstMem failed.";
        }
        SDL_Surface *surface = IMG_Load_IO(resourceStream, true);   // 这里传递了true给closeio参数，所以完成图片载入后，会自动关闭resourceStream
        if (surface == nullptr) {
            SDL_Log("LuotiAni::getImageFromResource Error: %s\n", SDL_GetError());
            throw "LuotiAni::getImageFromResource Error: IMG_Load_IO failed.";
        }
        // SDL_Log("LuotiAni::getImageFromResource: Load image resource '%s' success.\n", resourceId.c_str());
        return surface;
    }

    //将关键帧定义转换为操作数据
    OpData keyFrameToOpData(shared_ptr<KeyFrame> keyFrame, OpData srcOpData){
        if (keyFrame == nullptr) throw "LuotiAni::keyFrameToOpData: No keyframe to convert.";

        OpData opData = srcOpData;

        for (size_t o = 0; o < keyFrame->size(); o++) {
            shared_ptr<Operation> operation = (*keyFrame)[o];
            if (operation == nullptr) continue;

            switch (operation->getType()) {
                case Operation::OPERATION_TYPE::TRANSLATE:
                    opData.translate = opData.translate + SPoint(operation->getP0(), operation->getP1());
                    break;
                case Operation::OPERATION_TYPE::SCALE:
                    opData.m = opData.m * SMultipleSize(operation->getP0(), operation->getP1());
                    break;
                case Operation::OPERATION_TYPE::ROTATE:
                    opData.rotate += operation->getP0();
                    opData.centerPos = {operation->getP1(), operation->getP2()};
                    break;
                case Operation::OPERATION_TYPE::OPACITY:
                    opData.opacity = (uint8_t)(operation->getP0() * 255 / 100);
                    break;
                case Operation::OPERATION_TYPE::VISIBLE:
                    opData.visible = operation->getP0() > 0 ? true : false;
                    break;
                default:
                    break;
            }
        }
        return opData;
    }

    // 创建透明Surface
    SDL_Surface * createSurfaceForLayer(shared_ptr<Layer>layerInfo){
        SDL_Surface *newSurface = SDL_CreateSurface(layerInfo->getSize().width, layerInfo->getSize().height, SDL_PIXELFORMAT_RGBA8888);
        if (newSurface == nullptr) {
            SDL_Log("LuotiAni::prepare: Create scaled image surface error: %s", SDL_GetError());
            return nullptr;
        }
        // 设置图层透明度和混合模式
        if (!SDL_SetSurfaceAlphaMod(newSurface, layerInfo->getOpacity())) {
            SDL_DestroySurface(newSurface);
            SDL_Log("LuotiAni::prepare: Set surface alpha mod error: %s", SDL_GetError());
            return nullptr;
        }
        if (!SDL_SetSurfaceBlendMode(newSurface, (SDL_BlendMode)layerInfo->getBlendMode())) {
            SDL_DestroySurface(newSurface);
            SDL_Log("LuotiAni::prepare: Set surface blend mode error: %s", SDL_GetError());
            return nullptr;
        }
        return newSurface;
    }
public:
    LuotiAni(Control *parent, float xScale=1.0f, float yScale=1.0f):
        Material(parent, xScale, yScale),
        m_pJsonFileContent(nullptr),
        m_jsonAniDesc(nullptr),
        m_version("1.0"),
        m_name(""),
        m_canvasSize({0, 0}),
        m_frameRate(24),
        m_totalFrames(0),
        m_loop(false),
        m_lastFrameMsTick(0),
        m_frameMSDuration(0),
        m_isLoaded(false),
        m_isPrepared(false),
        m_isPlaying(false)
    {
    }
    ~LuotiAni(){};

    void loadFromFile(fs::path filePath) override {
        loadAniDesc(filePath);
    };
    void loadFromResource(string resourceId) override {
        loadAniDesc(resourceId);
    };
    void loadAniDesc(fs::path filePath){
        // SDL_Log("Open json file: %s", filePath.string().c_str());
        SDL_IOStream *jsonFIleStream = SDL_IOFromFile(filePath.string().c_str(), "r");
        if (jsonFIleStream == nullptr) {
            SDL_Log("Open aniDesc json file error: %s", SDL_GetError());
            throw "Open aniDesc json file error";
            return;
        }
        loadFromStream(jsonFIleStream);

        // 在loadFromStream函数内已经关闭了stream
        // SDL_CloseIO(jsonFIleStream);
    }

    void loadAniDesc(string resourceId){
        // SDL_Log("Open json resource: %s", resourceId.c_str());
        shared_ptr<Resource> resource = ResourceLoader::getInstance()->getResource(resourceId);
        if (resource == nullptr || resource->resourceType != ResourceLoader::RT_ANIMATIONS
            || resource->pMem == nullptr) {

            SDL_Log("LoadFromResource Error: '%s' is not a config file\n", resourceId.c_str());
            return;
        }
        SDL_IOStream *jsonResourceStream = SDL_IOFromConstMem(resource->pMem.get(), resource->resourceSize);

        loadFromStream(jsonResourceStream);

        // 在loadFromStream函数内已经关闭了stream
        // SDL_CloseIO(jsonResourceStream);
    }

    void loadFromStream(SDL_IOStream *stream){
        if (stream == nullptr) {
            SDL_Log("LoadFromStream Error: stream is nullptr\n");
            return;
        }
        size_t iFileLen = SDL_GetIOSize(stream);
        if (iFileLen <= 0) {
            SDL_Log("Get aniDesc jsonc file size error: %s", SDL_GetError());
            throw "Get aniDesc jsonc file size error";
            return;
        }

        // 1. 读取文件内容到内存
        m_pJsonFileContent = shared_ptr<char[]>(new char[iFileLen + 1]); // json file content
        if (SDL_ReadIO(stream, (void *)m_pJsonFileContent.get(), iFileLen) != iFileLen) {
            SDL_Log("Read config file content error: %s", SDL_GetError());
            return;
        }
        m_pJsonFileContent[iFileLen] = '\0'; // Null-terminate the content
        SDL_CloseIO(stream);

        // 2. 解析json内容
        m_jsonAniDesc = json::parse(m_pJsonFileContent.get(), nullptr, false, true);  // json object
        // SDL_Log("Animation Description json parsed.");

        json overview = m_jsonAniDesc["overview"];
        if (overview.is_null()) {
            SDL_Log("Animation Description json error: 'overview' section missing.");
            throw "Animation Description json error: 'overview' section missing.";
            return;
        }
        m_name = overview["name"].get<string>();
        m_version = overview["version"].get<string>();
        m_canvasSize.width = overview["view"].at("width").get<float>();
        m_canvasSize.height = overview["view"].at("height").get<float>();
        m_frameRate = overview["frameRate"].get<int>();
        if(m_frameRate == 0) {
            SDL_Log("Animation Description json error: 'frameRate' cannot be zero.");
            throw "Animation Description json error: 'frameRate' cannot be zero.";
            return;
        }
        m_frameMSDuration = 1000 / m_frameRate;
        m_totalFrames = overview["totalFrames"].get<uint32_t>();
        m_loop = overview.at("loop").get<bool>();
        // SDL_Log("Animation Name: %s, Version: %s, Canvas Size: %.2fx%.2f, Frame Rate: %d, Total Frames: %d, Loop: %s",
        //     m_name.c_str(), m_version.c_str(), m_canvasSize.width, m_canvasSize.height,
        //     m_frameRate, m_totalFrames, m_loop?"true":"false");

        // 3. 解析图层
        for (const auto& layerData : m_jsonAniDesc["layers"]) {
            auto layer = make_shared<Layer>();
            layer->setName(layerData.at("name").get<string>())
                ->setType(Layer::strToLayerType(layerData.at("type").get<string>()))
                ->setSrc(layerData.at("src").get<string>())
                ->setSize(SSize(layerData.contains("width") && layerData.contains("height") ?
                                SSize(layerData.at("width").get<float>(), layerData.at("height").get<float>()) :
                                SSize(0, 0)))
                ->setOpacity(layerData.at("opacity").get<float>() / 100.0f) // 转换为0.0~1.0;
                ->setBlendMode(Layer::blendModeStrToSDLBlendMode(layerData.at("blendMode").get<string>()));
            // SDL_Log("Layer: Name: %s is mapping...", layer->getName().c_str());
            // 4. 解析关键帧定义
            for (const auto& keyFrameData : layerData["keyFrames"]) {
                auto keyFrame = make_shared<KeyFrame>();
                uint32_t frameNumber = keyFrameData.at("frame").get<uint32_t>();

                // SDL_Log("Layer(%s) key frame: %u", layer->getName().c_str(), frameNumber);

                auto operationsData = keyFrameData.at("operation");
                for (const auto& operationData : operationsData) {
                    string type = operationData.at("type").get<string>();
                    Operation::OPERATION_TYPE opType = Operation::strToOperationType(type);

                    shared_ptr<Operation> operation = nullptr;
                    switch(opType) {
                        case Operation::OPERATION_TYPE::TRANSLATE:
                            operation = make_shared<Operation>(opType, operationData.at("tx").get<float>(), operationData.at("ty").get<float>());
                            break;
                        case Operation::OPERATION_TYPE::SCALE:
                            operation = make_shared<Operation>(opType, operationData.at("sx").get<float>(), operationData.at("sy").get<float>());
                            break;
                        case Operation::OPERATION_TYPE::ROTATE:
                            operation = make_shared<Operation>(opType, operationData.at("angle").get<float>(), operationData.at("cx").get<float>(), operationData.at("cy").get<float>());
                            break;
                        case Operation::OPERATION_TYPE::OPACITY:
                            operation = make_shared<Operation>(opType, operationData.at("opacity").get<float>());
                            break;
                        case Operation::OPERATION_TYPE::VISIBLE:
                            operation = make_shared<Operation>(opType, operationData.at("visible").get<bool>() ? 1.0f : 0.0f);
                            break;
                        default:
                            SDL_Log("KeyFrame Operation: Unknown operation type: %s", type.c_str());
                            continue; //跳过未知的操作类型
                    }
                    if (operation == nullptr) continue; //保护
                    keyFrame->addOperation(operation);
                    // SDL_Log("KeyFrame Operation: Type: %s, p0: %.2f, p1: %.2f, p2: %.2f",
                    //     type.c_str(), operation->getP0(), operation->getP1(), operation->getP2());
                }
                layer->addKeyFrame(frameNumber, keyFrame);
            }
            m_layers.push_back(layer);
            // SDL_Log("Layer: Name: %s mapped.", layer->getName().c_str());
        }

        // SDL_Log("LuotiAni::loadFromStream: loaded.");
        m_isLoaded = true;
    }


    void update(void) override {
        if (!m_visible) return;
        if (!m_isPrepared || m_frames.empty()) return;

        // 更新动画
        if (m_isPlaying) {
            uint64_t currentTick = SDL_GetTicks();
            uint64_t deltaTick = currentTick - m_lastFrameMsTick;
            if (deltaTick >= m_frameMSDuration) {
                uint32_t m_nextFrameToDraw = (m_frameToDraw + deltaTick / m_frameMSDuration) % m_totalFrames;

                if (m_nextFrameToDraw < m_frameToDraw) {
                    if (!m_loop) {
                        // m_frameToDraw = m_totalFrames - 1;
                        m_frameToDraw = 0;
                        m_isPlaying = false; // 播放结束

                        // 发出动画结束事件
                        triggerEvent(make_shared<Event>(EventName::AnimationEnded, m_id));
                        return;
                    }
                }
                m_frameToDraw = m_nextFrameToDraw;
                m_lastFrameMsTick = currentTick;
            }
        }
    }

    void draw(float x=0, float y=0, Uint8 alpha=SDL_ALPHA_OPAQUE) override{
        draw(m_frameToDraw, x, y, alpha);
    }

    void draw(uint32_t frameNo, float x=0, float y=0, Uint8 alpha=SDL_ALPHA_OPAQUE) {
        if (!m_visible) return;
        if (!m_isPrepared || m_frames.empty()) return;

        m_frames[frameNo]->draw(x, y, alpha);
    }
    // bool handleEvent(shared_ptr<Event> event) override;

    void setRect(SRect rect) override{
        Material::setRect(rect);
        if (m_isPrepared) {
            // 重新调整所有帧显示区域的大小
            // TODO: 实际上可能需要重新生成纹理
            for (size_t f = 0; f < m_frames.size(); f++) {
                m_frames[f]->setRect({0, 0, rect.width, rect.height});
            }
        }
    }

    void play(void){
        if (!m_isPrepared) {
            SDL_Log("LuotiAni::play: Animation not prepared.\n");
            throw "LuotiAni::play: Animation not prepared.";
            return;
        }
        m_frameToDraw = 0;
        m_isPlaying = true;
        m_lastFrameMsTick = SDL_GetTicks();
    }


    void prepare(uint32_t startFrame = 0){
        // SDL_Log("LuotiAni::prepare: Try to preparing animation...\n");
        if (!m_isLoaded) {
            SDL_Log("LuotiAni::prepare: Animation description not loaded.\n");
            throw "LuotiAni::prepare: Animation description not loaded.";
            return;
        }
        if (m_isPrepared) {
            // todo: 重复prepare时，需要释放之前的资源，这里暂不允许重复prepare
            SDL_Log("LuotiAni::prepare: Animation already prepared.\n");
            return;
        }

        m_frameToDraw = startFrame;

        if (m_rect.width <= 0 || m_rect.height <= 0) {
            // 如果显示区域大小未设置，则使用动画描述中的画布尺寸作为显示区域的大小
            m_rect.width = m_canvasSize.width;
            m_rect.height = m_canvasSize.height;
        }
        // Step1成第0帧变换
        // SDL_Log("LuotiAni::Step1 - prepare: Preparing frame 0 for all layers...\n");
        vector<OpData> frameOp; // 缓存单图层每一帧的运算数据
        vector<vector<OpData>> allFrameOp; // 缓存所有图层每一帧的运算数据
        for (uint32_t l = 0; l < m_layers.size(); l++) {
            shared_ptr<Layer> layer = m_layers[l];
            if (layer == nullptr) continue; //保护
            SDL_Surface *operationSurface = nullptr;
            switch( layer->getType() ){
                case Layer::LAYER_TYPE::IMAGE:
                    // 将图片load到一个临时Surface上，待后续根据关键帧信息进行变换
                    operationSurface = getImageFromResource(layer->getSrc());
                    // 设置图层透明度和混合模式
                    // if (!SDL_SetSurfaceAlphaMod(operationSurface, layer->getOpacity())) {
                    //     SDL_DestroySurface(operationSurface);
                    //     SDL_Log("LuotiAni::prepare: Set surface alpha mod error: %s", SDL_GetError());
                    //     throw "LuotiAni::prepare: Set surface alpha mod error";
                    // }
                    // if (!SDL_SetSurfaceBlendMode(operationSurface, (SDL_BlendMode)layer->getBlendMode())) {
                    //     SDL_DestroySurface(operationSurface);
                    //     SDL_Log("LuotiAni::prepare: Set surface blend mode error: %s", SDL_GetError());
                    //     throw "LuotiAni::prepare: Set surface blend mode error";
                    // }

                    // 检查图片是否需要强设大小，需要的话就预先做缩放变换
                    if(layer->getSize().width == 0) {
                        layer->setSize({(float)operationSurface->w, layer->getSize().height});
                    }
                    if(layer->getSize().height == 0) {
                        layer->setSize({layer->getSize().width, (float)operationSurface->h});
                    }
                    if(layer->getSize().width != operationSurface->w || layer->getSize().height != operationSurface->h){
                        // 创建透明Surface
                        SDL_Surface *scaledImageSurface = createSurfaceForLayer(layer);
                        if (scaledImageSurface == nullptr) {
                            SDL_Log("LuotiAni::prepare: Create scaled image surface error: %s", SDL_GetError());
                            SDL_DestroySurface(operationSurface);
                            throw "LuotiAni::prepare: Create scaled image surface error";
                        }
                        if(!SDL_BlitSurfaceScaled(operationSurface, nullptr, scaledImageSurface, nullptr, SDL_SCALEMODE_LINEAR)){
                            SDL_DestroySurface(operationSurface);
                            SDL_DestroySurface(scaledImageSurface);
                            SDL_Log("LuotiAni::prepare: Blit surface error: %s", SDL_GetError());
                            throw("LuotiAni::prepare: Blit surface error");
                        }
                        SDL_DestroySurface(operationSurface);
                        operationSurface = scaledImageSurface;
                    }
                    break;
                case Layer::LAYER_TYPE::SHAPE:
                    // todo: 暂不支持形状图层，这种模式，理论上不使用surface进行操作
                    // 在临时Surface上绘制形状，待后续根据关键帧信息进行变换
                    continue;
                case Layer::LAYER_TYPE::TEXT:
                    // todo: 暂不支持文本图层，这种模式，理论上不使用surface进行操作
                    // 在临时Surface上绘制文本，待后续根据关键帧信息进行变换
                    continue;
                default:
                    continue;
            }
            if (operationSurface == nullptr) {
                SDL_Log("LuotiAni::prepare: No surface for layer %d.\n", l);
                throw "LuotiAni::prepare: No surface for layer.";
                return;
            }

            OpData opData;
            opData.opacity = layer->getOpacity();
            opData.dRect = {0, 0, (float)operationSurface->w, (float)operationSurface->h};
            opData.m = {1, 1};//{getScaleXX(), getScaleYY()};
            opData.surface = operationSurface;

            shared_ptr<KeyFrame> keyFrame = (*layer)[0];
            if (keyFrame == nullptr) throw "LuotiAni::prepare: No keyframe for frame 0.";
            // 从关键帧数据中转换出运算数据
            opData = keyFrameToOpData(keyFrame, opData);
            // SDL_Log("LuotiAni::prepare: Layer %d/%d frame 0 dRect: {%.2f, %.2f, %.2f, %.2f}; translate: {%.2f, %.2f}; scale: {%.2f, %.2f}; rotate:%.2f @ {%.2f, %.2f}; opacity=%d; visible=%s, surface=0x16%X\n",
            //         l+1, (int)m_layers.size(),
            //         opData.dRect.left, opData.dRect.top, opData.dRect.width, opData.dRect.height,
            //         opData.translate.x, opData.translate.y,
            //         opData.m.scaleX, opData.m.scaleY,
            //         opData.rotate, opData.centerPos.x, opData.centerPos.y,
            //         opData.opacity,
            //         opData.visible ? "true" : "false",
            //         opData.surface
            // );

            frameOp.push_back(opData);
            allFrameOp.push_back(frameOp);
            frameOp.clear();
        }

        // Step2 生成后续帧
        // SDL_Log("LuotiAni::Step2 - prepare: Generating frames 1 to %d for all layers...\n", m_totalFrames-1);
        for (uint32_t l = 0; l < m_layers.size(); l++) {
            shared_ptr<Layer> layer = m_layers[l];
            if (layer == nullptr) continue; //保护

            // 取当前layer的所有帧变换
            frameOp = allFrameOp[l];

            uint32_t previousFrameNumber = 0; //frameOp.size() - 1;
            OpData previousOpData = frameOp[previousFrameNumber];

            uint32_t previousKeyFrame = 0;
            uint32_t nextKeyFrame = layer->nextKeyFrameNumber(previousKeyFrame);
            while (nextKeyFrame != 0) {
                shared_ptr<KeyFrame> keyFrame = (*layer)[nextKeyFrame];
                if (keyFrame == nullptr) break; //保护
                // 计算currentKeyFrame帧的运算数据
                OpData opData = keyFrameToOpData(keyFrame, previousOpData);

                // 通过线性插值自动生成上一关键帧与当前关键帧之间的帧数据
                for (uint32_t f = 1; f < nextKeyFrame - previousKeyFrame; f++){
                    OpData autoOpData;
                    float t = (float)f / (nextKeyFrame - previousKeyFrame);

                    autoOpData.dRect        = previousOpData.dRect; // dRect不插值，直接取上一关键帧的值
                    autoOpData.translate.x  = previousOpData.translate.x    + (opData.translate.x   - previousOpData.translate.x)   * t;
                    autoOpData.translate.y  = previousOpData.translate.y    + (opData.translate.y   - previousOpData.translate.y)   * t;
                    autoOpData.m.scaleX     = previousOpData.m.scaleX       + (opData.m.scaleX      - previousOpData.m.scaleX)      * t;
                    autoOpData.m.scaleY     = previousOpData.m.scaleY       + (opData.m.scaleY      - previousOpData.m.scaleY)      * t;
                    autoOpData.rotate       = previousOpData.rotate         + (opData.rotate        - previousOpData.rotate)        * t;
                    autoOpData.centerPos.x  = previousOpData.centerPos.x    + (opData.centerPos.x   - previousOpData.centerPos.x)   * t;
                    autoOpData.centerPos.y  = previousOpData.centerPos.y    + (opData.centerPos.y   - previousOpData.centerPos.y)   * t;
                    autoOpData.opacity      = previousOpData.opacity        + (opData.opacity       - previousOpData.opacity)       * t;
                    autoOpData.visible      = previousOpData.visible; // 可见性不插值，直接取上一关键帧的值
                    autoOpData.surface      = previousOpData.surface; // surface不变

                    // 将插值结果加入到当前layer的帧变换列表中
                    frameOp.push_back(autoOpData);
                    // SDL_Log("LuotiAni::prepare: Layer %d/%d frame %d dRect: {%.2f, %.2f, %.2f, %.2f}; translate: {%.2f, %.2f}; scale: {%.2f, %.2f}; rotate:%.2f @ {%.2f, %.2f}; opacity=%d; visible=%s, surface=0x16%X\n",
                    //         l+1, (int)m_layers.size(), previousFrameNumber + f,
                    //         autoOpData.dRect.left, autoOpData.dRect.top, autoOpData.dRect.width, autoOpData.dRect.height,
                    //         autoOpData.translate.x, autoOpData.translate.y,
                    //         autoOpData.m.scaleX, autoOpData.m.scaleY,
                    //         autoOpData.rotate, autoOpData.centerPos.x, autoOpData.centerPos.y,
                    //         autoOpData.opacity,
                    //         autoOpData.visible ? "true" : "false",
                    //         autoOpData.surface
                    // );
                }
                // 加入当前关键帧数据
                frameOp.push_back(opData);
                // SDL_Log("LuotiAni::prepare: Layer %d/%d frame %d dRect: {%.2f, %.2f, %.2f, %.2f}; translate: {%.2f, %.2f}; scale: {%.2f, %.2f}; rotate:%.2f @ {%.2f, %.2f}; opacity=%d; visible=%s, surface=0x16%X\n",
                //         l+1, (int)m_layers.size(), nextKeyFrame,
                //         opData.dRect.left, opData.dRect.top, opData.dRect.width, opData.dRect.height,
                //         opData.translate.x, opData.translate.y,
                //         opData.m.scaleX, opData.m.scaleY,
                //         opData.rotate, opData.centerPos.x, opData.centerPos.y,
                //         opData.opacity,
                //         opData.visible ? "true" : "false",
                //         opData.surface
                // );

                previousKeyFrame = nextKeyFrame;
                previousOpData = opData;
                // 取下一关键帧
                nextKeyFrame = layer->nextKeyFrameNumber(previousKeyFrame);

            }
            while (previousKeyFrame + 1 < m_totalFrames) {
                // 填充最后一个关键帧之后的帧数据
                frameOp.push_back(previousOpData);
                // SDL_Log("LuotiAni::prepare: Copy layer %d/%d frame %d dRect: {%.2f, %.2f, %.2f, %.2f}; translate: {%.2f, %.2f}; scale: {%.2f, %.2f}; rotate:%.2f @ {%.2f, %.2f}; opacity=%d; visible=%s, surface=0x16%X\n",
                //         l+1, (int)m_layers.size(), previousKeyFrame + 1,
                //         previousOpData.dRect.left, previousOpData.dRect.top, previousOpData.dRect.width, previousOpData.dRect.height,
                //         previousOpData.translate.x, previousOpData.translate.y,
                //         previousOpData.m.scaleX, previousOpData.m.scaleY,
                //         previousOpData.rotate, previousOpData.centerPos.x, previousOpData.centerPos.y,
                //         previousOpData.opacity,
                //         previousOpData.visible ? "true" : "false",
                //         previousOpData.surface
                // );

                previousKeyFrame++;
            }
            // 保存当前layer的所有帧变换
            allFrameOp[l] = frameOp;
        }

        // Step3 根据每一帧的变换数据生成每一帧的纹理
        // SDL_Log("LuotiAni::Step3 - prepare: Generating textures for all frames...\n");
        for (uint32_t f = 0; f < m_totalFrames; f++) {
            // 为每一帧创建一个空白透明画布，尺寸为画布尺寸
            SDL_Surface *canvas = SDL_CreateSurface(m_canvasSize.width, m_canvasSize.height, SDL_PIXELFORMAT_RGBA8888);
            if (canvas == nullptr) {
                SDL_Log("LuotiAni::prepare: Create canvas surface error: %s", SDL_GetError());
                throw "LuotiAni::prepare: Create canvas surface error";
                return;
            }

            // 取每图层的当前帧变换数据，对图层进行变换并绘制到画布上
            for(uint32_t l = 0; l < m_layers.size(); l++) {
                shared_ptr<Layer> layer = m_layers[l];
                if (layer == nullptr) continue; //保护

                OpData opData = allFrameOp[l][f];

                // 可见性最优先做
                if(!opData.visible) {
                    // SDL_Log("LuotiAni::prepare: Layer %d/%d frame %d is not visible, skip drawing this layer.\n", l+1, (int)m_layers.size(), f);
                    continue;
                }

                if (opData.surface == nullptr) continue; //保护
                SDL_Surface *operatingSurface = opData.surface;

                // 旋转变换次优先
                if(opData.rotate != 0) {
                    SDL_Surface *rotatedSurface = createSurfaceForLayer(layer);
                    if (rotatedSurface == nullptr) {
                        SDL_Log("LuotiAni::prepare: Create rotated surface error: %s", SDL_GetError());
                        throw "LuotiAni::prepare: Create rotated surface error";
                    }
                    // if(!normalRotateSurface(operatingSurface, rotatedSurface, opData.rotate, opData.centerPos)){
                    // if(!matrixRotateSurface(operatingSurface, rotatedSurface, opData.rotate, opData.centerPos)){
                    if(!gpuRotateSurface(operatingSurface, rotatedSurface, opData.rotate, opData.centerPos.x, opData.centerPos.y)){
                        SDL_Log("LuotiAni::prepare: rotate surface error.");
                        SDL_DestroySurface(rotatedSurface);
                        throw "LuotiAni::prepare: rotate surface error.";
                    }
                    operatingSurface = rotatedSurface;
                }

                // 再做平移和缩放变换
                SRect dRect = (opData.dRect + opData.translate) * opData.m;
                SDL_Rect sdlRect =  dRect.toSDLRect(); //{ dRect.left, dRect.top, dRect.width, dRect.height };
                // SDL_Log("LuotiAni::prepare: Layer %d/%d frame %d dRect: left=%.2f, top=%.2f, width=%.2f, height=%.2f, surface=0x16%x\n",
                //         l+1, (int)m_layers.size(), f, dRect.left, dRect.top, dRect.width, dRect.height, opData.surface);


                // 最后设置图层透明度和混合模式
                if (!SDL_SetSurfaceAlphaMod(operatingSurface, opData.opacity)) {
                    SDL_Log("LuotiAni::prepare: Set surface alpha mod error: %s", SDL_GetError());
                    throw("LuotiAni::prepare: Set surface alpha mod error");
                }
                if (!SDL_SetSurfaceBlendMode(operatingSurface, (SDL_BlendMode)layer->getBlendMode())) {
                    SDL_Log("LuotiAni::prepare: Set surface blend mode error: %s", SDL_GetError());
                    throw("LuotiAni::prepare: Set surface blend mode error");
                }

                // 将处理后的当前图层贴到画布上
                if(!SDL_BlitSurfaceScaled(operatingSurface, nullptr, canvas, &sdlRect, SDL_SCALEMODE_LINEAR)){
                    SDL_Log("LuotiAni::prepare: Blit surface error: %s", SDL_GetError());
                    throw("LuotiAni::prepare: Blit surface error");
                }
            }
            // 将画布保存为一帧纹理
            shared_ptr<Actor> frame = make_shared<Actor>(this, true);   // 继承父对像的显示区域
            frame->loadTextureFromSurface(canvas);
            m_frames.push_back(frame);

            // 释放画布
            SDL_DestroySurface(canvas);
            canvas = nullptr;
        }
        // 释放各图层临时Surface
        for (uint32_t l = 0; l < m_layers.size(); l++) {
            OpData opData = allFrameOp[l][0];
            if (opData.surface != nullptr) {
                SDL_DestroySurface(opData.surface);
                opData.surface = nullptr;
                // SDL_Log("LuotiAni::prepare: Free operation surface for layer %d done.\n", l+1);
            }
        }

        m_isPrepared = true;
        // SDL_Log("LuotiAni::prepare: Animation prepared done.\n");
    }
    void pause(void){
        m_isPlaying = false;
    }
    void resume(void) {
        if (!m_isPrepared) {
            SDL_Log("LuotiAni::resume: Animation not prepared.\n");
            throw "LuotiAni::resume: Animation not prepared.";
            return;
        }
        m_isPlaying = true;
        m_lastFrameMsTick = SDL_GetTicks();
    }
    bool isPlaying(void) { return m_isPlaying; };
    bool isPrepared(void) { return m_isPrepared; };
    bool isLoaded(void) { return m_isLoaded; };
    uint64_t getFrameDuration(){ return m_frameMSDuration; };
    uint32_t getTotalFrames(void) { return m_totalFrames; };
    uint32_t getCurrentFrame(void) { return m_frameToDraw; };
    bool isLoop(void) { return m_loop; };
    void setLoop(bool loop) { m_loop = loop; };
};

class LuotiAniBuilder{
private:
    shared_ptr<LuotiAni> m_luoAni;
public:
    LuotiAniBuilder(Control *parent, float xScale=1.0f, float yScale=1.0f):
        m_luoAni(nullptr)
    {
        m_luoAni = make_shared<LuotiAni>(parent, xScale, yScale);
    }
    LuotiAniBuilder& loadAniDesc(fs::path filePath){
        m_luoAni->loadAniDesc(filePath);
        return *this;
    }
    LuotiAniBuilder& loadAniDesc(string resourceId){
        m_luoAni->loadAniDesc(resourceId);
        return *this;
    }
    LuotiAniBuilder& loadFromStream(SDL_IOStream *stream){
        m_luoAni->loadFromStream(stream);
        return *this;
    }
    LuotiAniBuilder& setRect(SRect rect){
        m_luoAni->setRect(rect);
        return *this;
    }
    LuotiAniBuilder& prepare(uint32_t startFrame = 0){
        m_luoAni->prepare(startFrame);
        return *this;
    }
    LuotiAniBuilder& setLoop(bool loop){
        m_luoAni->setLoop(loop);
        return *this;
    }
    LuotiAniBuilder& setAutoStart(){
        m_luoAni->play();
        return *this;
    }
    shared_ptr<LuotiAni> build(void){
        return m_luoAni;
    }
};

class LuotiInstance: public Material{
private:
    uint64_t m_userId;
    bool m_isPlaying;
    uint32_t m_frameToDraw;
    uint64_t m_lastFrameMsTick;

    shared_ptr<LuotiAni> m_luoAni;
public:
    LuotiInstance(Control *parent, shared_ptr<LuotiAni> luoAni, uint64_t userId, float xScale=1.0f, float yScale=1.0f):
        Material(parent, xScale, yScale),
        m_userId(userId),
        m_isPlaying(false),
        m_frameToDraw(0),
        m_lastFrameMsTick(SDL_GetTicks()),
        m_luoAni(luoAni)
    {}
    void loadFromFile(fs::path filePath) override {};
    void loadFromResource(string resourceId) override {};
    void update(void) override {
        if (!m_visible) return;
        if (!m_luoAni->isPrepared()) return;

        // 更新动画
        if (m_isPlaying) {
            uint64_t currentTick = SDL_GetTicks();
            uint64_t deltaTick = currentTick - m_lastFrameMsTick;
            if (deltaTick >= m_luoAni->getFrameDuration()) {
                uint32_t m_nextFrameToDraw = (m_frameToDraw + deltaTick / m_luoAni->getFrameDuration()) % m_luoAni->getTotalFrames();

                if (m_nextFrameToDraw < m_frameToDraw) {
                    if (!m_luoAni->isLoop()) {
                        // m_frameToDraw = m_totalFrames - 1;
                        m_frameToDraw = 0;
                        m_isPlaying = false; // 播放结束

                        // 发出动画结束事件
                        triggerEvent(make_shared<Event>(EventName::LuotiInstanceEnded, m_id));
                        return;
                    }
                }
                m_frameToDraw = m_nextFrameToDraw;
                m_lastFrameMsTick = currentTick;
            }
        }
    }

    void draw(float x=0, float y=0, Uint8 alpha=SDL_ALPHA_OPAQUE) override{
        if (!m_visible) return;
        m_luoAni->draw(m_frameToDraw, x, y, alpha);
    }

    void play(void){
        if (!m_luoAni->isPrepared()) {
            SDL_Log("LuotiAni::play: Animation not prepared.\n");
            throw "LuotiAni::play: Animation not prepared.";
            return;
        }
        m_frameToDraw = 0;
        m_isPlaying = true;
        m_lastFrameMsTick = SDL_GetTicks();
    }
    uint64_t getUserId(void) { return m_userId; };
    bool isPlaying(void) { return m_isPlaying; };
};

#endif // LuotiAniH