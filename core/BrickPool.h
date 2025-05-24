#ifndef BrickPoolH
#define BrickPoolH
#include <vector>
#include <string>
#include <filesystem>

#include "SDL3/SDL.h"
#include "Actor.h"

using namespace std;
namespace fs = std::filesystem;

class BrickPool: public ControlImpl
{
private:
    int m_minBrickIdx;
    int m_maxBrickIdx;
    vector<shared_ptr<Actor>> m_bricks;
public:
    BrickPool(Control *parent, fs::path pathPrefix, float xScale=1.0f, float yScale=1.0f);
    ~BrickPool();

    void draw(float x, float y, int brickIdx);
    void draw(SPoint pos, int brickIdx);
    shared_ptr<Actor> getBrick(int brickIdx);
    int getBrickCount(void);
    int getMinBrickIdx(void);
    int getMaxBrickIdx(void);
};


#endif  // BrickPoolH