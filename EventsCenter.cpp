// EventsCenter.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "./EventsCenterImpl.h"


EVENTSCENTER_API IEventsCenter* APIENTRY CreateEventCenterObj()
{
	IEventsCenter* base = new EventsCenter();
	return base;
}

EVENTSCENTER_API void APIENTRY DestroyEventCenterObj(IEventsCenter* pCenter)
{
	delete pCenter;
}