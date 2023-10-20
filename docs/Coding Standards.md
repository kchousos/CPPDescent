# Coding Standards

## Formatting

- Indentation Style: 1TBS (OTBS)

    The function's opening brace is on the same line as the function name, 
    preceded by one space. The closing brace is on its own line aligned 
    with the function name.
    
    Indentation uses 4 spaces.
    
    Example:
    ```c
        int function(int arg1, int arg2) {
            // ...
        }
    ```

    At the project's root exists a file named `.clang-format` which contains
    the coding standard. It will be used by your IDE to automatically format
    the code.
    
## Comments

- Single line comments: `//` (C++ style)
- Multi-line comments: `/* ... **/` with a `*` at the start of each line (C style)