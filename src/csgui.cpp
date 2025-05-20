#ifndef CSGUI_CPP
#define CSGUI_CPP

#include "csgui.hpp"

/**
 * Permite conectarte como cliente TCP a una IP con un puerto definido
 *
 * @param port puerto al que conectarse.
 * @param ip por la que conectarse
 */
Csgui::Csgui(const int port, const std::string& ip){
    this->type_instance = 0; // 0 == cliente
    this->ip = ip;
    this->port = port;
    assert(port > 0);
    assert(csgui_init_socket_t() == 0);

    // la direccion ip no puede ser nula
    //assert(ip != nullptr);

    // el puerto debe ser mayor a 0
    assert(port > 0);

    // error al iniciar los sockets si no devuelve 0
    assert (csgui_init_socket_t() == 0);

    sock = csgui_create_socket_t(AF_INET, SOCK_STREAM, 0);

    // Error al crear el socket
    assert(sock >= 0);

    // si devuelve 0 o un numero negativo, error
    //assert(csgui_bind_t(&addr, AF_INET, port, ip.c_str()) > 0);

    // Prepara la estructura de dirección del servidor
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    assert(inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) > 0);

    // Desactivar el algoritmo Nagle
    const char flag = 1;
    assert (
        csgui_setsockopt_t(
            sock, IPPROTO_TCP,
            TCP_NODELAY, &flag,
            sizeof(int)) == 0
    );

}

/**
 * Constructor para servidor (escucha en cualquier IP) (0.0.0.0)
 *
 * @param port puerto desde el que escuchar una conexion
 */
Csgui::Csgui(int port) {
    this->type_instance = 1; // 1 == servidor
    this->ip = "0.0.0.0";
    this->port = port;
    assert(port > 0);
    assert(csgui_init_socket_t() == 0);

    sock = csgui_create_socket_t(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int bind_result = csgui_bind_t(sock, &addr, AF_INET, port, ip.c_str());
    assert(bind_result == 0);


    // Desactivar el algoritmo Nagle
    const char flag = 1;
    assert (
        csgui_setsockopt_t(
            sock, IPPROTO_TCP,
            TCP_NODELAY, &flag,
            sizeof(int)) == 0
    );
}

/**
 * @return estados de error: 0 es correcta la conexion, -1 es incorrecto
 */
int Csgui::connect_to_server() {
    if (this->type_instance != 0) return -1;
    int res = connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    if(res < 0) {
        perror("connect");
    }
    return res;
}

/**
 * Pone el socket en modo escucha para aceptar conexiones entrantes.
 * Esta función solo debe llamarse en instancias de tipo servidor.
 *
 * @param backlog Número máximo de conexiones pendientes en la cola de espera.
 * @return 0 si la operación fue exitosa, -1 si hubo un error (por ejemplo, si no es servidor o falla listen()).
 */
int Csgui::listen_on(int backlog) const {
    // Verifica que la instancia sea de tipo servidor (type_instance == 1).
    // Si no lo es, retorna -1 indicando error.
    if (this->type_instance != 1) return -1;

    // Llama a la función csgui_listen_t para poner el socket en estado de escucha.
    int res = csgui(listen_t)(sock, backlog);

    // Si listen falla (retorna valor negativo), imprime el error del sistema.
    if(res < 0) perror("listen");

    // Devuelve el resultado de listen (0 si ok, -1 si error).
    return res;
}

/**
 *
 * @param status_connect codigo de error dada por una funcion
 * @return se retorna true si huebo error, y false si no.
 */
bool Csgui::is_error_connect(int status_connect) {
    /*
    Permite comprobar si Csgui::connect_to_server tuvo exito
    o devolvio un error
    */
    return status_connect < 0;
}

/**
 *
 * @return Permite acceptar la conexion de un cliente desde un servidor
 */
Csgui Csgui::accept_client() {
    if (this->type_instance != 1) return -1;

    sockaddr_in_t client_addr{};
    socklen_t len = sizeof(client_addr);
    socket_t client_sock = csgui(accept_t)(sock, &client_addr, &len);
    assert(client_sock >= 0);

    Csgui client(this->port, this->ip); // Dummy init
    client.sock = client_sock;
    client.addr = client_addr;
    return client;
}

/**
 *
 * @return Permite cerrar el socket y con ello la conexion
 */
int Csgui::close() const {
    return csgui(close_t)(sock);
}

/*Csgui::~Csgui() {
    this->close();
}*/

/**
 * Permite el envío de datos al host remoto.
 *
 * @param data Puntero a los datos a enviar.
 * @param size Tamaño de los datos a enviar, en bytes.
 * @return 0 si se enviaron todos los datos correctamente, -1 si hubo error o no se enviaron todos los bytes.
 */
int Csgui::send_data(const char* data, size_t size) const {
    ssize_t sent = send(sock, data, size, 0);
    if (sent == static_cast<ssize_t>(size)) {
        return 0; // Éxito
    }
    return -1; // Error o envío incompleto
}

/**
 * Permite el envío de datos al host remoto usando std::string.
 *
 * @param data Cadena a enviar.
 * @return 0 si se enviaron todos los datos correctamente, -1 si hubo error o no se enviaron todos los bytes.
 */
int Csgui::send_data(const std::string& data) const {
    return this->send_data(data.c_str(), data.size());
}



/**
 * Recibe datos del host remoto.
 *
 * @param size Número máximo de bytes a recibir.
 * @return Cadena con los datos recibidos. Si ocurre un error, la cadena estará vacía.
 */
std::string Csgui::recv_data(int size) const {
    std::string response(size, 0); // Reserva espacio
    ssize_t received = recv(sock, &response[0], size, 0);

    if (received > 0) {
        response.resize(received); // Ajusta el tamaño al número real de bytes recibidos
        return response;
    }
    // Error o conexión cerrada
    return {};
}

#endif
