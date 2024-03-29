// Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../EventsCenterInterface.h"
#include "./utils.h"
#include <chrono>
#include <iostream>

#ifdef DEBUG
#pragma comment(lib, "../Debug/EventsCenter.lib")
#else
#pragma comment(lib, "../Release/EventsCenter.lib")
#endif // DEBUG

#include <thread>
using namespace std;
typedef chrono::time_point<chrono::steady_clock, chrono::microseconds> microClock_type_steady;
int diffNow(microClock_type_steady micro)
{
	microClock_type_steady temp_now = chrono::time_point_cast<chrono::microseconds>(chrono::steady_clock::now());
	return (temp_now - micro).count();

}



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
	int c;
};

/*
  yourself eventListenerClass
*/
int lenTest = 1000;
int PerformanceTest[1000000];
int indexTest = 0;
void ShowPerformanceBench()
{
	int i = 0;
	double sum = 0;
	while (i<lenTest)
	{
		sum += PerformanceTest[i];
		i++;
	}
	cout <<sum / lenTest << endl;
}

class TestTickListener : public IEventListener
{
public:
	virtual bool EventHandle(EVENT args) override
	{
		if (args.type == EVENT_TYPE::EVENT_TICK)
		{
			//cout << __FUNCTION__ << " args.type " << args.type << " " << ((TickEvent*)args.data)->c << endl;

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

		microSeoncd_timePoint handleTime = chrono::time_point_cast<chrono::microseconds>(chrono::steady_clock::now());
		std::chrono::microseconds timeSpan_micro(1);
		timeSpan_micro = handleTime - ((OrderEvent*)args.data)->createTime;
		PerformanceTest[indexTest++] = timeSpan_micro.count();

		//TODO
		//cout << __FUNCTION__ << " args.type " << args.type << " " << ((OrderEvent*)args.data)->c << endl;

		return true;
	}
};
#endif 

void threadRunMethod(IEventsCenter *pCenter)
{
	int i = 0;
	microSeoncd_timePoint start = chrono::time_point_cast<chrono::microseconds>(chrono::steady_clock::now());

	while (i < lenTest)
	{
		EVENT ev1;
		OrderEvent *order = new OrderEvent();
		order->c=1;
		ev1.type = EVENT_TYPE::EVENT_ORDER;
		ev1.data = order;
		pCenter->sendEvent(ev1);
		i++;
	}	
	
	while (pCenter->getQueueEvents() > 0)
	{
		SleepMilliSec(1);
		continue;
	}
	std::chrono::microseconds timeSpan_micro(1);
	timeSpan_micro = chrono::time_point_cast<chrono::microseconds>(chrono::steady_clock::now()) - start;
	cout << "" << i << "," << timeSpan_micro.count() << "," << (double)i / timeSpan_micro.count() * 1000000 << ",";
	ShowPerformanceBench();
}
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
	//cout << "enter events: (1-1000000)" ;
	cin >> lenTest;
	IEventsCenter *pCenter = CreateEventCenterObj();
	pCenter->Init(1);
	pCenter->Run();
	SleepMilliSec(1000);
	TestTickListener tickListener;
	pCenter->addEventListener(&tickListener);
	TestOrderListener *orderListener = new TestOrderListener();
	pCenter->addEventListener(orderListener);

	EVENT ev1;
	TickEvent *t = new TickEvent();
	ev1.type = EVENT_TYPE::EVENT_TICK;
	ev1.data = t;
	pCenter->sendEvent(ev1);




	thread th(threadRunMethod,pCenter);
	th.join();
	SleepMilliSec(1000);
	pCenter->Close();
	DestroyEventCenterObj(pCenter);
#endif
    return 0;
}

