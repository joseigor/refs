# Course Topics

## Show the process involved in build a simple project.

- Create a simple calculator program build/compine direct with gcc.
- Folder structure:
  - add.hpp
  - add.cpp
  - divide.cpp
  - divide.hpp
  - multiple.cpp
  - multiple.hpp
  - subtract.hpp
  - subtract.cpp
  - calculator.cpp

## Introduce the 4 stages involved when build a program
- Show that actually gcc is performing the following actions in the background:
  - Preprocessing
  - Compilation
  - Assembly
  - Building
- Go through the 4 steps above manually, and show that it can be quite
time consuming and error prone, not easy to maintain a big project using
this approch.

## Introduce Make and Ninja
- show the prupose of Make and Ninja (called generators in cmake) and how
they can help to build a project in an easier way, but still quite some
work is involved.
- build the same calculator program above but using Make and CMake

## Introduction to CMake
- Create a simple main.cpp program to demostrate the basics of cmake
- Here introduce the basic idea behind cmake
- source and out-source building
- configuring and generating phase
- how to create a very simple executable
- how to choose the generator

## Configure the CMakeLists.txt for the calculator project
- Here we just show the steps above but for the calculator project
- We can also move the .h/.cpp file to an appropiated folder(s) so we
can use target_include_directories.

## Building Simple Targets

### How to create a simple executable
Explain the command:
```cmake
add_executable(targetName [WIN32] [MACOSX_BUNDLE]
[EXCLUDE_FROM_ALL]
source1 [source2 ...]
)
```
The idea here is to create another simple target (a target that tests the mathlib with some asserts that we only build for tests) for out calculator APP
and show by default that cmake will build all the targets if none is specified.
`EXCLUDE_FROM_ALL` can be used to remove a target from the `ALL` detfault rule, so the target is only built when requested. This is util for a project where we do not want to build a target occasionally.


### How to create librarries
Explain the command:

```cmake
add_library(targetName [STATIC | SHARED | MODULE]
[EXCLUDE_FROM_ALL]
source1 [source2 ...]
)
```
In this point we can create a math library that will container the operations
used by the calcualtor. Also explain `[STATIC | SHARED | MODULE]` and for sake
of example show how to generate a STATIC and SHARED library by:
- setting the type in the `add_library`
- using `cmake -DBUILD_SHARED_LIBS=YES /path/to/source` or `set(BUILD_SHARED_LIBS YES)`

### How to link targets

In here we want to link our executable (calculator) to the library 
create previously.

Explain the idea behind `PRIVATE-PUBLIC-INTERFACE` for managing dependencies.
We can refactor the `mymath` library so it links to others small refactored libs with `PRIVATE/PUBLIC/INTERFACE` for the sake of example. SOmething like
below:

```cmake
add_library(mymath src1.cpp)
add_library(add add.cpp)
add_library(divide divide.cpp)
add_library(subtract subtract.cpp)
add_library(multiply.cpp)

target_link_libraries(mymath
PUBLIC ...
PRIVATE add divide subtract multiply
INTERFACE ...
)

add_executable(calculator main.cpp)
target_link_libraries(calculator PRIVATE mymath)
```

### Recommended practices 
- Do no use the same of the targets the same name of the project, This can work for the most basic projects. A project should be thinked as a collection as targets.
- Names the targets based on the target purpose.
- Set the project name directly instead of using a varaible
- Do not add a `lib` to you libraries targets as this `lib` prefix is already added in most system (Windows is the exeption).
- Use `PUBLIC-PRIVATE-INTERFACE` to explicity scope the link dependencies.
- Unless there are strong reasons to do so, try to avoid specifying the `STATIC` or `SHARED` keyword for a library until it is known to be needed.
This allows greater flexibility in choosing between static or
dynamic libraries as an overall project-wide strategy. The `BUILD_SHARED_LIBS` variable can be used to change the default in one place instead of having to modify every call to `add_library()`.


## Variable

### Normal variables

We here we can show how to set varaibles in different ways, how to unset
and we can also create a varaible that is set if we want to build the test target.

A varaible can be unset by : set(myvar) or unset(myvar)

### Environment Variables
 CMake allows to set environment variables in the same way as internal varaibles, but instead
 of the simple variable name it needs to be enclosed in a ENV{} like this `ENV{myvar}`.

```cmake
 set(ENV{PATH} "$ENV{PATH}:/opt/myDir")
 ```

 > Note: The scope of the modified environment varaible is only valid when CMAKE is running and for the `configuration` and `generation` phase, but not for the `build` phase.

> After the introdution of `internal variables` and `env. variables` we can show that they do not persist over multiple CMAKE runs and specially that modified `env. variables` do not persist during the build.  

### Cache Variable (I think we need some classes to cover only cache variables)

