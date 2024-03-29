#include "./EventsCenterImpl.h"


#include <chrono>



void EventsCenter::addEventListener(IEventListener *listener)
{
	lock_guard<mutex> lk(m_mutex_update);
	list_listeners.push_back(listener);
}
//发送事件

void EventsCenter::sendEvent(EVENT ev1)
{
	if (ev1.type >= 0)
	{
		if(ev1.data!=nullptr)
			ev1.data->createTime = chrono::time_point_cast<chrono::microseconds>(chrono::steady_clock::now());
	}
	mes.notify(ev1);
}
//事件分发,采用类似广播方式，不进行区分消息类型，由接收者自己判断消息类型来处理
/*
void  EventsCenter::eventDispatchLoop()
{
	while (true)
	{
		EVENT newEvent = mes.wait();

		if (isOpen == false)
		{
			break;
		}
		if (newEvent.type == -1)
		{
			isOpen = false;
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
					

					break;
				}
			}
#ifdef EVENT_DATA_VOID
			if(newEvent.data!=nullptr)
				delete newEvent.data;
#endif // EVENT_DATA_VOID
#ifdef EVENT_DATA_CLASS
			if (newEvent.data != nullptr)
				delete newEvent.data;
#endif 
		}
	}
}
*/

void  EventsCenter::eventDispatchLoop()
{
	while (true)
	{
		EVENT newEvent = mes.wait();

		if (isOpen == false)
		{
			break;
		}
		if (newEvent.type == -1)
		{
			isOpen = false;
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


					break;
				}
			}
#ifdef EVENT_DATA_VOID
			if (newEvent.data != nullptr)
				delete newEvent.data;
#endif // EVENT_DATA_VOID
#ifdef EVENT_DATA_CLASS
			if (newEvent.data != nullptr)
				delete newEvent.data;
				
#endif 
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

	EVENT ev1;
	ev1.type = -1;
	ev1.data = nullptr;
	sendEvent(ev1);
	
}


void EventsCenter::Init(int id)
{
	name = id;
	
}

int EventsCenter::getQueueEvents()
{
	
	return mes.getDepth();
}

EventsCenter::EventsCenter()
{

}

EventsCenter::~EventsCenter()
{
	delete td_dispatch;
}



