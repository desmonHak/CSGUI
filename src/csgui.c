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
    socket_t sock,
    sockaddr_in_t * addr,
    short family,
    unsigned short port,
    const char * ip_address
) {
    addr->sin_family = family;
    addr->sin_port   = htons(port);
    if (inet_pton(AF_INET, ip_address, &(addr->sin_addr)) <= 0) {
        return -1;
    }

    return bind(sock, (struct sockaddr *)addr, sizeof(*addr));
}

static inline int csgui(listen_t)(socket_t self, int backlog) {
    return listen(self, backlog);
}

static inline socket_t csgui(accept_t)(socket_t self, sockaddr_in_t * addr, socklen_t * addrlen) {
    return accept(self, (struct sockaddr *)addr, addrlen);
}

static inline int csgui(connect_t)(socket_t self, const sockaddr_in_t * addr, socklen_t addrlen) {
    return connect(self, (const struct sockaddr *)addr, addrlen);
}

static inline ssize_t csgui(send_t)(socket_t self, const void * buf, size_t len, int flags) {
#ifdef __linux__
    return send(self, buf, len, flags);
#elif defined(_WIN32) || defined(_WIN64)
    return send(self, (const char *)buf, (int)len, flags);
#endif
}

static inline ssize_t csgui(recv_t)(socket_t self, void * buf, size_t len, int flags) {
#ifdef __linux__
    return recv(self, buf, len, flags);
#elif defined(_WIN32) || defined(_WIN64)
    return recv(self, (char *)buf, (int)len, flags);
#endif
}




#endif