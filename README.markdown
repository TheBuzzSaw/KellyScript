# KellyScript

Because C++ is the hero we need but not the one we deserve.

Also, despite the working title, this is not a scripting language. It is a programming language that will compile down to machine code. I want a version of C++ without all the stupid stuff. I want many of the niceties introduced by C#/Java without crossing into garbage collection territory. I want fast build times and easy code/package sharing. I want the syntax to remain familiar, so yes, that means braces and semicolons. Get over it.

## Feature Ideas

* Proper array and slicing support. Tools (and people) can better reason about `int[]` than the traditional `int*, size_t` separation.
* Eliminate `#include` and `#define`. Add a proper package/namespace system a la Java/C#.
* Use dot operator for everything. No more `::` or `->` operators. (They can be moved to other purposes though.) I'd much rather see `Package.Class.Member.Data.Stuff` than `Package::Class::Member.Data->Stuff`.
* Case statements break by default. Jumps or fallthrough must be explicit.
* All variables initialize to default values. Add a keyword/operator to indicate no-initialization. This also applies to return statements.
* Support multiple return values.
* Lambda syntax closer to that of C#.
* Explicitly support int32, int64, etc. Switch the vague int, long, etc. to aliases.
* Support better metaprogramming. Stop invading the templating system with complexity.
* Support pattern matching and optionals.
* Drop exceptions.
* If possible, eliminate the pointer-reference dichotomy. There should be one way to work with addresses.
* Integrate proper span support. I'd rather specify `int[]` than `int*` and `size_t`.
* Support reflection. Empower tools to (for example) auto serialize structs.
* Instead of obscure macros, have a rich suite of compiler queries: `Compiler.CurrentLine`, `Compiler.CurrentFunction`, `Compiler.OperatingSystem`, etc.

### Unrefined Ideas

These are concepts that need further exploration. They are not priorities in these early stages of development.

* "Enum tables". I want to be able to more strongly associate other kinds of data with each enum value.
* "Open enums and closed enums". Or maybe "loose enums and strict enums". A closed/strict enum would more carefully enforce that only valid values are set. This may only be feasible in a higher level language with more type constraints.
* "Resources". Move-only polymorphic classes with constructors/destructors. Resources may contain structs, but structs may not contain resources. (Structs may contain resource pointers.)
* C# has `IEnumerable<T>` at the core of its `foreach` technology. Is that feasible in KellyScript? Or does it introduce native-unfriendly overhead? Does it hinder optimization opportunities?

### Safer Switch

C/C++

    switch
    {
        case 1:
            DoThings();
            break;
        
        case 2:
        case 3:
            DoOtherThings();
            break;
    }

KellyScript

    switch
    {
        case (1)
            DoThings();
        
        case (2, 3)
            DoOtherThings();
        
        case (4)
        {
            DoThings();
            DoMultipleThings();
            Fun();
        }
    }

## Challenges

* Can we eliminate the pointer-reference dichotomy? Can pointers just be made nicer to work with?

## C++ Keywords

Still deciding which of these to keep.

* asm
* auto
* bool
* break
* case
* catch
* char
* class
* const
* const_cast
* continue
* default
* delete
* do
* double
* dynamic_cast
* else
* enum
* explicit
* export
* extern
* false
* float
* for
* friend
* goto
* if
* inline
* int
* long
* mutable
* namespace
* new
* operator
* private
* protected
* public
* register
* reinterpret_cast
* return
* short
* signed
* sizeof
* static
* static_cast
* struct
* switch
* template
* this
* throw
* true
* try
* typedef
* typeid
* typename
* union
* unsigned
* using
* virtual
* void
* volatile
* wchar_t
* while
