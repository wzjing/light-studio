#ifndef LIGHTNING_DRIVER_LOG_HPP
#define LIGHTNING_DRIVER_LOG_HPP

#include <string>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG
#define LOG_INFO
#define LOG_WARN
#define LOG_ERROR

#define debug(format, args...) log(__FILENAME__, LOG_DEBUG, ##args)
#define info(format, args...) log(__FILENAME__, LOG_INFO, ##args)
#define warn(format, args...) log(__FILENAME__, LOG_WARN, format, ##args)
#define error(format, args...) log(__FILENAME__, LOG_ERROR, format, ##args)

template<typename ... Args>
void log(const char *tag, int level, const char *format, Args... args) {
    int size_s = std::snprintf( nullptr, 0, format, args ... ) + 1; // Extra space for '\0'
//    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    std::snprintf( buf.get(), size, format, args ... );
//    switch (level) {
//        case LOG_DEBUG:
//            break;
//        case LOG_INFO:
//            break;
//        case LOG_WARN:
//            break;
//        case LOG_ERROR:
//            break;
//        default:
//            break;
//    }
    std::cout<<std::string( buf.get(), buf.get() + size - 1 );
}

#endif //LIGHTNING_DRIVER_LOG_HPP
