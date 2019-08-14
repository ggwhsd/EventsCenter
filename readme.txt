#include "./lib/EventsCenterInterface.h"
#pragma comment(lib, "./lib/EventsCenter.lib")
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



}
