
# HardLinkCpp

HardLinkCpp is a C++ utility designed to recursively create hard links for
files from a source directory into a destination directory. This can help
save disk space by linking identical files instead of duplicating them.

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
HardLinkCpp <source_directory> <destination_directory>
```

Example:

```bash
HardLinkCpp C:\path\to\source C:\path\to\destination
```

This command will recursively traverse `C:\path\to\source` and create hard links
for all files in `C:\path\to\destination`, preserving the directory structure.

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

