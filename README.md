
# HardLinkCpp

HardLinkCpp is a C++ utility designed to recursively create hard links for
files from a source directory into a destination directory.

## Features

- Recursively traverses the source directory.
- Creates hard links in the destination directory for each file found in the source.
- Preserves the directory structure in the destination.

## Requirements

- C++17 or later.
- [Meson](https://mesonbuild.com/) build system.
- A C++ compiler compatible with C++17.

## Building the Project

1. **Clone the Repository:**

```bash
git clone https://github.com/Zira3l137/HardLinkCpp.git
cd HardLinkCpp
```

2. **Configure the Build Environment:**

```bash
meson setup build
```

This command sets up the build directory and configures the project using Meson.

3. **Build the Project:**

After configuration, use the following commands to build the project:

```bash
cd build
meson compile
```

These commands compile the project and place the executable in the build directory.

## Usage

Once built, you can use the executable to create hard links from a source directory
to a destination directory. The basic syntax is:

```bash
HardLinkCpp -s <source_directory> -o <destination_directory> -i <ignore_pattern> -d <debug_mode>
```

Example:

```bash
HardLinkCpp -s "C:\path\to\source" -o "C:\path\to\destination" -i "*.txt" -d 0
```

This command will recursively traverse `C:\path\to\source` and create hard links
for all files in `C:\path\to\destination`, preserving the directory structure and ignoring
any files that match the pattern `*.txt`. Additionally, the debug mode will be set to 0 (all).
Which means that all log messages will be printed to the console.

Both `-i` and `-d` flags are optional.
`-d` flag (stands for debug) will set internal debug mode (0 = all, 1 = info and errors, 2 = errors only 3 = none).
Providing the `-i` flag will ignore files that match the pattern.
`*` before the pattern will match files ending in the pattern. Example: `*.txt`
`*` after the pattern will match files that start with the pattern. Example: `prefix*`

## Notes

- Ensure that the source and destination directories reside on
the same filesystem, as hard links cannot be created across different filesystems.
- Be cautious when using this utility, as creating hard links means that changes
to one file will affect all linked instances.

## License

This project is licensed under the MIT License, see the [LICENSE](https://opensource.org/license/mit) file for details.

## The purpose of this project

The purpose of this project is purely educational, as I am currently learning C++. This is
just a simple solution to get me started.

