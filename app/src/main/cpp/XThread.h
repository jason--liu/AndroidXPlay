//
// Created by jason on 19-10-17.
//

#ifndef XPLAY_XTHREAD_H
#define XPLAY_XTHREAD_H

//跨平台sleep
void XSleep(int ms);
class XThread {
public:
    virtual void Start();
    virtual void Stop();
    //入口主函数
    virtual void Main(){

    }

protected:
    bool isExit = false;
    bool isRunning = false;
private:
    void ThreadMain();
};


#endif //XPLAY_XTHREAD_H
