#include "./EventsCenterImpl.h"






void EventsCenter::addEventListener(IEventListener *listener)
{
	lock_guard<mutex> lk(m_mutex_update);
	list_listeners.push_back(listener);
}
//发送事件

void EventsCenter::sendEvent(EVENT ev1)
{
	mes->notify(ev1);
}
//事件分发,采用类似广播方式，不进行区分消息类型，由接收者自己判断消息类型来处理

void  EventsCenter::eventDispatchLoop()
{
	while (true)
	{
		EVENT newEvent = mes->wait();

		if (isOpen == false)
		{
			break;
		}
		if (newEvent.type >= 0)
		{
			lock_guard<mutex> lk(m_mutex_update);
			bool isContinue = false;
			for (list<IEventListener*>::const_iterator listener = list_listeners.cbegin(); listener != list_listeners.cend(); listener++)
			{
				isContinue = (*listener)->EventHandle(newEvent);
				if (isContinue == false)
				{
					#ifdef EVENT_DATA_VOID
					delete newEvent.data;  
					#endif // EVENT_DATA_VOID

					break;
				}
			}
		}
	}
}
//运行事件中心。
void EventsCenter::Run()
{

	isOpen = true;
	td_dispatch = new thread(&EventsCenter::eventDispatchLoop, this);
	td_dispatch->detach();
}
//关闭事件中心。
void EventsCenter::Close()
{
	isOpen = false;
	delete td_dispatch;
	td_dispatch = nullptr;
}


void EventsCenter::Init(string str)
{
	name = str;
	mes = new messenger<EVENT>();
}

EventsCenter::EventsCenter()
{

}

EventsCenter::~EventsCenter()
{
	delete mes;
}



