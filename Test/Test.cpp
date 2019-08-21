// Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../EventsCenterInterface.h"
#include <iostream>
#pragma comment(lib, "../Debug/EventsCenter.lib")
using namespace std;

class TestTickListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT result) override
	{
		if (result.type == EVENT_TYPE::EVENT_TICK)
			cout << __FUNCTION__ << " result " << result.type << " " << endl;
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

		cout << __FUNCTION__ << " result " << result.type << " " << endl;
		cout << __FUNCTION__ << " result " << result.type << " " << endl;
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
    return 0;
}

