//
// Created by jason on 19-10-17.
//

#ifndef XPLAY_IOBSERVER_H
#define XPLAY_IOBSERVER_H

#include <vector>
#include <mutex>
#include "XData.h"
#include "XThread.h"


class IObserver: public XThread {
public:
    virtual void Update(XData data) {
    }
    // 主体函数 添加观察者(thread safety)
    void AddObs(IObserver *obs);
    // 通知所有观察者(thread safety)
    void Notify(XData data);

protected:
    std::vector<IObserver *>obss;
    std::mutex mux;
};


#endif //XPLAY_IOBSERVER_H
