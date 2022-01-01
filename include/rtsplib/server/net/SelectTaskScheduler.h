// PHZ
// 2018-5-15

#ifndef XOP_SELECT_TASK_SCHEDULER_H
#define XOP_SELECT_TASK_SCHEDULER_H

#include "TaskScheduler.h"
#include <mutex>
#include <unordered_map>

#if defined(__linux) || defined(__linux__) 
#include <sys/select.h>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#endif

#if defined(WIN32) || defined(_WIN32) 
#include "Socket.h"
#endif

namespace xop
{	

class SelectTaskScheduler : public TaskScheduler
{
public:
    explicit SelectTaskScheduler(int id);
    ~SelectTaskScheduler() override;

    void updateChannel(ChannelPtr channel) override;
    void removeChannel(ChannelPtr& channel) override;
    bool handleEvent(int timeout) override;
	
private:
    fd_set _fdReadBackup{};
    fd_set _fdWriteBackup{};
    fd_set _fdExpBackup{};
    int _maxfd = 0;

    bool _isfdReadReset = false;
    bool _isfdWriteReset = false;
    bool _isfdExpReset = false;

    std::mutex _mutex;
    std::unordered_map<int, ChannelPtr> _channels;
};

}

#endif
