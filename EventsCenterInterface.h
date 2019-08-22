#pragma once
#include <string>
#include <chrono>
using namespace std;
/*
* 2019/8/14
* author �� GUGW
* 
*/


#ifdef EVENTSCENTER_EXPORTS
#define EVENTSCENTER_API __declspec(dllexport)
#else
#define EVENTSCENTER_API __declspec(dllimport)
#endif

//�¼����ݵ�����ʵ�ַ�ʽ
//#define EVENT_DATA_UNION  
//#define EVENT_DATA_VOID
#define EVENT_DATA_CLASS

typedef chrono::time_point<chrono::steady_clock, chrono::microseconds> microSeoncd_timePoint;


#ifdef EVENT_DATA_UNION

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
struct NOTIFY
{

};

/*
union��ʽ�ĺô��������������Ͷ�����ã�Ȼ��ͨ��ջ�����������ݡ�
���ƣ���Ҫ�������Ͷ�����ã������޷�ʹ�ã������¼����ݵ���ʡ��չ��һ�����ޡ�
*/
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
#endif // EVENT_DATA_UNION

#ifdef EVENT_DATA_VOID
/*
void*��ʽ�ĺô��������������ͣ������߸���event_type���жϾ������ͣ�Ȼ��ʹ������ת��������Ըߡ�
���ƣ���ΪEventsCenter�Ƕ��̰߳�ȫ�ģ�������Ҫ��̬�����¼����ݣ�new �ѣ���ʹ����֮��������Ҫ������ݣ���Ҫ��ʱ���٣�delete����
*/

#define EVENT_DATA (void*);
struct EVENT
{
	int type;
	void* data;
};
#endif // EVENT_DATA_VOID

#ifdef EVENT_DATA_CLASS
/*�̳з�ʽ*/
class IEVENT
{
public:
	microSeoncd_timePoint createTime;
	virtual ~IEVENT() {};
};
struct EVENT
{
	int type;
	IEVENT *data;
};

#endif // EVENT_DATA_CLASS



class IEventListener
{
public:
	IEventListener() {};
	virtual ~IEventListener() {};

public:
	virtual bool EventHandle(EVENT) = 0;
};


/*
���̣߳� 1�� ��д ����Ϣ/�¼��ַ����ģ���Ӧ��ͨ��������¼����߻����¼��������棬Ӧ�ò��棬���㲻ͬģ�����ݽ���ͨ�š�
�¼����Ϳ����Զ���
*/
class IEventsCenter
{
public:
	//ע���¼����������,������ͨ��receEvent�ӿڷ�����ȡ�¼����ͼ����¼����ݡ�
	virtual void addEventListener(IEventListener *listener)=0;
	//�����¼�
	virtual void sendEvent(EVENT ev1)=0;
	//�¼��ַ�,�������ƹ㲥��ʽ��������������Ϣ���ͣ��ɽ������Լ��ж���Ϣ����������
	virtual void eventDispatchLoop() =0;
	//�����¼����ġ�
	virtual void Run()=0;
	//�ر��¼����ġ�
	virtual void Close()=0;
	//��ʼ�������븴�����Ʊ�ʶ���¼��ַ����ġ�
	virtual void Init(string str)=0;

	virtual int getQueueEvents()=0;
	virtual ~IEventsCenter() {};
};


extern "C" EVENTSCENTER_API IEventsCenter* _stdcall CreateEventCenterObj();

extern "C" EVENTSCENTER_API void _stdcall DestroyEventCenterObj(IEventsCenter* pExport);