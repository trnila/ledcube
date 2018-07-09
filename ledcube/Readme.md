# ledcube for STM32F103

## Build

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
```

## Flash
### OpenOCD
```sh
$ openocd -f ../ocd-maple-stlink.cfg -c "init" -c "reset init" -c "flash write_image erase ledcube.bin 0x08000000" -c "reset" -c "exit"
```

### st-util + gdb
```sh
$ st-util
...

$ arm-none-eabi-gdb ledcube.elf 
(gdb) target extended-remote :4242
(gdb) load
(gdb) cont
