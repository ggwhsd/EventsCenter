#include "./EventsCenterImpl.h"






void EventsCenter::addEventListener(IEventListener *listener)
{
	lock_guard<mutex> lk(m_mutex_update);
	list_listeners.push_back(listener);
}
//�����¼�

void EventsCenter::sendEvent(EVENT ev1)
{
	mes->notify(ev1);
}
//�¼��ַ�,�������ƹ㲥��ʽ��������������Ϣ���ͣ��ɽ������Լ��ж���Ϣ����������

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
//�����¼����ġ�
void EventsCenter::Run()
{

	isOpen = true;
	td_dispatch = new thread(&EventsCenter::eventDispatchLoop, this);
	td_dispatch->detach();
}
//�ر��¼����ġ�
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



