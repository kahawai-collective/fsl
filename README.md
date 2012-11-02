# FiSL : Fisheries Simuation Library

*FiSL* is a C++ library for fisheries simulation.
It aims to be highly modular library without compromising speed.
It does that by making the most of modern C++ language features such as templates
and type inference, as well as recent advances in compiler optimisation. 

Because *FiSL* is modular it can be used for general fisheries simulation.
However, the main purpose for *FiSL* is as a library for fisheries management procedure evaluation (MPE).
*FiSL* provides C++ classes that can be composed in alternative ways to 
provides for alternative types of MPE range from simple to complex.

## Documentation

As far as possible all documentation is contained within comment blocks in 
C++ files.
Having documentation in close proximity to code makes it much easier
to keep the two in sync.

### Writing documentation
 
Block equations can be inserted by surrounding them using the `\f[` and `\f]`
e.g.

    \f[ y = a + bx \f]
    
For inline equations should be sourrounded by the `\f$` tag e.g.

    where \f$b\f$ is the slope

### Generating documentation

Two programs are used for converting the C++ comments into readable documentation.
Each has its own advantages.

#### [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html)

Doxygen is a documentation tool that is extensively ised for documenting C++ code.
It has the advantage that of auto-documentation: it parses the C++ code and produces
a large volume of documentation from it. Whilst the documentation that Dosygen produces is
very comprehensive it can be a bit daunting to non-programmers.

You can download Doxygen [here](http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc).
See `requirements/Makefile` for quick installation for Linux.

#### Fislgen

Fislgen is a Python script written for FiSL (hence the name!).
It extracts sections of documentation written using the [Markdown language](http://daringfireball.net/projects/markdown).
Markdown allows for very easy formatting of text.
The purpose of Fislgen is to create more customised, less busy, documentation for the library,
targetted at people that are not programmers.

### Publishing documentation

Currently, documentation is published to a [Github Project Page](http:http://pages.github.com/) using the 
`gh-pages` branch and is available at <a href="http://trident-systems.github.com/fisl/index.html">http://trident-systems.github.com/fisl/</a>.
To generate and publish documentation do (see the Makefile for a few requirements):

```sh
cd  documentation
make publish
```
