Remove-Item .\build -Recurse -ErrorAction SilentlyContinue
cmake -S . -B .\build -G Ninja
Remove-Item .\compile_commands.json -ErrorAction SilentlyContinue
Copy-Item -Path .\build\compile_commands.json -Destination .\compile_commands.json

