// PHZ
// 2018-5-16

#ifndef XOP_G711A_SOURCE_H
#define XOP_G711A_SOURCE_H

#include "MediaSource.h"
#include "rtp.h"

namespace xop
{

class G711ASource : public MediaSource
{
public:
    static G711ASource* createNew();
    ~G711ASource() override;

    uint32_t getSampleRate() const
    { return _sampleRate; }

    uint32_t getChannels() const
    { return _channels; }

    //std::string getMediaDescription(uint16_t port=0) override;
    std::string getMediaDescription(uint16_t port) override;


    std::string getAttribute() override;

    bool handleFrame(MediaChannelId_t channelId, AVFrame frame) override;

    static uint32_t getTimeStamp();

private:
    G711ASource();

    uint32_t _sampleRate = 8000;  
    uint32_t _channels = 1;       
};

}

#endif
