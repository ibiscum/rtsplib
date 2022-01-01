// PHZ
// 2018-6-8

#ifndef XOP_MEDIA_SESSION_H
#define XOP_MEDIA_SESSION_H

#include "AACSource.h"
#include "G711ASource.h"
#include "H264Source.h"
#include "H265Source.h"
#include "MediaSource.h"
#include "media.h"
#include "rtsplib/server/net/RingBuffer.h"
#include "rtsplib/server/net/Socket.h"
#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace xop {

class RtpConnection;

class MediaSession {
public:
  typedef std::function<void(MediaSessionId sessionId, uint32_t numClients)>
      NotifyCallback;

  static MediaSession *createNew(std::string rtspUrlSuffix = " ");
  ~MediaSession();

  bool addMediaSource(MediaChannelId_t channelId, MediaSource *source);
  bool removeMediaSource(MediaChannelId_t channelId);

  bool startMulticast();

  void setNotifyCallback(const NotifyCallback &cb) { _notifyCallback = cb; }

  std::string getRtspUrlSuffix() const { return _suffix; }

  void setRtspUrlSuffix(std::string &suffix) { _suffix = suffix; }

  void setMediaDescribeSDPAddon(std::string sdp_addon) {
    _sdp_addon = std::move(sdp_addon);
  }

  std::string getSdpMessage(const std::string &sessionName = "");
  MediaSource *getMediaSource(MediaChannelId_t channelId);
  bool handleFrame(MediaChannelId_t channelId, AVFrame frame);
  bool addClient(SOCKET rtspfd,
                 const std::shared_ptr<RtpConnection> &rtpConnPtr);
  void removeClient(SOCKET rtspfd);

  MediaSessionId getMediaSessionId() const { return _sessionId; }

  uint32_t getNumClient() const {
    return static_cast<uint32_t>(_clients.size());
  }

  bool isMulticast() const { return _isMulticast; }

  std::string getMulticastIp() const { return _multicastIp; }

  uint16_t getMulticastPort(MediaChannelId_t channelId) const {
    if (channelId >= MAX_MEDIA_CHANNEL)
      return 0;
    return _multicastPort[channelId];
  }

private:
  friend class MediaSource;
  friend class RtspServer;
  explicit MediaSession(std::string rtspUrlSuffix);

  MediaSessionId _sessionId = 0;
  std::string _suffix;
  std::string _sdp;
  std::string _sdp_addon;

  std::vector<std::shared_ptr<MediaSource>> _mediaSources;
  std::vector<RingBuffer<AVFrame>> _buffer;

  NotifyCallback _notifyCallback;
  std::mutex _mutex;
  std::mutex _mtxMap;
  std::map<SOCKET, std::weak_ptr<RtpConnection>> _clients;

  bool _isMulticast = false;
  uint16_t _multicastPort[MAX_MEDIA_CHANNEL]{};
  std::string _multicastIp;

  std::atomic_bool _gopCacheEnabled{}, _hasNewClient{};
  AVFrame _gopCache;

  static std::atomic_uint _lastMediaSessionId;

  const uint8_t kGOP = 200;
};

typedef std::shared_ptr<MediaSession> MediaSessionPtr;

class MulticastAddr {
public:
  static MulticastAddr &instance() {
    static MulticastAddr s_multi_addr;
    return s_multi_addr;
  }

  std::string getAddr() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string addrPtr;
    struct sockaddr_in addr = {0};
    std::random_device rd;
    for (int n = 0; n <= 10; n++) {
      uint32_t range = 0xE8FFFFFF - 0xE8000100;
      addr.sin_addr.s_addr = htonl(0xE8000100 + (rd()) % range);
      addrPtr = inet_ntoa(addr.sin_addr);

      if (m_addrs.find(addrPtr) != m_addrs.end()) {
        addrPtr.clear();
      } else {
        m_addrs.insert(addrPtr);
        break;
      }
    }

    return addrPtr;
  }

  void release(const std::string &addr) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_addrs.erase(addr);
  }

private:
  std::mutex m_mutex;
  std::unordered_set<std::string> m_addrs;
};

} // namespace xop

#endif
