// EventsCenter.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "./EventsCenterImpl.h"


EVENTSCENTER_API IEventsCenter*  CreateEventCenterObj()
{
	IEventsCenter* base = new EventsCenter();
	return base;
}

EVENTSCENTER_API void  DestroyEventCenterObj(IEventsCenter* pCenter)
{
	delete pCenter;
}