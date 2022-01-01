// PHZ
// 2018-5-16

#ifndef XOP_H265_SOURCE_H
#define XOP_H265_SOURCE_H

#include "MediaSource.h"
#include "rtp.h"

namespace xop
{

class H265Source : public MediaSource
{
public:
    static H265Source* createNew(uint32_t frameRate=25);
    ~H265Source() override;

    void setFrameRate(uint32_t frameRate)
    { _frameRate = frameRate; }

    uint32_t getFrameRate() const 
    { return _frameRate; }

    std::string getMediaDescription(uint16_t port) override;

    std::string getAttribute() override;

    bool handleFrame(MediaChannelId_t channelId, AVFrame frame) override;

    static uint32_t getTimeStamp();
	
private:
    explicit H265Source(uint32_t frameRate);

    uint32_t _frameRate = 25;
};
	
}

#endif


