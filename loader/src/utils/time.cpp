#include <Sapfire/utils/general.hpp>

#ifdef SAPFIRE_IS_WINDOWS
    #define SAPFIRE_CTIME() ctime_s(buf, sizeof buf, &t);
#else
    #define SAPFIRE_CTIME() strcpy(buf, ctime(&t));
#endif

std::string sapfire::utils::timePointAsString(std::chrono::system_clock::time_point const& tp) {
    auto t = std::chrono::system_clock::to_time_t(tp);
    char buf[128];
    SAPFIRE_CTIME();
    std::string res = buf;
    res.pop_back();
    return res;
}
