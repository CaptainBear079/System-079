# Getting started with ChaosLang
ChaosLang is a programming and scripting language made for easy and fast software, it combines fast interpretation of scripts with complex compiling of source code.

Let's get going, all people who already know C can just skip this and read the next one here: https://github.com/CaptainBear079/System-079/blob/main/doc/InterpretingAndCompiling/ScriptVSSource.md

How about we start with a "Hello world!" program.

## The compile Main function
First we need a point to start our program from, the compiler always searches for a function named "main" so we have to define it.

    int main() {
        return 0;
    }

Let's go trough this:

"int" is the type of value the function will return (give back) when it reaches a "return" and ends. It can be any defined type, "int" means "signed integer number with a 4 byte size" you don't need to worry about that for now, just remember that main ALWAYS returns an integer.

"main" is the name of the function, there are some names used by the compiler and interpreter that have a special meaning and shouldn't get used by the programmers, "main" is just the standard name for the entry point of a program.

"()" these brackets contain all parameters the function needs, main usally takes "int argc, char* argv[]" as parameters, but for now we can just leave it empty since we don't give information into the program.

"{" and "}" these curly brackets contain all code that belongs to the function, everything inside these brackets is executed when the function is called.

"return 0;" this line ends the function and gives back the value "0" to the caller, in case of main this value is given back to the operating system that started the program. A return value of "0" usually means that everything worked fine.

## Printing text
Now let's print "Hello world!" to the console, we use the C-like "printf" function from the standard library "stdio.h" for that.

    #include <stdio.h>

    int main() {
        printf("Hello world!\n");
        return 0;
    }

The first line "#include <stdio.h>" tells the compiler to include the standard input/output library, which contains the definition of "printf".

Then inside the main function we call "printf("Hello world!\n");" which prints the text "Hello world!" followed by a new line to the console. The "\n" is a escape sequence that represents a new line character. It's used since we can't just press enter inside the string.

## Compiling and running
To compile and run the program, save it in a file named "main.source" and use "chaoslang compile -o main main.source" on linux or "chaoslang.exe compile -o main.exe main.source" on Windows to compile it. Then run the compiled program with "./main" on linux or "main.exe" on Windows.
You should see the output:

    Hello world!

## Next steps
Here we talk about interpreting and compiling use cases: https://github.com/CaptainBear079/System-079/blob/main/doc/InterpretingAndCompiling/ScriptVSSource.md

And my DEV Log: https://github.com/CaptainBear079/System-079/blob/main/doc/HCB_DEVLog/Entry0.md