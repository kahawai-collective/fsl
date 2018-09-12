# FSL : Fisheries Simulation Library

FSL is a C++ library for fisheries simulation.
It aims to be highly modular library without compromising speed.
It does that by making the most of modern C++ language features such as templates
and type inference, as well as recent advances in compiler optimisation. 

Because FSL is modular it can be used for general fisheries simulation.
However, the main purpose for FSL is as a library for fisheries management procedure evaluation (MPE).
FSL provides C++ classes that can be composed in alternative ways to 
provides for alternative types of MPE range from simple to complex.

## Setting up

On most Linux systems you should have the required build tools installed already and you should be able to run:

    make tests

On Windows, the recommended approach is to build using MSYS2 and MinGW64:

1. Download and install MYSYS2 following the instructions at https://msys2.github.io/

2. Open the MYSY2 shell and install build tools:

    pacman -S make mingw-w64-x86_64-toolchain zip

3. Open the MinGW64 shell and run:

    make tests

## Documentation

### Doxygen

Doxygen is a documentation tool that is extensively used for documenting C++ code.
It has the advantage of auto-documentation: it parses the C++ code and produces
a large volume of documentation from it. Whilst the documentation that Doxygen produces is
very comprehensive it can be a bit daunting to non-programmers.
To generate doxygen (requires that [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) is installed):

You can download Doxygen [here](http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc).

```sh
make doxygen
```

### Publishing

Currently, documentation is published to a [Github Project Page](http:http://pages.github.com/) using the 
`gh-pages` branch and is available at <a href="http://trident-systems.github.com/fsl/index.html">http://trident-systems.github.com/fsl/</a>.

To publish documentation (requires that [`ghp-import`](https://pypi.python.org/pypi/ghp-import) is installed):

```sh
cd  docs
make publish
```
