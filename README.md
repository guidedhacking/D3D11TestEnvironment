# GH D3D11 Test Environment

This repository contains two dedicated test environments for Direct3D 9 and Direct3D 11 developed in C++. These environments are designed to provide a platform for beginner hack programmers to practice function hooking and other techniques using the DirectX API, a popular Windows graphics API for drawing 3-dimensional objects. The goal here is to enable learning and experimentation without the risk of penalties associated with live game testing, such as kicks and bans.

The Direct3D 11 test environment creates a window, initializes DX, and enters the main loop where messages are processed, requiring more null pointers declarations and usage of the DirectX namespace compared to D3D9. While the Direct3D 11 environment may be more complex, it provides an excellent opportunity to deepen your understanding of DirectX's intricacies.

Please note that the D3D9 project relies on the DXSDK_DIR macro. If you encounter any issues, try adding a new entry to your PATH variable with the name DXSDK_DIR, and the value being the path to the SDK on disk (don't forget the trailing '\').

Feel free to use these environments as your sandbox for practicing and experimenting. Happy Coding!

[More info @ GuidedHacking](https://guidedhacking.com/threads/d3d9-11-test-environments.11934/)