- Is persistent accross multipe CMake runs
- Saved under CMakeCache.txt in the build directory
- Can be set with the following syntax: 
```cmake
set(varName value... CACHE type "docstring" [FORCE])
```
- `type` can be of :  BOOL, FILEPATH, PATH, STRING, INTERNAL
- In reality, the type is only use by GUI tools to present the variables to the user in a proper way, internall CMAKE treats all cache varaibles as `strings`.
- CMAKE has an special syntax (a syntax sugar) for `BOOL` cache variables :
```cmake
option(varName helpString [initialValue])
# line above is equivalent to
set(varName initialValue CACHE BOOL helpString)
```

> An important difference between normal and cache variables is that the set() command will only
overwrite a cache variable if the FORCE keyword is present, unlike normal variables where the set()
command will always overwrite a pre-existing value. The set() command acts more like set-if-notset
when used to define cache variables, as does the option() command (which has no FORCE
capability). The main reason for this is that cache variables are primarily intended as a
customisation point for developers. Rather than hard-coding the value in the CMakeLists.txt file as
a normal variable, a cache variable can be used so that the developer can override the value
without having to edit the CMakeLists.txt file. The variable can be modified by interactive GUI tools
or by scripts without having to change anything in the project itself.

> A point that is often not well understood is that normal and cache variables are two separate
things. It is possible to have a normal variable and a cache variable with the same name but
holding different values. In such cases, CMake will retrieve the normal variable’s value rather than
the cache variable when using ${myVar}, or put another way, normal variables take precedence over
cache variables. The exception to this is that when setting a cache variable’s value, if that cache
variable did not exist before the call to set() or if the FORCE option was used, then any normal
variable in the current scope is effectively removed. Note that this unfortunately means it is
possible to get different behavior between the first and subsequent CMake runs, since in the first
run, the cache variable won’t exist, but in subsequent runs it will. Therefore, in the first run, a
normal variable would be hidden, but in subsequent runs it would not. An example should help
illustrate the problem.

```CMake
set(myVar foo) # Local myVar
set(result ${myVar}) # result = foo
set(myVar bar CACHE STRING “”) # Cache myVar
set(result ${myVar}) # First run: result = bar
# Subsequent runs: result = foo
set(myVar fred)
set(result ${myVar}) # result = fred
```

#### Manipulating cache variables
```sh
cmake -D myVar:type=someValue ...
cmake -D foo:BOOL=ON ...
cmake -D "bar:STRING=This contains spaces" ...
cmake -D hideMe=mysteryValue ...
cmake -D helpers:FILEPATH=subdir/helpers.txt ...
cmake -D helpDir:PATH=/opt/helpThings ...

# remove cache variable
cmake -U 'help*' -U foo ...
```

> Show also hot to use the `cmake-gui`, it shows 2 buttons `configure` and `generate` because in CMake
first configure the project in which a memory representation of the project is created and the we CMake 
generated the build files in the `generate` phase.


## Debugging Variables And Diagnostics

```cmake
message([mode] msg1 [msg2]...)
set(myVar HiThere)
message("The value of myVar = ${myVar}")
# Modes : STATUS, WARNING, AUTHOR_WARNING, SEND_ERROR, FATAL_ERROR, DEPRECATION
variable_watch(myVar [command]) # watch variable for changes
```


## String Manipulation

```cmake
string(FIND inputString subString outVar [REVERSE])
# example
set(longStr abcdefabcdef)
set(shortBit def)
string(FIND ${longStr} ${shortBit} fwdIndex)
string(FIND ${longStr} ${shortBit} revIndex REVERSE)
message("fwdIndex = ${fwdIndex}, revIndex = ${revIndex}")

# Regular expression are supported
string(REGEX MATCH regex outVar input [input...])
string(REGEX MATCHALL regex outVar input [input...])
string(REGEX REPLACE regex replaceWith outVar input [input...])

#example
set(longStr abcdefabcdef)
string(REGEX MATCHALL "[ace]" matchVar ${longStr})
string(REGEX REPLACE "([de])" "X\\1Y" replVar ${longStr})
message("matchVar = ${matchVar}")

# Other examples f string manipulation
string(SUBSTRING input index length outVar)
string(LENGTH input outVar)
string(TOLOWER input outVar)
string(TOUPPER input outVar)
string(STRIP input outVar)
```


## Lists
```cmake
list(LENGTH listVar outVar)
list(GET listVar index [index...] outVar)

# example
set(myList a b c) # Creates the list "a;b;c"
list(LENGTH myList len)
message("length = ${len}")
list(GET myList 2 1 letters)
message("letters = ${letters}")

# output
length = 3
letters = c;b


# append and insert
list(APPEND listVar item [item...])
list(INSERT listVar index item [item...])

set(myList a b c)
list(APPEND myList d e f)
message("myList (first) = ${myList}")
list(INSERT myList 2 X Y Z)
message("myList (second) = ${myList}")

#output
myList (first) = a;b;c;d;e;f
myList (second) = a;b;X;Y;Z;c;d;e;f
Finding
```

