# RVR

Playground for writing code for the galaxy-rvr in C++

### Required packages

Install `gcc-avr`, `binutils-avr`, `avr-libc` and `avrdude`:

```shell
  sudo apt-get install gcc-avr binutils-avr avr-libc avrdude
```

### Build

In the project root:

```shell
  mkdir build
  cd build
  cmake ..
  cmake --build .
```