#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>

#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;
enum  TrafficLightPhase {RED, GREEN };

template <class T>
class MessageQueue
{
public:

    void send(T &&msg);
    T receive();

private:
    std::deque<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
};


class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    //constr
    TrafficLight();
    // getters / setters
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();
    // typical behaviour methods

private:
    // typical behaviour methods
    void cycleThroughPhases();
    MessageQueue<TrafficLightPhase> traffic_light_queue; 
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    
};

#endif