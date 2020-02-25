project(Client)

find_package(SFML 2 COMPONENTS graphics window system audio network)
find_package(TGUI CONFIG REQUIRED)

add_executable(Battleships_Client)

target_sources(Battleships_Client PRIVATE
	src/main.cpp
	src/Application.h
	src/Application.cpp
	src/State.h
	src/State.cpp
	src/StateManager.h
	src/StateManager.cpp
	src/AssetManager.h
	src/AssetManager.cpp
	src/Network/ServerConnection.h
	src/Network/ServerConnection.cpp
	src/Network/ServerMessageQueue.h
	src/Network/ServerMessageQueue.cpp
	src/Network/MessageType.h
	src/Login.h
	src/Login.cpp
	src/LoginUI.h
	src/LoginUI.cpp
	src/Game.h
	src/Game.cpp
	src/GameUI.h
	src/GameUI.cpp
	src/Grid.h
	src/Grid.cpp
	src/Ship.h
	src/Ship.cpp
	src/DeploymentPhase.h
	src/DeploymentPhase.cpp
	src/PlayerMovePhase.h
	src/PlayerMovePhase.cpp
	src/EnemyMovePhase.h
	src/EnemyMovePhase.cpp
	src/GameOverPhase.h
	src/GameOverPhase.cpp
	src/Fleet.h
	src/Fleet.cpp
	src/Singleton.h
)

target_include_directories(Battleships_Client PRIVATE "src/")

target_compile_features(Battleships_Client PRIVATE cxx_std_17)

target_link_libraries(Battleships_Client PRIVATE
	sfml-graphics
	sfml-audio
	sfml-network
	tgui
)
