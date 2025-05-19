#ifndef CSGUI_HPP
#define CSGUI_HPP

#include <iostream>
#include <cassert>

#ifndef CSGUI_H
#include "csgui.h"
#endif

class Csgui {
    public:
        socket_t             client;
        sockaddr_in_t   server_addr{};
        std::string              ip;
        uint16_t               port;

        Csgui(int port, const std::string& ip);
        //~Csgui();
        int connect_to_server();
        static bool is_error_connect(int status_connect);
        int close() const;
        int send_data(const char* data, size_t size) const;
        std::string recv_data(int size) const;
        int send_data(const std::string& data) const;

};

#endif