## Maths

```cmake
math(EXPR outVar mathExpr)
set(x 3)
set(y 7)
math(EXPR z "(${x}+${y}) / 2")
message("result = ${z}")

# supported operators
# + - * / % | & ^ ~ << >> * / %.
```


## Check Recommended practices in chapter 5


## Chapter 6

### `if` command

```cmake
if(expression1)
  # commands ...
elseif(expression2)
  # commands ...
else()
  # commands ...
endif()

#-----
# Examples of unquoted constants
if(YES)
if(0)
if(TRUE)
# These are also treated as unquoted constants because the
# variable evaluation occurs before if() sees the values
set(A YES)
set(B 0)
if(${A}) # Evaluates to true
if(${B}) # Evaluates to false
# Does not match any of the true or false constants, so proceed
# to testing as a variable name in the fallthrough case below
if(someLetters)
# Quoted value, so bypass the true/false constant matching
# and fall through to testing as a variable name or string
if("someLetters")

#-----
# Common pattern, often used with variables defined
# by commands such as option(enableSomething "...")
if(enableSomething)
  #...
endif()

#-----
### Logic operators
# Logical operators
if(NOT expression)
if(expression1 AND expression2)
if(expression1 OR expression2)
# Example with parentheses
if(NOT (expression1 AND (expression2 OR expression3)))

#-----
## Comparison test
if(value1 OPERATOR value2)

# Numeric           String                Version numbers
# LESS              STRLESS               VERSION_LESS
# GREATER           STRGREATER            VERSION_GREATER
# EQUAL             STREQUAL              VERSION_EQUAL
# LESS_EQUAL1       STRLESS_EQUAL1        VERSION_LESS_EQUAL1
# GREATER_EQUAL1    STRGREATER_EQUAL1     VERSION_GREATER_EQUAL1

# Valid numeric expressions, all evaluating as true
if(2 GREATER 1)
if("23" EQUAL 23)
set(val 42)
if(${val} EQUAL 42)
if("${val}" EQUAL 42)
# Invalid expression that evaluates as true with at
# least some CMake versions. Do not rely on this behavior.
if("23a" EQUAL 23)

# expression below evaluates to true
if(1.2 VERSION_EQUAL 1.2.0)
if(1.2 VERSION_LESS 1.2.3)
if(1.2.3 VERSION_GREATER 1.2 )
if(2.0.1 VERSION_GREATER 1.9.7)
if(1.8.2 VERSION_LESS 2 )

#-----
## Regular expressions
if(value MATCHES regex)

if("Hi from ${who}" MATCHES "Hi from (Fred|Barney).*")
message("${CMAKE_MATCH_1} says hello")
endif()

#-----
## Files system tests

if(EXISTS pathToFileOrDir)
if(IS_DIRECTORY pathToDir)
if(IS_SYMLINK fileName)
if(IS_ABSOLUTE path)
if(file1 IS_NEWER_THAN file2)

#-----
## Existence tests

if(DEFINED name)
if(COMMAND name)
if(POLICY name)
if(TARGET name)
if(TEST name) # Available from CMake 3.4 onwards
if(value IN_LIST listVar)
```

### Looping

#### `foreach`

```cmake
foreach(loopVar arg1 arg2 ...)
# ...
endforeach()

foreach(loopVar IN [LISTS listVar1 ...] [ITEMS item1 ...])
# ...
endforeach()

set(list1 A B)
set(list2)
set(foo WillNotBeShown)
foreach(loopVar IN LISTS list1 list2 ITEMS foo bar)
message("Iteration for: ${loopVar}")
endforeach()

# output
Iteration for: A
Iteration for: B
Iteration for: foo
Iteration for: bar

#----
### Range
foreach(loopVar RANGE start stop [step])
foreach(loopVar RANGE value)
```

### `while`

```cmake
while(condition)
# ...
endwhile()
```

### Interrupting loops

```cmake
foreach(outerVar IN ITEMS a b c)
  unset(s)
  foreach(innerVar IN ITEMS 1 2 3)
    # Stop inner loop once string s gets long
    list(APPEND s "${outerVar}${innerVar}")
    string(LENGTH s length)

    if(length GREATER 5)
      break() ①
    endif()

    # Do no more processing if outer var is "b"
    if(outerVar STREQUAL "b")
      continue() ②
    endif()

    message("Processing ${outerVar}-${innerVar}")
  endforeach()

  message("Accumulated list: ${s}")
endforeach()

# output
Processing a-1
Processing a-2
Processing a-3
Accumulated list: a1;a2;a3
Accumulated list: b1;b2;b3
Processing c-1
Processing c-2
Processing c-3
Accumulated list: c1;c2;c3
```

# Chapter 7 : Using subdirectories






