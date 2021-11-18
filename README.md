# Code colour Formatter for HTML code examples

## About

the purpose of this program is to increase speed in creating code examples in the documentation by automatically assigning the correct colours to the text. At this moment, for me to create example code snippets to explain how something works is very time consuming and I would like to have a faster way of doing the same thing. Right now you would have to write down the C code and then in the html file, go through it and add in a bunch of "span" containers setting the correct colour of that word, this program automates that.

## Project Status

#### Build Status

The program is current in development (starting on the 17/11/2021). 
currently working on formatting .c code

##### What The Program Supports Now 

* [] .c (currently working on)
* [] .cpp
* [] .cs
* [] .py
* [] .java

## Setup

all you have to do is compile with any compiler. Make sure that the compiler compiles at source_location\src and include the source_location\src\headers 

#### example of compiling through MinGW:

``` 
C:\...\MinGW\mingw64\bin\gcc.exe
C:\...\src\*.c 
-IC:\...\src\headers
-o C:\...\build\code_formatting_program.exe
```
