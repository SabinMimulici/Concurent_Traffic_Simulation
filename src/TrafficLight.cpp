#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <future>
#include <queue>

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex>ulock(_mutex);
    _cond.wait(ulock, [this]{ return !_queue.empty(); } );
    T msg = std::move(_queue.front());
    _queue.pop_front();
 
  
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T && msg)
{

    std::lock_guard<std::mutex>ulock(_mutex);

    _queue.push_back(std::move(msg));
    _cond.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::RED;
}

void TrafficLight::waitForGreen()
{

    while(1)
    {
        auto get_phase = traffic_light_queue.receive();
        if(get_phase == TrafficLightPhase::GREEN)
        {
            return;
        }

    }

}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{

    std::random_device device;
    std::mt19937 generator (device());
    std::uniform_int_distribution<int> distribution(4, 6);
    int cycle_duration = distribution(generator) *1000;

    std::chrono::time_point<std::chrono::system_clock> stop_watch = std::chrono::system_clock::now(); 
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - stop_watch).count();
        
        if(delta_time >= cycle_duration )
        {

            if(_currentPhase == GREEN)
            {
                _currentPhase = RED;
            }
            else if(_currentPhase == RED)
            {
                _currentPhase = GREEN;   
            }  

            traffic_light_queue.send(std::move(_currentPhase));
            stop_watch = std::chrono::system_clock::now();  
            cycle_duration = distribution(generator) *1000;
        }

    }  
}
