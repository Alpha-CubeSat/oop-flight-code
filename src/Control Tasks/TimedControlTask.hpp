//Taken from Cornell's PAN

#ifndef TIMED_CONTROL_TASK_HPP_
#define TIMED_CONTROL_TASK_HPP_


#include <string>

#ifdef DESKTOP
#include <thread>
#include <chrono>
#include <time.h>
#else
#include <Arduino.h>
#endif

/**
 * The sys_time_t and systime_duration_t are common types for tracking system
 * time across both the desktop and the native platforms.
 * 
 * - sys_time_t denotes an absolute time
 * - systime_duration_t denotes a unitless separation between two system times
 * 
 * On Teensies, these quantities are all unsigned ints representing microseconds. On
 * desktop platforms, the std::chrono library is used to provide meaning to each of
 * these two constructs. See below.
 */
#ifdef DESKTOP
typedef std::chrono::steady_clock::time_point sys_time_t;
typedef std::chrono::steady_clock::duration systime_duration_t;
#else
typedef unsigned int sys_time_t;
typedef unsigned int systime_duration_t;
#endif

/**
 * @brief Timing values and functions that are shared across all timed control tasks,
 * irrespective of return type.
 */
class TimedControlTaskBase
{
  protected:
    /**
    * @brief The time at which the current control cycle started.
    */
    static sys_time_t control_cycle_start_time;

  public:
    static unsigned int control_cycle_count;

    /**
     * @brief Get the system time.
     * 
     * @return sys_time_t
     */
    static sys_time_t get_system_time()
    {
    #ifdef DESKTOP
      return std::chrono::steady_clock::now();
    #else
      return (sys_time_t) micros();
    #endif
    }

    /**
     * @brief Convert a duration object into microseconds.
     * 
     * @param delta 
     * @return systime_duration_t 
     */
    static unsigned int duration_to_us(const systime_duration_t &delta)
    {
      #ifdef DESKTOP
        return std::chrono::duration_cast<std::chrono::microseconds>(delta).count();
      #else
        return delta;
      #endif
    }

    /**
     * @brief Convert microseconds into a duration object.
     * 
     * @param delta 
     * @return systime_duration_t 
     */
    static systime_duration_t us_to_duration(const unsigned int delta)
    {
      #ifdef DESKTOP
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(delta));
      #else
        return delta;
      #endif
    }

    static void wait_duration(const unsigned int &delta_t)
    {
      const sys_time_t start = get_system_time();
      // Wait until execution time
      while (duration_to_us(get_system_time() - start) < delta_t)
      { 
        #ifndef DESKTOP
        delayMicroseconds(10);
        #endif
      }
    }
};
template <typename T>
class ControlTask {
  public:
    /**
     * @brief Construct a new Control ControlTaskBase object
     *
     */
    ControlTask() {}

    /**
     * @brief Run main method of control ControlTaskBase.
     */
    virtual T execute() = 0;

};

/**
 * @brief Task-specific methods and values for timed control tasks.
 * 
 * @tparam T Return type of control task.
 */
template <typename T>
class TimedControlTask : public ControlTask<T>, public TimedControlTaskBase
{
  private:
    /**
     * @brief The start time of this control task, relative
     * to the start of any control cycle, in microseconds.
     */
    systime_duration_t offset;

  public:
    /**
     * @brief Execute this control task's task, but only if it's reached its
     * start time.
     * 
     * @param control_cycle_start_time System time for the start of the control task.
     * @return T Value returned by execute().
     */
    T execute_on_time()
    {
      sys_time_t earliest_start_time =
        TimedControlTaskBase::control_cycle_start_time + offset;
      wait_until_time(earliest_start_time);
      //Serial.print("Executing");
      //Serial.print(" offset: ");
      //Serial.println(offset);
      //Serial.print("start time: ");
      //Serial.println(earliest_start_time);
      return this->execute();
    }

    /**
     * @brief Cause the system to pause operation until a system time is reached.
     * 
     * TODO check for errors that could happen on Teensy due to integer overflow.
     * 
     * @param time Time until which the system should pause.
     */
    sys_time_t wait_until_time(const sys_time_t &time)
    {
      // Compute timing statistics and publish them to state fields
      const signed int delta_t = (signed int)duration_to_us(time - get_system_time());
      const unsigned int wait_time = std::max(delta_t, 0);

      wait_duration(wait_time);
      return delta_t;
    }

    /**
     * @brief Construct a new Timed Control Task object
     * 
     * @param offset Time offset of start of this task from the beginning of a
     *               control cycle, in microseconds.
     */
    TimedControlTask(const unsigned int _offset):
    ControlTask<T>(), offset(us_to_duration(_offset + 1)){
        }
};

#endif