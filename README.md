## Goals
- Support multiple file formas (md,txt,pdf,docx,pptx..)
- Optionally include multiple stemmers
  - Maybe include [langdetect](https://github.com/scivey/langdetectpp) to decide which stemmer to use  
## Usage
Currently only supported filetypes are those that are puerly textual. Use `--all`
option to go through all files regardless of their extension (currently .txt, .md, .html)

`./search --index [directory] --db [name]` index a directory and save it

`./search --db [name]` search an already indexed directory

## Build
Initialize SQLitecpp

`git submodule init`

`git submodule update`

You must have [ICU](https://icu.unicode.org/home) installed on your system. ( Linux distros may come with it pre-installed)

Download [fmt](https://github.com/fmtlib/fmt), edit CmakeLists as needed.

