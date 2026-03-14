CXX := g++
CXXFLAGS := -Wall -Wextra -O2

DEBUG_FLAGS := -Wall -Wextra -g

BUILD_DIR := build

CLIENT_BIN := $(BUILD_DIR)/client
SERVER_BIN := $(BUILD_DIR)/server

CLIENT_DBG_BIN := $(BUILD_DIR)/client_dbg
SERVER_DBG_BIN := $(BUILD_DIR)/server_dbg

CLIENT_OBJ := $(BUILD_DIR)/client.o
SERVER_OBJ := $(BUILD_DIR)/server.o
UTILS_OBJ  := $(BUILD_DIR)/utils.o
HASHTABLE_OBJ := $(BUILD_DIR)/hashtable.o
AVL_OBJ := $(BUILD_DIR)/avl.o
ZSET_OBJ := $(BUILD_DIR)/zset.o
HEAP_OBJ := $(BUILD_DIR)/heap.o
THREADPOOL_OBJ := $(BUILD_DIR)/thread_pool.o 

UTILS_HDR := utils.h
HASHTABLE_HDR := hashtable.h
AVL_HDR := avl.h
ZSET_HDR := zset.h
HEAP_HDR := heap.h 
THREADPOOL_HDR := thread_pool.h 

.PHONY: all clean client server debug-client debug-server

all: $(CLIENT_BIN) $(SERVER_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLIENT_BIN): $(CLIENT_OBJ) $(UTILS_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(SERVER_BIN): $(SERVER_OBJ) $(UTILS_OBJ) $(HASHTABLE_OBJ) $(AVL_OBJ) $(ZSET_OBJ) $(HEAP_OBJ) $(THREADPOOL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CLIENT_OBJ): client.cpp $(UTILS_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SERVER_OBJ): server.cpp list.h $(UTILS_HDR) $(HASHTABLE_HDR) $(AVL_HDR) $(ZSET_HDR) $(HEAP_HDR) $(THREADPOOL_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(UTILS_OBJ): utils.cpp $(UTILS_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(HASHTABLE_OBJ): hashtable.cpp $(HASHTABLE_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(AVL_OBJ): avl.cpp $(AVL_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ZSET_OBJ): zset.cpp $(ZSET_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(HEAP_OBJ): heap.cpp $(HEAP_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(THREADPOOL_OBJ): thread_pool.cpp $(THREADPOOL_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLIENT_DBG_BIN): $(CLIENT_OBJ) $(UTILS_OBJ)
	$(CXX) $(DEBUG_FLAGS) $^ -o $@

$(SERVER_DBG_BIN): $(SERVER_OBJ) $(UTILS_OBJ) $(HASHTABLE_OBJ) $(AVL_OBJ) $(ZSET_OBJ) $(HEAP_OBJ) $(THREADPOOL_OBJ)
	$(CXX) $(DEBUG_FLAGS) $^ -o $@

client: $(CLIENT_BIN)

server: $(SERVER_BIN)

debug-client: $(CLIENT_DBG_BIN)

debug-server: $(SERVER_DBG_BIN)

clean:
	rm -rf $(BUILD_DIR)
