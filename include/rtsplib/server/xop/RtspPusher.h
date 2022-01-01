#ifndef XOP_RTSP_PUSHER_H
#define XOP_RTSP_PUSHER_H

#include <mutex>
#include <map>
#include "rtsp.h"

namespace xop
{

class RtspConnection;

class RtspPusher : public Rtsp
{
public:
    explicit RtspPusher(xop::EventLoop *eventLoop);
    ~RtspPusher() override;

    MediaSessionId addMediaSession(MediaSession* session) override;
    int removeMediaSession(MediaSessionId sessionId) override;

    int openUrl(const std::string& url);
    void close();

    bool pushFrame(MediaSessionId sessionId, MediaChannelId_t channelId, AVFrame frame) override;

    bool isConnected() const
    { return (!_connections.empty()); }

private:
    friend class RtspConnection;
    MediaSessionPtr lookMediaSession(MediaSessionId sessionId) override;

    std::shared_ptr<RtspConnection> newConnection(SOCKET sockfd);
    void removeConnection(SOCKET sockfd);

    xop::EventLoop *_eventLoop = nullptr;
    std::mutex _mutex;
    std::map<SOCKET, std::shared_ptr<RtspConnection>> _connections;

    std::shared_ptr<MediaSession> _mediaSessionPtr;
};

}

#endif
