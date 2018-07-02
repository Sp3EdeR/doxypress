## CopperSpice

### Introduction
CopperSpice is of a set of C++ libraries used to develop cross-platform software applications. This is an open source project
released under the LGPL V2.1 license. CopperSpice was derived from the Qt framework. Our motivation for developing CopperSpice was to
change the core design and leverage modern C++ functionality.

The libraies provided in CopperSpice include:

 * CsCore
 * CsGui
 * CsMultimedia
 * CsNetwork
 * CsOpenGL
 * CsPhonon
 * CsScript
 * CsSql
 * CsSvg
 * CsWebKit
 * CsXml
 * CsXmlPatterns

There are also three open source BSD licensed libraries which are used by CopperSpice and available as stand alone libraries
for anyone developing a C++ application.

 * CsSignal
 * CsString
 * libGuarded


### Major enhancements

Reflection
 * No Meta-Object Compiler is required for generating meta data, all references were removed
 * The functionality provided by moc was replaced with compile time templates
 * CopperSpice automatically generates meta data for processing Signals/ Slots and Introspection
 * A template class can now inherit from QObject with no restrictions on types
 * Complex data types such as **%QMap&lt;%QString, int&gt;** can be used without restriction

Enhanced Functionality
 * CopperSpice makes extensive use of modern C++ features like variadic templates, constexpr, SFINAE, lambdas, atomics,
   tuple, etc
 * Reimplemented container classes in CsCore using the C++ standard library containers, iterators, and algorithms
 * CopperSpice includes several of the Qt 5 classes
 * Integration of CsSignal
   * Improved thread aware Signal/Slot delivery
   * Increased efficiency while maintaining the full Signal/Slot API
 * Integration of CsString
   * Improved storage of Unicode strings
   * %QString8 (UTF-8) and QString16 (UTF-16)

Using the Libraries
 * Any application using CopperSpice can be built with CMake, GNU Autotools, or any other build system
 * CopperSpice can be linked directly into any standard C++ application


### System Requirements

To use the CopperSpice libraries you will need a C++14 compiler and a C++14 standard library.


### Building

The CopperSpice libraries can be built using CMake or GNU Autotools.


### Presentations

Our YouTube channel videos are about modern C++, build systems, CopperSpice, DoxyPress, and other topics related to our
open source work.

https://www.youtube.com/copperspice

<br>
