CXX := g++
CXXFLAGS := -Wall -Wextra -O2

BUILD_DIR := build

CLIENT_BIN := $(BUILD_DIR)/client
SERVER_BIN := $(BUILD_DIR)/server

CLIENT_OBJ := $(BUILD_DIR)/client.o
SERVER_OBJ := $(BUILD_DIR)/server.o
UTILS_OBJ  := $(BUILD_DIR)/utils.o
HASHTABLE_OBJ := $(BUILD_DIR)/hashtable.o

UTILS_HDR := utils.h
HASHTABLE_HDR := hashtable.h

.PHONY: all clean client server

all: $(CLIENT_BIN) $(SERVER_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLIENT_BIN): $(CLIENT_OBJ) $(UTILS_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(SERVER_BIN): $(SERVER_OBJ) $(UTILS_OBJ) $(HASHTABLE_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CLIENT_OBJ): client.cpp $(UTILS_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SERVER_OBJ): server.cpp $(UTILS_HDR) $(HASHTABLE_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(UTILS_OBJ): utils.cpp $(UTILS_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(HASHTABLE_OBJ): hashtable.cpp $(HASHTABLE_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

client: $(CLIENT_BIN)

server: $(SERVER_BIN)

clean:
	rm -rf $(BUILD_DIR)
