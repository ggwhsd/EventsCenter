# EventsCenter
 It is a event Engine . I suggest it is better to  use it in application layer.  it is no like libevent which is a network event library.  It is like eventspp project, clients push  Events into the EventsCenter , and then EventsCenter will broadcast  events to all the listener to deal it.  The EventsCenter is thread safe. And it use thread wait/notify schema to transfer the events into the Event dispatch  thread.
 when it runs, it will create a new thread and dispatch events in this new thread.

# democode

```cpp

#include "./lib/EventsCenterInterface.h"
#include <iostream>
#pragma comment(lib, "./lib/EventsCenter.lib")
using namespace std;

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
	return 0;
}
#endif

```

# 扩展
* `EVENT_DATA` 也就是事件的数据，如果要支持自定义，同时又方便不同模块传递，则使用union是一种不错的方式，但是也可以使用void *的方式灵活性会更高，或者使用比void*更强的约束继承方式。

* union方式：
    * 好处：（值传递）所有数据类型都定义好，方便快捷。
	* 劣势：需要数据类型都定义好，否则无法使用，对于事件数据的内省扩展有一定局限。

* void指针方式：
	* 好处：（引用）可以任意类型，接收者根据`EVENT_TYPE`来判断具体类型，然后使用类型转换。灵活性高。
	* 劣势：需要动态创建事件数据（new 堆），因为EventsCenter是多线程安全的，指针在传递给接收者线程使用完之后若不需要这个数据，需要及时销毁指针（delete）。但是对于绝大多数系统而言，这个劣势根本不值得考虑，应该说是习以为常的。比如java语言中，每次事件都会new一个对象，用完就由GC自动回收了。

* 继承方式：
	* 好处：（引用）更void指针类似，但是由于使用了继承，就限定了一些约束关系，提高开发的安全性。而不是像void指针那样自由。
	* 劣势：更void指针一样。

#性能测试

* 环境：win10_x64 home, Core(TM)i5-6500@3.2GHz, 16GB DDR3
* 测试demo：参考Test项目中Test.cpp
* 数据：
	<table>
	<tr>
	<th>发送事件数</th>
	<th>发送耗时（微秒）</th>
	<th>平均每秒发送事件数</th>
	<th>平均每秒处理事件耗时（微秒）</th>
	</tr>
	<tr>
	<td>1000</td>
<td>2148</td>
<td>465549</td>
	<td>36.938</td>
	</tr>
	<tr>
<td>100000</td>
<td>1164576</td>
<td>85868.2</td>
	<td>121.517</td>
	</tr>
	<tr>
	<td>100000</td>
	<td>1164576</td>
	<td>85868.2</td>
	<td>2925.1</td>
	</tr>
