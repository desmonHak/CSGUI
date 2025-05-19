#include <iostream>

#include "csgui.hpp"

int main(){
    
    Csgui client(450, "127.0.0.1");
    if (Csgui::is_error_connect(client.connect_to_server())) {
        std::cout << "Error connecting to server" << std::endl;
        return 1;
    }

    std::cout << "Connected to server : " << client.ip << ":" << client.port << std::endl;

    if (client.send_data("hola") < 0) {
        std::cout <<  "Error al enviar el comando" << std::endl;
        return 1;
    }

    std::cout << client.recv_data(1024) << std::endl;

    std::cout << "exit sucessfully..." << std::endl;
    return 0;
}