param(
    [switch]$Release,
    [switch]$Regenerate
)

$BuildType = "Debug"

if ($Release)
{
    $BuildType = "Release"
}

if ($Regenerate)
{
    # Generate and fetch compile_commands.json
    Remove-Item .\build -Recurse -ErrorAction SilentlyContinue
    cmake -S . -B .\build -G "Ninja"
    Remove-Item .\compile_commands.json -ErrorAction SilentlyContinue
    Copy-Item -Path .\build\compile_commands.json -Destination .\compile_commands.json

    # Finally generate makefiles for MSVC
    Remove-Item .\build -Recurse -ErrorAction SilentlyContinue
    cmake -S . -B .\build -DCMAKE_BUILD_TYPE=$BuildType -G "Visual Studio 17 2022"
}

# Build the project and fetch the executable
Remove-Item hardlinkcpp.exe -ErrorAction SilentlyContinue
cmake --build build --config $BuildType

Copy-Item -Path .\build\$BuildType\hardlinkcpp.exe -Destination .\hardlinkcpp.exe
