# FSL : Fisheries Simulation Library

FSL is a C++ library for fisheries simulation.
It aims to be highly modular library without compromising speed.
It does that by making the most of modern C++ language features such as templates
and type inference, as well as recent advances in compiler optimisation. 

Because FSL is modular it can be used for general fisheries simulation.
However, the main purpose for FSL is as a library for fisheries management procedure evaluation (MPE).
FSL provides C++ classes that can be composed in alternative ways to 
provides for alternative types of MPE range from simple to complex.

## Organisation



## Documentation

`fsl/<grandparent>/<parent>/<module>`

Each module should have the following files:

- A module __must__ have a C++ header file with a name `<module>.hpp`

- A module __should__ have a C++ testing file with a name `<module>.cxx`

- A module __should__ have a documentation files with a name `<module>.cila`


```sh
make population/recruitment/ricker.module
```

```
fsl/population/recruitment/ricker/
├── ricker.cila
├── ricker.cxx
└── ricker.hpp
```

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
