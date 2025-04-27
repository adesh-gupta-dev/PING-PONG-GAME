# PING-PONG-GAME
 

A simple C/C++ game built using raylib and Visual Studio.

Getting Started

Follow the steps below to set up your environment and run the project.

1. Clone vcpkg (C++ package manager)

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat

2. Integrate vcpkg with Visual Studio

vcpkg integrate install

This command makes vcpkg automatically available in all your Visual Studio projects.

3. Install raylib using vcpkg

vcpkg install raylib

Note: If you target x64, specify the triplet:

vcpkg install raylib:x64-windows

4. Open the Project in Visual Studio

Open Visual Studio.

Open your ${game} project folder (where your .sln or .vcxproj is located).

Ensure the solution platform is set to x64.

5. Build and Run

Press F5 to build and run with debugging.

Press Ctrl+F5 to run without debugging.

Troubleshooting

If raylib headers aren’t found, verify vcpkg install raylib completed successfully.

If linker errors occur, confirm the platform is set to x64 and re-run:

vcpkg integrate install

License

This project is open source under the MIT License.