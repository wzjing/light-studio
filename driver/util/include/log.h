//
// Created by Infinity on 1/15/22.
//

#ifndef LIGHTNING_DRIVER_LOG_H
#define LIGHTNING_DRIVER_LOG_H

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define debug(format, args...) printf("%s -> %s : " #format "\n", __FILENAME__, __FUNCTION__, ##args)
#define warn(format, args...) printf("\x1B[33m%s -> %s : " #format "\x1B[0m\n", __FILENAME__, __FUNCTION__, ##args)
#define error(format, args...) printf("\x1B[31m%s -> %s : " #format "\x1B[0m\n", __FILENAME__, __FUNCTION__, ##args)

#endif //LIGHTNING_DRIVER_LOG_H
