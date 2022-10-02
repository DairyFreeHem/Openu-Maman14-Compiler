# maman14

Maman14 assembler project for Openu class 2022B

## Standard
    * Runs ISO C(89/90).
    * Built on Ubuntu 16.04.

## Filesystem guide
### 	source
		main
		preprocessor - Contains preprocessor code
		compiler - Containes code for first and second pass

####		tables
			labelTable
			LabelReferenceTable - Contains labels found in .entry or .extern
			codeTable
			dataTable

####		utilities
			input - Generic Code for managing external files
			formatter - Generic Code for Text formatting
			textverify - Verify what type of text is in the input
			Converters - Converters to Bin/Decimal/Base32
			defines.h - Generic defines for our code
			writer - functions that assist in creating a .ob file

## How to compile
    in the project folder, run make command

  
## NOTES
    Remove -g in Makefile before handing out project
