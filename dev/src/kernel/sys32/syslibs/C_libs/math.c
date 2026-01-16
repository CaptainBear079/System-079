#include <sys32/syslibs/math.h>

// Convert from fraction to percentage
uint32_t FracToPercent(long long* p_PERCENT, long long value, long long total_value) {
    return (*p_PERCENT = ((value / total_value) * 100));
}