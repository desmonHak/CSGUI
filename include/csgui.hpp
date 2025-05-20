#ifndef CSGUI_HPP
#define CSGUI_HPP

#include <iostream>
#include <cassert>

#ifndef CSGUI_H
#include "csgui.h"
#endif

class Csgui {
    public:
        socket_t               sock;
        sockaddr_in_t        addr{};
        std::string              ip;
        uint16_t               port;
        uint8_t       type_instance;

        /**
         *
         * @param port puerto al que realizar la conexion
         * @param ip ip a la que conectarse
         */
        Csgui(int port, const std::string& ip);

        // Constructor para servidor
        Csgui(int port);

        //~Csgui();
        int connect_to_server();
        int listen_on(int backlog = 5) const;
        Csgui accept_client();


        static bool is_error_connect(int status_connect);
        int close() const;
        int send_data(const char* data, size_t size) const;
        int send_data(const std::string& data) const;
        std::string recv_data(int size) const;

};

#endif