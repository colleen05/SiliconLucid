echo Building \'example1-osx-`uname -m`\' ...
clang -std=c99 -Wall example1.c minisdl_audio.c -lm -ldl -lpthread -framework CoreServices -framework CoreAudio -framework AudioUnit -o example1-osx-`uname -m`
echo Building \'example2-osx-`uname -m`\' ...
clang -std=c99 -Wall example2.c minisdl_audio.c -lm -ldl -lpthread -framework CoreServices -framework CoreAudio -framework AudioUnit -o example2-osx-`uname -m`
echo Building \'example3-osx-`uname -m`\' ...
clang -std=c99 -Wall example3.c minisdl_audio.c -lm -ldl -lpthread -framework CoreServices -framework CoreAudio -framework AudioUnit -o example3-osx-`uname -m`
echo Done!