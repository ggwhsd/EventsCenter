// Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../EventsCenterInterface.h"
#include <iostream>
#pragma comment(lib, "../Debug/EventsCenter.lib")
using namespace std;
#ifdef EVENT_DATA_UNION


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

#endif

#ifdef EVENT_DATA_VOID
/*
void指针方式：事件类型自定义，事件内容自定义，灵活性很高。
*/
enum EVENT_TYPE
{
	EVENT_NONE,
	EVENT_TICK,
	EVENT_ORDER,
	EVENT_TRADE
};

struct Tick
{
	int a;
	int b;
	int c;
};
struct Order
{
	int a;
	int b;
	string c;
};

class TestTickListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT args) override
	{
		if (args.type == EVENT_TYPE::EVENT_TICK)
		{
			cout << __FUNCTION__ << " args.type " << args.type << " " << ((Tick*)args.data)->c<< endl;
			
		}
		return true;
	}
};
class TestOrderListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT args) override
	{
		if (args.type != EVENT_TYPE::EVENT_ORDER)
			return true;

		cout << __FUNCTION__ << " args.type " << args.type << " " << ((Order*)args.data)->c << endl;
		return true;
	}
};

#endif


#ifdef EVENT_DATA_CLASS
/*
	yourself eventtype and event data class
*/
enum EVENT_TYPE
{
	EVENT_NONE,
	EVENT_TICK,
	EVENT_ORDER,
	EVENT_TRADE
};
class TickEvent : public IEVENT
{
public:
	int a;
	int b;
	int c;
};

class OrderEvent : public IEVENT
{
public:
	int a;
	int b;
	string c;
};

/*
  yourself eventListenerClass
*/
class TestTickListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT args) override
	{
		if (args.type == EVENT_TYPE::EVENT_TICK)
		{
			cout << __FUNCTION__ << " args.type " << args.type << " " << ((TickEvent*)args.data)->c << endl;

		}
		return true;
	}
};
class TestOrderListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT args) override
	{
		if (args.type != EVENT_TYPE::EVENT_ORDER)
			return true;

		cout << __FUNCTION__ << " args.type " << args.type << " " << ((OrderEvent*)args.data)->c << endl;
		return true;
	}
};
#endif 


int main()
{
#ifdef EVENT_DATA_UNION
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
#endif

#ifdef EVENT_DATA_VOID
	IEventsCenter *pCenter = CreateEventCenterObj();
	pCenter->Init("ooo");
	pCenter->Run();
	TestTickListener tickListener;
	pCenter->addEventListener(&tickListener);
	TestOrderListener *orderListener = new TestOrderListener();
	pCenter->addEventListener(orderListener);
	EVENT ev1;
	Tick *t = new Tick();
	ev1.type = EVENT_TYPE::EVENT_TICK;
	ev1.data = t;
	pCenter->sendEvent(ev1);
	Order *order = new Order;
	order->c = "hello my father";
	ev1.type = EVENT_TYPE::EVENT_ORDER;
	ev1.data = order;
	pCenter->sendEvent(ev1);
	system("pause");
	pCenter->Close();
#endif

#ifdef EVENT_DATA_CLASS
	IEventsCenter *pCenter = CreateEventCenterObj();
	pCenter->Init("ooo");
	pCenter->Run();
	TestTickListener tickListener;
	pCenter->addEventListener(&tickListener);
	TestOrderListener *orderListener = new TestOrderListener();
	pCenter->addEventListener(orderListener);
	EVENT ev1;
	TickEvent *t = new TickEvent();
	ev1.type = EVENT_TYPE::EVENT_TICK;
	ev1.data = t;
	pCenter->sendEvent(ev1);
	OrderEvent *order = new OrderEvent();
	order->c = "hello my father";
	ev1.type = EVENT_TYPE::EVENT_ORDER;
	ev1.data = order;
	pCenter->sendEvent(ev1);
	system("pause");
	pCenter->Close();
#endif
    return 0;
}

