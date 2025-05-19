#ifndef CSGUI_C
#define CSGUI_C

#include "csgui.h"

static inline int csgui(init_socket_t)() {
    /*
     * Si retorna algo distinto a 0 ocurrio error en la inicializacion de winsock
     */
    #ifdef __linux__
    return 0;
    #elif defined(_WIN32) || defined(_WIN64)
    static WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif
}

static inline socket_t csgui(create_socket_t)(
        const int ARG_IN af,
        int ARG_IN type,
        int ARG_IN protocol
    ) {
    /*
     * retorna un socket
     */

    // inicializamos un socket con error:
    socket_t self = (socket_t)(~0);
    self = socket(af, type, protocol);
    return self;
}

static inline int csgui(setsockopt_t)(
        socket_t ARG_IN self,
        int ARG_IN level,
        int ARG_IN optname,
        const char * ARG_IN optval,
        int ARG_IN optlen
    ){
    /*
     * Si retorna algo distinto a 0 ocurrio error
     */
    return setsockopt(self, level, optname, optval, optlen);
}

static inline int csgui(close_t)(socket_t ARG_IN self) {
    #ifdef __linux__
    return close(self);
    #elif defined(_WIN32) || defined(_WIN64)
    int val = closesocket(self);
    WSACleanup();
    return val;
    #endif
}

static inline int csgui(bind_t)(
        sockaddr_in_t * ARG_IN addr,
        short ARG_IN family,
        unsigned short ARG_IN port,
        const char * ARG_IN ip_address
    ) {
    addr->sin_family = family;
    addr->sin_port   = htons(port);

    return inet_pton(AF_INET, ip_address, &(addr->sin_addr));
}



#endif