# ps2algo

A simple converter for linux that converts from a simple pseudo code format,
into human readable algorithm instructions!

Written to solve for people who are lazy to write long algorithm text, and just want to write code.
It's actually just a funny little parsing project, I am not intending to improve or maintain this.

## prerequisite
your need `make`, so install make for yourself.

### Arch linux:
```
pacman -S make
```

### Debian based:
```
apt install make
```

or directly download binaries from [here](https://gnuwin32.sourceforge.net/packages/make.htm)

## building
1. clone this repository
2. change directory into the root project folder where `makefile` is located.
3. then simply run `make`

## usage
by running `ps2algo` it will ask for a pseudo code file.

```
./ps2algo <pseudo code file path>
```

the format of pseudo code is simple:
```
in a: num1;
in b: num2;

sum = a + b;
avrg = sum / 2;

out avrg;
```

when saved in a file called `pseudo.ps`

and ran using
```
./ps2algo pseudo.ps
```

will output something like this:
```
1. START
2. Input num1 as variable a
3. Input num2 as variable b
4. Add a with b then assign into the variable sum
5. Divide sum with 2 then assign into the variable avrg
6. Return avrg
7. END
```

have fun!
