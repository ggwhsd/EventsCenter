#pragma once
#include "./EventsCenterInterface.h"
#include <thread>
#include <queue>
#include <iostream>
#include <mutex>
#include <list>
#include <condition_variable>
using namespace std;


template<typename T>
class messenger
{

public:
	T wait()
	{
		T data;

		unique_lock<mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });

		if (!data_queue.empty())
		{
			
			data = data_queue.front();
			data_queue.pop();
		}
		lk.unlock();
		return data;

	}

	void notify(T &message)
	{
		lock_guard<mutex> lk(mut);
		data_queue.push(message);
		data_cond.notify_one();
	}

	void init()
	{
		
	}
	int  getDepth()
	{
		return data_queue.size();
	}
	~messenger() 
	{
		
	}
private:
	mutex mut;
	queue<T> data_queue;
	condition_variable data_cond;

};


class EventsCenter : public IEventsCenter
{
private:
	int name;
	messenger<EVENT>  mes;
	bool isOpen; //是否关闭
	thread *td_dispatch;
	mutex m_mutex_update;
	list<IEventListener*> list_listeners;
private:
	EventsCenter(const EventsCenter&) = delete;
	EventsCenter& operator=(const EventsCenter&) = delete;

public:

	//注册事件及其监听者,监听者通过receEvent接口方法获取事件类型及其事件内容。
	void addEventListener(IEventListener *listener);
	//发送事件
	void sendEvent(EVENT ev1);
	//事件分发,采用类似广播方式，不进行区分消息类型，由接收者自己判断消息类型来处理
	void eventDispatchLoop();
	//运行事件中心。
	void Run();
	//关闭事件中心。
	void Close();

	void Init(int id);

	virtual int getQueueEvents();
	EventsCenter();
	~EventsCenter();

};
