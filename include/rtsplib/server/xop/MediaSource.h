﻿// PHZ
// 2018-6-8

#ifndef XOP_MEDIA_SOURCE_H
#define XOP_MEDIA_SOURCE_H

#include "media.h"
#include "rtp.h"
#include "rtsplib/server/net/Socket.h"
#include <string>
#include <memory>
#include <cstdint>
#include <functional>
#include <map>

namespace xop
{

class MediaSource
{
public:
    typedef std::function<bool (MediaChannelId_t channelId, RtpPacket pkt)> SendFrameCallback;

    MediaSource() = default;
    virtual ~MediaSource() = default;

    virtual MediaType getMediaType() const
    { return _mediaType; }

    virtual std::string getMediaDescription(uint16_t port) = 0;

    virtual std::string getAttribute()  = 0;

    virtual bool handleFrame(MediaChannelId_t channelId, AVFrame frame) = 0;
    virtual void setSendFrameCallback(const SendFrameCallback cb)
    { _sendFrameCallback = cb; }

    virtual uint32_t getPayloadType() const
    { return _payload; }

    virtual uint32_t getClockRate() const
    { return _clockRate; }

protected:
    MediaType _mediaType = NONE;
    uint32_t _payload = 0;
    uint32_t _clockRate = 0;
    SendFrameCallback _sendFrameCallback;
};

}

#endif
