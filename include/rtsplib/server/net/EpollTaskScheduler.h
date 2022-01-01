// PHZ
// 2018-5-15

#ifndef XOP_EPOLL_TASK_SCHEDULER_H
#define XOP_EPOLL_TASK_SCHEDULER_H

#include "TaskScheduler.h"
#include <mutex>
#include <unordered_map>

namespace xop
{	
class EpollTaskScheduler : public TaskScheduler
{
public:
    explicit EpollTaskScheduler(int id);
    ~EpollTaskScheduler() override;

    void updateChannel(ChannelPtr channel) override;
    void removeChannel(ChannelPtr& channel) override;

    // timeout: ms
    bool handleEvent(int timeout) override;

private:
    void update(int operation, ChannelPtr& channel) const;

    int _epollfd = -1;
    std::mutex _mutex;
    std::unordered_map<int, ChannelPtr> _channels;
};

}

#endif
