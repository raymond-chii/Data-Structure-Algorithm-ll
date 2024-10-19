# Spell Check Project

## Overview
This project implements a spell-checking program using a custom hash table implementation. It's designed to load a dictionary of words and then check the spelling of words in a given document, reporting any unknown or excessively long words.

## Features
- Custom hash table implementation
- Dictionary loading with support for large dictionaries (up to 1,000,000 words)
- Case-insensitive spell checking
- Reporting of unknown words and their line numbers
- Reporting of excessively long words (>20 characters)
- Performance timing for dictionary loading and document checking

## Files
- `hash.h`: Header file for the hash table class
- `hash.cpp`: Implementation of the hash table class
- `spellCheck.cpp`: Main program for spell checking
- `Makefile`: For compiling the project
- `benchmark.cpp`: I am still working on benchmarking and writing test files for this project

## Compilation
To compile the project, use the provided Makefile:

```bash
make
```

This will create an executable named `spell.exe`.

## Usage
Run the compiled program:

```bash
./spell.exe
```

The program will prompt you for:
1. The name of the dictionary file
2. The name of the input file to spell check
3. The name of the output file for results

## Input Format
- Dictionary file: One word per line, no leading or trailing spaces
- Document file: Any text file with Unix-style line endings

## Output
The program will display the time taken to load the dictionary and check the document. The spell check results will be written to the specified output file, listing:
- Unknown words and their line numbers
- Words longer than 20 characters (showing the first 20 characters)

## Implementation Details
- The hash table uses linear probing for collision resolution
- Words are considered valid if they contain only letters, digits, dashes, and apostrophes
- The program is case-insensitive, converting all input to lowercase
- Words containing digits are not spell-checked

## Performance
The hash table is designed to handle dictionaries of up to 1,000,000 words efficiently. It includes a rehash function to maintain performance as the table fills up.

## Development
This project was developed as part of the Data Structures and Algorithms II course (ECE 365) for Fall 2024.
