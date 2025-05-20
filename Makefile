CXX      = g++
CPPFLAGS = -I./include -I./src
CXXFLAGS = -Wall -Wextra -std=c++11

# Archivos fuente
CLIENT_SRC = code1_client.cpp ./src/csgui.cpp
SERVER_SRC = code1_server.cpp ./src/csgui.cpp

# Ejecutables
CLIENT_EXE = code1_client.exe
SERVER_EXE = code1_server.exe

# Librerías para Windows (agrega -lWs2_32 solo si compilas en Windows)
LDLIBS = -lWs2_32

all: $(CLIENT_EXE) $(SERVER_EXE)

$(CLIENT_EXE): $(CLIENT_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

$(SERVER_EXE): $(SERVER_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f $(CLIENT_EXE) $(SERVER_EXE)
