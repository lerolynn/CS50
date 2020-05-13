# Questions

## What's `stdint.h`?

`stdint.h` is a header file that declares sets of integer types having specified width requirements along wiht macros specifying their limits and macro functions to create values of these types. It standardizes integers across different systems.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

`uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` define the range of integers, allowing for a more controlled way of defining the memory required to store different integer data types.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

`BYTE` - 1 byte; `DWORD` - 4 bytes, a `LONG` - 8 bytes, and a `WORD` - 4 or 8 bytes, depending on the bit size of the CPU the program is running on. 4 bytes for 32-bit computer, 8 bytes for 64-bit computer,

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first 2 characters of any BMP file is the character "B" then the character "M"

## What's the difference between `bfSize` and `biSize`?

`bfSize` is the full file size of the bitmap image, which includes the header part and the image part. `biSize` is the size of the info header part of the BMP header, the BITMAPINFOHEADER only.

## What does it mean if `biHeight` is negative?

If `biHeight` is negative, the bitmap is a top-down DIB and its origin is in the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount` specifies the number of bits per pixel (bpp).

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

`fopen` might return `NULL` if the file outfile does not exist of if it cannot find the file outfile.

## Why is the third argument to `fread` always `1` in our code? (For example, see lines 40, 44, and 75.)

The third argument to `fread` is the number of elements to accept as input. As `copy.c` reproduces the image pixel-by-pixel, it reads the pixels one at a time hence `fread` accepts 1 pixel argument at a time.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

If `bi.biWidth` is 3,
`padding = (4 - (3 * 3) % 4) % 4;`
`padding = 3;`
sizeof(RGBTRIPLE) = 3 bytes as the RGBTRIPLE struct has 3 elements of size 1 byte each

## What does `fseek` do?

`fseek` changes the file position indicator for the specified stream, allowing the user to move around the file.

## What is `SEEK_CUR`?

`SEEK_CUR` moves the file pointer position to a given location.
