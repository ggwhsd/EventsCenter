#pragma once
#include <string>
using namespace std;
/*
* 2019/8/14
* author ： GUGW
* 事件分发中心接口
*/


#ifdef EVENTSCENTER_EXPORTS
#define EVENTSCENTER_API __declspec(dllexport)
#else
#define EVENTSCENTER_API __declspec(dllimport)
#endif



enum EVENT_TYPE
{
	EVENT_NONE,
	EVENT_TICK,
	EVENT_ORDER,
	EVENT_TRADE
};
struct TICK
{

};

struct ORDER
{

};

struct TRADE
{

};

union EVENT_DATA
{
	TRADE trade;
	ORDER order;
	TICK tick;

};

struct EVENT
{

	EVENT_TYPE type;
	EVENT_DATA data;

};


class IEventListener
{
public:
	IEventListener() {};
	virtual ~IEventListener() {};

public:
	virtual bool EventHandle(EVENT) = 0;
};


/*
多线程（ 1读 多写 ）消息/事件分发中心，对应于通常意义的事件总线或者事件驱动引擎，应用层面，方便不同模块数据进行通信。
事件类型可以自定义
*/
class IEventsCenter
{
public:
	//注册事件及其监听者,监听者通过receEvent接口方法获取事件类型及其事件内容。
	virtual void addEventListener(IEventListener *listener)=0;
	//发送事件
	virtual void sendEvent(EVENT ev1)=0;
	//事件分发,采用类似广播方式，不进行区分消息类型，由接收者自己判断消息类型来处理
	virtual void eventDispatchLoop() =0;
	//运行事件中心。
	virtual void Run()=0;
	//关闭事件中心。
	virtual void Close()=0;
	//初始化，给与复制名称标识该事件分发中心。
	virtual void Init(string str)=0;
	virtual ~IEventsCenter() {};
};


extern "C" EVENTSCENTER_API IEventsCenter* _stdcall CreateEventCenterObj();

extern "C" EVENTSCENTER_API void _stdcall DestroyEventCenterObj(IEventsCenter* pExport);