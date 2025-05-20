#include "csgui.hpp"

int main() {
    Csgui server(9000);
    while (true) {
        int listen_result = server.listen_on();
        std::cout << "listen result: " << listen_result << std::endl;
        assert(listen_result == 0);

        std::cout << "Servidor escuchando..." << std::endl;
        Csgui client = server.accept_client();
        std::cout << "Cliente aceptado!" << std::endl;
        std::string recibido = client.recv_data(1024);
        std::cout << "Mensaje recibido: " << recibido << std::endl;
        client.send_data("Hola cliente!");
    }
    return 0;
}
