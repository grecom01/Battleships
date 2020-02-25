project(Server)

find_package(SFML 2 COMPONENTS system network)

add_executable(Battleships_Server)

target_sources(Battleships_Server PRIVATE
	src/main.cpp
	src/Server.h
	src/Server.cpp
	src/MessageType.h
	src/Player.h
	src/Player.cpp
	src/Ship.h
	src/Ship.cpp
)

target_compile_features(Battleships_Server PRIVATE cxx_std_17)

target_link_libraries(Battleships_Server sfml-network)
