//
// Created by jason on 19-10-14.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    while (!isExit) {
        XData d = Read();
        //XLOGD("IDemux Read %d", d.size);
        //if (d.size <= 0)
        //   break;
        if (d.size > 0)
            Notify(d);
    }
}