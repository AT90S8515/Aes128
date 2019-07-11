# Aes128 ![Version 1.0](https://img.shields.io/badge/Version-1.0-brightgreen.svg) ![Build passed](https://img.shields.io/badge/Build-Passed-brightgreen.svg)

Aes128 is a library for encrypting and decrypting files with the AES CBC algorithm with a key length of 128 bits. The library supports PC class devices and 8-bit AVR microcontrollers. ![PC](https://img.shields.io/badge/PC-blue.svg) ![AVR](https://img.shields.io/badge/AVR-blue.svg)

## Compiling

To use the library, just attach "Aes128.h" and "Aes128.cpp" to the project and that's it.

## Building standalone app (PC)

To build standalone application for desktop use premake.

Example:
```
premake5 gmake2
make
```


## License
MIT License

Copyright (c) 2019 Xoliper

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
