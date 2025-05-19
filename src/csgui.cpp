#ifndef CSGUI_CPP
#define CSGUI_CPP

#include "csgui.hpp"

Csgui::Csgui(const int port, const std::string& ip){

    this->ip = ip;
    this->port = port;

    // la direccion ip no puede ser nula
    //assert(ip != nullptr);

    // el puerto debe ser mayor a 0
    assert(port > 0);

    // error al iniciar los sockets si no devuelve 0
    assert (csgui_init_socket_t() == 0);

    client = csgui_create_socket_t(AF_INET, SOCK_STREAM, 0);

    // Error al crear el socket
    assert(client >= 0);

    // si devuelve 0 o un numero negativo, error
    assert(csgui_bind_t(&server_addr, AF_INET, port, ip.c_str()) > 0);

    // Desactivar el algoritmo Nagle
    const char flag = 1;
    assert (
        csgui_setsockopt_t(
            client, IPPROTO_TCP,
            TCP_NODELAY, &flag,
            sizeof(int)) == 0
    );

}

int Csgui::connect_to_server() {
    return connect(client, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
}

bool Csgui::is_error_connect(int status_connect) {
    /*
    Permite comprobar si Csgui::connect_to_server tuvo exito
    o devolvio un error
    */
    return status_connect < 0;
}

int Csgui::close() const {
    return csgui(close_t)(client);
}

/*Csgui::~Csgui() {
    this->close();
}*/

int Csgui::send_data(const char* data, size_t size) const{
    return send(client, data, size, 0);
}
int Csgui::send_data(const std::string& data) const{
    return this->send_data(data.c_str(), data.size());
}


std::string Csgui::recv_data(int size) const{
    std::string response(size, 0);
    recv(client, &response[0], size, 0);
    return response;
}

#endif
