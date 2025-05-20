#include "csgui.hpp"

int main() {
    Csgui client(9000, "127.0.0.1");
    while (true) {
        int status = client.connect_to_server();
        if (!Csgui::is_error_connect(status)) {
            std::cout << "Conectado al servidor!" << std::endl;
            client.send_data("Hola servidor!");
            std::string respuesta = client.recv_data(1024);
            std::cout << "Respuesta del servidor: " << respuesta << std::endl;
            break;
        } else {
            std::cerr << "Fallo la conexion, reintentando..." << std::endl;
            sleep(1); // Espera antes de reintentar
        }
    }
}
