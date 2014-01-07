let g:syntastic_cpp_remove_include_errors = 1
let g:syntastic_cpp_compiler = '/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc'
let g:syntastic_cpp_compiler_options =
  \ ' -DBLESS_DEBUG' .
  \ ' -mmcu=atmega328p' .
  \ ' -DF_CPU=16000000L' .
  \ ' -DARDUINO=105' .
  \ ' -Ilib/AltSoftSerial' .
  \ ' -Ilib/AltSoftSerial/config' .
  \ ' -I/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/avr/include' .
  \ ' -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/standard' .
  \ ' -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino' .
  \ ' -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/avr-libc'
