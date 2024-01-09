LIBS = -lcsfml-system -lcsfml-network -lcsfml-graphics
BUILD_DIR = makefile-build

all: client_side

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

main.o: main.c | $(BUILD_DIR)
	gcc -c "main.c" -o $(BUILD_DIR)/main.o

application.o: application/application.h application/application.c | $(BUILD_DIR)
	gcc -c "application/application.c" -o $(BUILD_DIR)/application.o

bullet.o: bullet/bullet.h bullet/bullet.c | $(BUILD_DIR)
	gcc -c "bullet/bullet.c" -o $(BUILD_DIR)/bullet.o

map.o: map/map.h map/map.c | $(BUILD_DIR)
	gcc -c "map/map.c" -o $(BUILD_DIR)/map.o

tank.o: tank/tank.h tank/tank.c | $(BUILD_DIR)
	gcc -c "tank/tank.c" -o $(BUILD_DIR)/tank.o

main_menu.o: windows/main_menu/main_menu.h windows/main_menu/main_menu.c | $(BUILD_DIR)
	gcc -c "windows/main_menu/main_menu.c" -o $(BUILD_DIR)/main_menu.o

menu.o: windows/menu/menu.h windows/menu/menu.c | $(BUILD_DIR)
	gcc -c "windows/menu/menu.c" -o $(BUILD_DIR)/menu.o

show_score.o: windows/show_score/show_score.h windows/show_score/show_score.c | $(BUILD_DIR)
	gcc -c "windows/show_score/show_score.c" -o $(BUILD_DIR)/show_score.o

show_status.o: windows/show_status/show_status.h windows/show_status/show_status.c | $(BUILD_DIR)
	gcc -c "windows/show_status/show_status.c" -o $(BUILD_DIR)/show_status.o

button.o: windows/elements/button.h windows/elements/button.c | $(BUILD_DIR)
	gcc -c "windows/elements/button.c" -o $(BUILD_DIR)/button.o

textbox.o: windows/elements/textbox.h windows/elements/textbox.c | $(BUILD_DIR)
	gcc -c "windows/elements/textbox.c" -o $(BUILD_DIR)/textbox.o

linked_list.o: ../common/linked_list/linked_list.c ../common/linked_list/linked_list.h | $(BUILD_DIR)
	gcc -c "../common/linked_list/linked_list.c" -o $(BUILD_DIR)/linked_list.o

server_controller.o: server_controller/server_controller.c server_controller/server_controller.h ../common/base/server_message_types.h ../common/base/type_of_maps.h | $(BUILD_DIR)
	gcc -c "server_controller/server_controller.c" -o $(BUILD_DIR)/server_controller.o

client_side: main.o application.o bullet.o map.o tank.o main_menu.o menu.o show_score.o show_status.o button.o textbox.o linked_list.o server_controller.o | $(BUILD_DIR)
	@echo "** Building client_side"
	gcc -o client_side $(BUILD_DIR)/main.o $(BUILD_DIR)/application.o $(BUILD_DIR)/bullet.o $(BUILD_DIR)/map.o $(BUILD_DIR)/tank.o $(BUILD_DIR)/main_menu.o $(BUILD_DIR)/menu.o $(BUILD_DIR)/show_score.o $(BUILD_DIR)/show_status.o $(BUILD_DIR)/button.o $(BUILD_DIR)/textbox.o $(BUILD_DIR)/linked_list.o $(BUILD_DIR)/server_controller.o $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -rf $(BUILD_DIR) client_side
