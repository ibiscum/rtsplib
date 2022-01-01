// PHZ
// 2018-6-8

#ifndef XOP_RTSP_SERVER_H
#define XOP_RTSP_SERVER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <mutex>
#include "rtsplib/server/net/TcpServer.h"
#include "rtsp.h"

namespace xop
{

class RtspConnection;

class RtspServer : public Rtsp, public TcpServer
{
public:
    RtspServer(xop::EventLoop *loop, const std::string& ip, uint16_t port = 554);
    ~RtspServer() override;

    MediaSessionId addMediaSession(MediaSession* session) override;
    int removeMediaSession(MediaSessionId sessionId) override;

    bool pushFrame(MediaSessionId sessionId, MediaChannelId_t channelId, AVFrame frame) override;

private:
    friend class RtspConnection;
    MediaSessionPtr lookMediaSession(const std::string& suffix) override;
    MediaSessionPtr lookMediaSession(MediaSessionId sessionId) override;

    TcpConnection::Ptr newConnection(SOCKET sockfd) override;

    std::mutex _mtxSessionMap;
    std::unordered_map<MediaSessionId, std::shared_ptr<MediaSession>> _mediaSessions;
    std::unordered_map<std::string, MediaSessionId> _rtspSuffixMap;
};

}

#endif

