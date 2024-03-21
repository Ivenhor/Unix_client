# Unix Server

The Unix Server is a companion project to the Unix Client, providing the server-side functionality for communication with the client via Unix sockets.

## Getting Started

Before running the Unix Client, ensure that the Unix Server is up and running. You can find the repository for the Unix Server [here](https://github.com/Ivenhor/Unix_server). Follow the instructions in the Unix Server repository to build and start the server.

## Supported Commands

The server can process the following commands received from the client:

- `date`: Returns the current date.
- `time`: Returns the current time.
- `exit`: Closes the client connection and exits the server.
- `shutdown`: Shuts down the server.

## Building and Running

### Prerequisites
- C/C++ compiler
- CMake

### Steps to Build and Run
1. Open the project in VS Code.
2. Install the CMake Tools extension if not already installed.
3. Press `Ctrl+Shift+P` to open the command palette.
4. Select CMake: Configure to configure the project.
5. Build the project by selecting CMake: Build. Or `cmake run . Unix_client`.
6. Run the client application by executing the generated executable `./Unix_client`.
7. Ensure that the server is running before starting the client. Refer to the Unix Server repository for instructions on setting up and running the server.
8. To run tests, use VS Code test tools or CMake tools ctest.
9. Clone the Unix Server repository from [here](https://github.com/Ivenhor/Unix_server).
10. Navigate to the server directory.
11. Follow the instructions in the server repository to build and start the server.

## Usage
- Once the server is running, you can run the Unix Client to connect to the server and send commands.
