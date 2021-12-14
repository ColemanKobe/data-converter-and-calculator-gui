# Table of Contents

- [Environment Setup](#environment-setup)
- [Converter](#converter)
- [Calculator](#calculator)
- [Contribution](#contribution)

## Environment Setup

No matter what platform you're using, it's highly recommended to use Linux for GTK development in C. Even if you have Windows,
use a Windows Subsystem for Linux. [This link](https://docs.microsoft.com/en-us/windows/wsl/install) will guide you on how
to enable it on your machine. Then, you should be able to access a Linux terminal of some sort if you don't have one already.

Install the GTK3 libraries:

`sudo apt-get install libgtk-3-dev`

Install the C compiler itself:

`sudo apt-get install gcc`

Install the Geany Text Editor:

`sudo apt-get install geany`

## Converter

**Compile command:**

```
gcc `pkg-config --cflags gtk+-3.0` -o converter converter.c `pkg-config --libs gtk+-3.0` -lm
```

**Allowed input:**

decimal -> binary: less than or equal to 99998
decimal -> hex: less than or equal to 999999999

binary -> decimal: 20 bits or less
binary -> hex: 20 bits or less

hex -> decimal: less than or equal to fffffff
hex -> binary: less than or equal to ffff

## Calculator

**Compile command:**

```
gcc `pkg-config --cflags gtk+-3.0` -o calculator calculator.c `pkg-config --libs gtk+-3.0` -lm
```

**Allowed input:**

Adding -> 16 bits each
Subtracting -> only positive results allowed (top must be greater than bottom)

## Contribution

Additional enhancements to the calculator and converter are always welcome! Just try to follow the [GitHub Flow guidelines](https://docs.github.com/en/get-started/quickstart/github-flow)
while doing so.