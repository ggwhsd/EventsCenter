# EventsCenter
 It is a event Engine . I suggest it is better to  use it in application layer.  it is no like libevent which is a network event library.  It is like eventspp project, clients push  Events into the EventsCenter , and then EventsCenter will broadcast  events to all the listener to deal it.  The EventsCenter is thread safe. And it use thread wait/notify schema to transfer the events into the Event dispatch  thread.

* democode
'''cpp
 #include "./lib/EventsCenterInterface.h"
#include <iostream>
#pragma comment(lib, "./lib/EventsCenter.lib")

using namespace std;

//EVENT_TYPE type，you can declare yourself type  in the EventsCenterInterface.h.
//EVENT_DATA ,you can declare yourself type in the EventsCenterInterface.h

class TestTickListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT result) override
	{
		if (result.type == EVENT_TYPE::EVENT_TICK)
			cout  << __FUNCTION__ << " result " << result.type << " " << endl;
		return true;
	}
};

class TestOrderListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT result) override
	{
		if (result.type != EVENT_TYPE::EVENT_ORDER)
			return true;

		cout  << __FUNCTION__ << " result " << result.type << " " <<endl;
		cout  << __FUNCTION__ << " result " << result.type << " "  << endl;
		return true;
	}
};

int main()
{

	IEventsCenter *pCenter = CreateEventCenterObj();
	pCenter->Init("ooo");
	pCenter->Run();
	TestTickListener tickListener;
	pCenter->addEventListener(&tickListener);
	TestOrderListener *orderListener = new TestOrderListener();
	pCenter->addEventListener(orderListener);


	
	EVENT ev1;

	TICK t;
	ev1.type = EVENT_TYPE::EVENT_TICK;
	ev1.data.tick = t;
	pCenter->sendEvent(ev1);

	

	ORDER order;
	

	ev1.type = EVENT_TYPE::EVENT_ORDER;
	ev1.data.order = order;
	pCenter->sendEvent(ev1);

	system("pause");
	pCenter->Close();



}

'''