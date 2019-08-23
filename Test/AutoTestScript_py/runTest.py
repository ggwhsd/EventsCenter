# -*- coding: UTF-8 -*-

import os
import datetime
class testResult:
    length = 0
    Timespan = 0
    eventsPerSeconds = 0.0
    agvTimeSpanPerEvent = 0.0
    
    def __init__(self, length, Timespan, eventsPerSeconds, agvTimeSpanPerEvent):
        self.length = length
        self.Timespan = Timespan
        self.eventsPerSeconds = eventsPerSeconds
        self.agvTimeSpanPerEvent = agvTimeSpanPerEvent


def main():
    
    testNeedCount = 5
    testCycle = 1
    testArgs= ["1000","10000","100000"]
    
    
    testNow=0
    testNowCycle=0
    testArgsIndex=0;
    
    performanceTestResult = {}
    i = 0
    while(i<len(testArgs)):
        performanceTestResult[testArgs[i]]=[]
        i = i+1
    
    fdetail=open(datetime.datetime.now().strftime('%Y_%m_%d_%H_%M_%S')+'.csv','w+')
    while (testNowCycle < testCycle):
        args = testArgs[testArgsIndex]
        testpath = r"echo %s | Test.exe" % (args)
        #r_v = os.system(testpath)
        fout = os.popen(testpath)
        data = fout.readlines()
        testData = data[0].strip('\n').split(",")
        fdetail.write(data[0]);
        result = testResult(testData[0],testData[1],testData[2],testData[3]);
        performanceTestResult[args].append(result);
        fout.close();
        testNow = testNow + 1
        if(testNow == testNeedCount):
            testNow=0
            testArgsIndex +=1
            print "testArgsIndex:",testArgsIndex
        if(testArgsIndex == 3):
            testNowCycle += 1
            testArgsIndex = 0;
            print "cycleCounts:",testNowCycle
    fdetail.close()
    i=0
    tplt = "{0:>10}\t{1:>15}\t{2:>15}\t{3:>15}"
    print tplt.format("events", "totalTimeSpan", "EventsPerSecond", "TimeSpanPerTime",'')
    tplt = "{0:>10}\t{1:>15.2f}\t{2:>15.2f}\t{3:>15.2f}"
    while(i<len(testArgs)):
        j=0
        while(j<len(performanceTestResult[testArgs[i]])):
            sumTimespan =0.0
            sumeventsPerSeconds=0.0
            sumagvTimeSpanPerEvent=0.0
            sumTimespan = sumTimespan + float(performanceTestResult[testArgs[i]][j].Timespan)
            sumeventsPerSeconds = sumeventsPerSeconds + float(performanceTestResult[testArgs[i]][j].eventsPerSeconds)
            sumagvTimeSpanPerEvent = sumagvTimeSpanPerEvent + float(performanceTestResult[testArgs[i]][j].agvTimeSpanPerEvent)
            j = j+1
        print tplt.format(testArgs[i],sumTimespan/j,sumeventsPerSeconds/j,sumagvTimeSpanPerEvent/j)
        i = i+1
        
        
            
if __name__ == '__main__':
    main()
    
