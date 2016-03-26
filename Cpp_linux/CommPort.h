#ifndef __COMMPORT_H
#define __COMMPORT_H

#include "GlobalDefine.h"

#define SD_RECEIVE 0x00
#define SD_SEND 0x01
#define SD_BOTH 0x02

#define TCPBUFFERSIZE 4096
#define UDPBUFFERSIZE 4096

#define THREAD_TERMINATION_MAXWAIT_S 5		// seconds

enum SOCKET_SERVICE_TYPE { SOCKET_SERVER = 0, SOCKET_CLIENT };

#define SIGNAL_COMM_RECEIVE 0x00000001
#define SIGNAL_COMM_ACCEPT 0x00000002
#define SIGNAL_COMM_CLOSE 0x00000003

#define MAX_COM_RECV_LEN 8192			// Comm port recv buffer size
#define MAX_COM_SEND_LEN 8192			// Comm port send buffer size

#define MAX_SOCKET_RECV_LEN 8192		// Socket port recv buffer size
#define MAX_SOCKET_SEND_LEN 8192		// Socket port send buffer size

#define MAX_FRAME_LEN 1024


#endif // __COMMPORT_H
