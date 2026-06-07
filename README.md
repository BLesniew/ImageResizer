# Resizer

A small interactive image editing tool written in C++ that supports resizing, cropping, display, and file-size-based compression.

## Features

- Resize images by pixel dimensions
- Crop images using coordinates
- Resize image output to a target file size in bytes
- Display the current image in a window
- Save edited images to disk
- Interactive CLI menu for easy use

## Requirements

- C++17 compatible compiler
- CMake 3.10 or newer
- OpenCV development libraries

## Build

Build the project from the repository root:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

If you prefer Docker, the repository includes a `Dockerfile` for building inside an Ubuntu container.

```bash
docker run --rm -it blesniewski/resizer
```

## Usage

Run the executable with an input image and optional output path:

```bash
./Resizer -i <input-image> -o <output-image>
```

Example:

```bash
./Resizer -i lena-alone.jpg -o lena-out.jpg
```

If `-o` is omitted, the program will prompt for an output path when you choose to save.

### Interactive menu options

- `ResizePx` — resize the image by specifying a new width/height in pixels
- `Crop` — crop the image by entering two corner points
- `ResizeFile` — compress/resize the image to meet a target file size
- `Display` — show the image in a display window
- `Save` — persist changes to disk
- `Quit` — exit the program

## Project Structure

- `main.cpp` — program entry point and interactive menu loop
- `common/` — shared utilities and user interface helpers
- `Image/` — image processing implementation using OpenCV
- `Dockerfile` — optional container build environment

## License

This project is released under the MIT License. See `LICENSE` for details.
