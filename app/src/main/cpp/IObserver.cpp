//
// Created by jason on 19-10-17.
//

#include "IObserver.h"
void IObserver::AddObs(IObserver *obs) {
    if(!obs)
        return;
    std::lock_guard<std::mutex> lock(mux);
    obss.push_back(obs);

}
void IObserver::Notify(XData data) {
    std::lock_guard<std::mutex> lock(mux);
    // vector的size函数每次调用的时候都会统计一般，
    // 当vector数量非常大的时候效率就很低了
    for(int i=0;i<obss.size();i++){
        obss[i]->Update(data);
    }
}