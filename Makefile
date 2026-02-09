COMM := g++ -Wall -Wextra -O2
BIN_DIR := bin

CLIENT_SRC := client.cpp utils.cpp
SERVER_SRC := server.cpp utils.cpp

CLIENT_BIN := $(BIN_DIR)/client
SERVER_BIN := $(BIN_DIR)/server

client: $(BIN_DIR) $(CLIENT_SRC)
	$(COMM) $(CLIENT_SRC) -o $(CLIENT_BIN)

server: $(BIN_DIR) $(SERVER_SRC)
	$(COMM) $(SERVER_SRC) -o $(SERVER_BIN)
