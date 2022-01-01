﻿// PHZ
// 2018-6-8

#ifndef _RTSP_CONNECTION_H
#define _RTSP_CONNECTION_H

#include "rtsplib/server/net/EventLoop.h"
#include "rtsplib/server/net/TcpConnection.h"
#include "RtpConnection.h"
#include "RtspMessage.h"
#include "rtsp.h"
#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <cstdint>

namespace xop
{

class RtspServer;
class MediaSession;

class RtspConnection : public TcpConnection
{
public:
    typedef std::function<void (SOCKET sockfd)> CloseCallback;

    enum ConnectionMode
    {
        RTSP_SERVER,
        RTSP_PUSHER, 
        //RTSP_CLIENT,
    };

    RtspConnection() = delete;
    RtspConnection(Rtsp *rtspServer, TaskScheduler *taskScheduler, int sockfd);
    ~RtspConnection() override;

    MediaSessionId getMediaSessionId() const
    { return _sessionId; }

    TaskScheduler *getTaskScheduler() const override
    { return _pTaskScheduler; }

    void keepAlive()
    { _aliveCount++; }

    bool isAlive() const
    {
        if (isClosed())
        {
            return false;
        }

        if(_rtpConnPtr != nullptr)
        {
            if(_rtpConnPtr->isMulticast())
                return true;
        }

        return (_aliveCount > 0);
    }

    void resetAliveCount()
    { _aliveCount = 0; }

    int getId() const
    { return _pTaskScheduler->getId(); }

private:
    friend class RtpConnection;
    friend class MediaSession;
    friend class RtspServer;
    friend class RtspPusher;

    bool onRead(BufferReader& buffer);
    void onClose();
    void handleRtcp(SOCKET sockfd);
    static void handleRtcp(BufferReader& buffer);
    void sendMessage(std::shared_ptr<char> buf, uint32_t size);
    bool handleRtspRequest(BufferReader& buffer);
    bool handleRtspResponse(BufferReader& buffer);

    void handleCmdOption();
    void handleCmdDescribe();
    void handleCmdSetup();
    void handleCmdPlay();
    void handleCmdTeardown();
    void handleCmdGetParameter();

    void sendOptions(ConnectionMode mode= RTSP_SERVER);
    void sendDescribe();
    void sendAnnounce();
    void sendSetup();
    void handleRecord();

    std::atomic_int _aliveCount{};

    Rtsp* _pRtsp = nullptr;
    xop::TaskScheduler *_pTaskScheduler = nullptr;
    enum ConnectionMode _connMode = RTSP_SERVER;
    MediaSessionId _sessionId = 0;

    std::shared_ptr<xop::Channel> _rtpChannelPtr;
    std::shared_ptr<RtspRequest> _rtspRequestPtr;
    std::shared_ptr<RtspResponse> _rtspResponsePtr;
    std::shared_ptr<RtpConnection> _rtpConnPtr;
    std::shared_ptr<xop::Channel> _rtcpChannels[MAX_MEDIA_CHANNEL];
};

}

#endif
