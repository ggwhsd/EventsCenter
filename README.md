# EventsCenter
 It is a event Engine . I suggest it is better to  use it in application layer.  like EventsBus,  clients push  Events into the EventsCenter , and then EventsCenter will broadcast  events to all the listener to deal it.  The EventsCenter is thread safe. And it use thread wait/notify schema to transfer the events into the Event dispatch  thread.
