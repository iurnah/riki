Compiler Related Knowledge Base
===

##1. Weak symbol and strong symbol.

By defalut, without any annotation, all symbols are strong. weak symbol denotes a specially annotated symbol during linking of elf object files. strong symbol can overwrite a weak symbol. Keep in mind, tow strong symbols in the same piece of code results in compiling error. Weakly declared symbol don't need a definition when linking, strong symbol mush have definition when linking.

See the [weak symbol wiki](http://en.wikipedia.org/wiki/Weak_symbol) 

**use cases**

1. The default implementation is then defined as weak symbol, to provide default implementations of functions can be replaced by more specialized one at link-time. 
2. Library define symbol as weak, program that link the library is free to provide a strong one for customization purpose.
3. maintenance of binary backward compatabilities. 

**Limitations**

see the [weak symbol wiki](http://en.wikipedia.org/wiki/Weak_symbol) 