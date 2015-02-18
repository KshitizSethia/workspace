using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Lawnmower
{
    class EventCascader
    {
        public ManualResetEvent eventToNotify;
        public ManualResetEvent[] eventsToWaitFor;

        public EventCascader(ManualResetEvent eventToNotify, ManualResetEvent[] eventsToWaitFor)
        {
            this.eventToNotify = eventToNotify;
            this.eventsToWaitFor = eventsToWaitFor;
        }
    }

}
