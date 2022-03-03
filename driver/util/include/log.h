#ifndef LIGHTNING_DRIVER_LOG_H
#define LIGHTNING_DRIVER_LOG_H

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG   0
#define LOG_INFO    1
#define LOG_WARN    2
#define LOG_ERROR   3

// Set the default log to output
#define LOG_DEFAULT LOG_INFO
#define LOG(LEVEL, FUNC) if (LOG_DEFAULT <= LEVEL) FUNC

#define debug(format, args...)  LOG(LOG_DEBUG, printf("\x1B[38;5;8m%s -> %s : " #format "\x1B[0m\n", __FILENAME__, __FUNCTION__, ##args))
#define info(format, args...)   LOG(LOG_INFO, printf("%s : " #format "\n", __FUNCTION__, ##args))
#define warn(format, args...)   LOG(LOG_WARN, printf("\x1B[33m%s -> %s : " #format "\x1B[0m\n", __FILENAME__, __FUNCTION__, ##args))
#define error(format, args...)  LOG(LOG_ERROR, printf("\x1B[31m%s -> %s : " #format "\x1B[0m\n", __FILENAME__, __FUNCTION__, ##args))


#endif //LIGHTNING_DRIVER_LOG_H
