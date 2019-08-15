# Work Setup

## Robot

- plug in and switch on power source
- plug 3-ended power cable into power source and robot
    - **2-ended side:** blue → src blue, red → src red
    - **single-ended side:** → robot
- plug USB3-VGA cable into PC and Robot
- turn robot in programming mode by pushing the switch downwards

## IDE

- local project located somewhere in ~/workspace/
- open IDE (Kate/VSC), terminal and file browser in project folder
- make sure robolib* commands work in the terminal (type robo[tab] and see if command suggestions appear)

## build & download

- build command: `$ robolib_all && robolib_download`
- build with cleared console: `$ clear && robolib_all && robolib_download`

## git

- clone remote repository to new local folder: `$ git clone`
- pull remote changes to local repository: `$ git pull`
- list of changed files: `$ git status`
- add new / update changed files/folders: `$ git stage <files>` or `$ git add <files>`
- view diff in console: `$ git diff --staged [<files>]`
- commit changes: `$ git commit -m "descriptive message"`
- push commits to remote repository: `$ git push`

## other useful shell commands

- find files: `$ find -iname PATTERN` or `$ find -regex REGEX`
- find files containing some text: `$ grep -rn text`
    - **-r**ecursive
    - **-n** show line numbers & content
    - **-E**xtended Regex
    - **--include PATTERN** specific file patterns, ie *.c for c source files

