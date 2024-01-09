LIBS = -lcsfml-system -lcsfml-network
BUILD_DIR = makefile-build

all: server_side

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

main.o: main.c | $(BUILD_DIR)
	gcc -c "main.c" -o $(BUILD_DIR)/main.o

player.o: player/player.h player/player.c | $(BUILD_DIR)
	gcc -c "player/player.c" -o $(BUILD_DIR)/player.o

application.o: application/application.h application/application.c | $(BUILD_DIR)
	gcc -c "application/application.c" -o $(BUILD_DIR)/application.o

linked_list.o: ../common/linked_list/linked_list.c ../common/linked_list/linked_list.h | $(BUILD_DIR)
	gcc -c "../common/linked_list/linked_list.c" -o $(BUILD_DIR)/linked_list.o

server_controller.o: server_controller/server_controller.c server_controller/server_controller.h ../common/base/server_message_types.h ../common/base/type_of_maps.h | $(BUILD_DIR)
	gcc -c "server_controller/server_controller.c" -o $(BUILD_DIR)/server_controller.o

server_side: main.o player.o application.o linked_list.o server_controller.o
	@echo "** Building server_side"
	gcc -o server_side $(BUILD_DIR)/main.o $(BUILD_DIR)/player.o $(BUILD_DIR)/application.o $(BUILD_DIR)/linked_list.o $(BUILD_DIR)/server_controller.o $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -rf $(BUILD_DIR) server_side
