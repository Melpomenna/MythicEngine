<p align="center">
  <a href="https://dev.azure.com/your-organization/your-project/_build">
    <img src="Misc/MythinEngineMain.png" alt="MythicEngine Main" />
  </a>
</p>

# MythicEngine

[![Build Status](https://dev.azure.com/mellpomeNn/MythicEngine/_apis/build/status%2FMelpomenna.MythicEngine?branchName=main)](https://dev.azure.com/mellpomeNn/MythicEngine/_build/latest?definitionId=3&branchName=main)

MythicEngine is an open-source, lightweight game engine currently under active development. The project aims to provide a modern, modular foundation for game creation using C++.

## Features

- Written in modern C++ (C++17/C++23)
- Cross-platform build system using CMake
- Unit tests powered by Google Test (GTest)
- Designed for extensibility and ease of use

## Building MythicEngine

### Prerequisites

- **CMake** >= 3.25.1
- **Visual Studio 2022** (or compatible C++ compiler)
- **Git** (for cloning the repository)

### Steps

1. **Clone the repository:**
2. **Configure the project with CMake:**
3. **Build the project:**
   - Open the generated solution in Visual Studio (`build/x64-release/MythicEngine.sln` or `build/x64-debug/MythicEngine.sln`)
   - Build the desired configuration (x64-debug/x64-release)

   Or build from the command line:
```
    cmake --preset=x64-release
    cmake --build . --preset=x64-release
```

### Running Tests

Unit tests are implemented using GTest. To run tests:
Or use Visual Studio's integrated test runner.

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, features, or improvements.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Status

**Note:** MythicEngine is in early development. Features and APIs may change frequently.
