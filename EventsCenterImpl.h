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
	bool isOpen; //�Ƿ�ر�
	thread *td_dispatch;
	mutex m_mutex_update;
	list<IEventListener*> list_listeners;
private:
	EventsCenter(const EventsCenter&) = delete;
	EventsCenter& operator=(const EventsCenter&) = delete;

public:

	//ע���¼����������,������ͨ��receEvent�ӿڷ�����ȡ�¼����ͼ����¼����ݡ�
	void addEventListener(IEventListener *listener);
	//�����¼�
	void sendEvent(EVENT ev1);
	//�¼��ַ�,�������ƹ㲥��ʽ��������������Ϣ���ͣ��ɽ������Լ��ж���Ϣ����������
	void eventDispatchLoop();
	//�����¼����ġ�
	void Run();
	//�ر��¼����ġ�
	void Close();

	void Init(int id);

	virtual int getQueueEvents();
	EventsCenter();
	~EventsCenter();

};
