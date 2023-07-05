## Goals
- Support multiple file formas (md,txt,pdf,docx,pptx..)
- Graphical interface
- Optionally include multiple stemmers
  - Maybe include [langdetect](https://github.com/scivey/langdetectpp) to decide which stemmer to use  
## Build
Initialize SQLitecpp

`git submodule init`

`git submodule update`

You must have [ICU](https://icu.unicode.org/home) installed on your system. ( Linux distros may come with it pre-installed)

Download [fmt](https://github.com/fmtlib/fmt), edit CmakeLists as needed.
