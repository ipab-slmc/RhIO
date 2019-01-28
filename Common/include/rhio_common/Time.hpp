#ifndef RHIO_TIME_HPP
#define RHIO_TIME_HPP

#include <functional>

namespace RhIO {

/**
 * Function pointer to the time getter 
 * function used for default value timestamp
 */
extern std::function<int64_t()> FuncGetTime;

/**
 * Return the current time expressed
 * in microseconds ticks (Thread safe)
 */
inline int64_t getRhIOTime()
{
    return FuncGetTime();
}

}

#endif

