/*******************************************************************************
 * Description:
 * @file : global.h
 * @brief : contains standard header files used and defines
 * Author       - HolyDevil
 *
 *******************************************************************************
 *
 * History
 *
 * Jan/23/2017, Holydevil, Created for training purpose
 *
 ******************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#define DEBUG 0
#if DEBUG
	#define PRINTD(fmt, ...) printf(fmt,##__VA_ARGS__)
#else
	#define PRINTD(fmt, ...) do{} while (0)
#endif



/******************************************************************************
 * Include
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/ipc.h>

#include "on_off.h"

/******************************************************************************
 * Define
 ******************************************************************************/
#define SUCCESSFUL 0
#define UNSUCCESSFUL 1
#define UNSUPPORTED_ATTRIBUTE 1
#define VALID_DATA 2
#define INVALID_ATTRIBUTE 3
#define INVALID_CLUSTERID 4

/******************************************************************************
 * Structure
 ******************************************************************************/
struct command {
        uint16_t clusterId;
        bool clusterSpecific;			// 1 for yes, 0 for not
        uint8_t commandId;
        uint16_t attributeId;
        uint16_t data;
};

#endif  /* '#endif' of CODING_STYLE_H */
