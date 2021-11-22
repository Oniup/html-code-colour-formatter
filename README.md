# Code colour Formatter for HTML code examples

## About

The purpose of the Program is to provide colour formatted code that can be passed into a html file and render the correct colours assigned the the different keywords within the code. Doing this by hand is very time consuming, thus the creation of this program. 
A use for the program is the initial inspiration for the development is the use it for providing code examples when explaining what a specific function does within the API you are creating, or when you are teaching but want the code to be stylised. 

How it works is that it wraps keywords in span containers and assigns a class which you will have to define in the css file what colours or style the container represents.
Example:

original: 
``` float n = 1.0f; ```

formatted version: 
``` <span class="c dt">float</span> n = <span class="c n">1.0f</span>; ```

## Project Status

#### Build Status

The program is current in development (starting on the 17/11/2021). 
currently working on formatting .c code (which is currently working however with some missing features and very buggy)
Also this it the first time I am writing any kind of parsing function so bugs are expected

#### Known Bugs

there are 4 main bugs which would be annoying to fix if you pass through a lot of code. This are currently being worked on

* custom data types do not format. This means if you create a struct, that struct will just be rendered as normal 
  * example: ```<span class="c cdt">...</span>``` (it will not do this)
* #include <...> formatting is broken. It does't not assign the string formatting to it. Secondly, it assigns the parented formatting to the .h. You will have to fix ever #include line to the correct formatting. *note: this is the only parse that is completely broken, at least the #include works*
  * example: ```<span class="c dt">#include</span> &gt;stdlib.<span class="c p">h</span>&lt;```
* when using a function and there is a "(" || ")" || "," || ";" before, the program will wrap that in the function container
  * example: ```(<span class="c f">char*)malloc</span>(<span class="c f">sizeof</span>(char*) * <span class="c n">45.5f)</span>```
  * there are a couple problems within this line such as:
    * (char*) should be: ```(<span class="c dt">char*</span>)```
    * ```<span class="c n">45.5f)</span> should be: <span class="c n">45.5f</span>)```
* brackets mess with the parsing
  * examples are found above

##### What The Program Supports Now 

* [x] c (currently working on)
* [ ] cpp
* [ ] cs
* [ ] py
* [ ] java

## Setup

all you have to do is compile with any compiler. Make sure that the compiler compiles at source_location\src and include the source_location\src\headers 

#### example of compiling through MinGW:

``` 
C:\...\MinGW\mingw64\bin\gcc.exe
C:\...\src\*.c 
-IC:\...\src\headers
-o C:\...\build\code_formatting_program.exe
```
