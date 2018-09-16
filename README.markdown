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

### Unify pointer regions

Since `int[]` references a runtime number of `int` values, perhaps the language should stay consistent and interpret `int[6]` as still a reference but with its range baked in at compile time. This means there needs to be new syntax to represent an array on the stack.

    int32[] values = ObtainValues();
    int32[3] header = values[0 to 3];

    inline int32[7] stackArray; // Does this work?
    int32 stackArray[7]; // Revert to C style for this case?
    Array<int32, 3> GetVector() // Is there some new native type?

This approach results in reverse accesses. (I believe this is why other languages opted for the `[a][b]values` syntax.)

    int32[][5] values = ...; // 5 arrays of N integers.
    values[1]; // Second array of N integers.

    int32[5][] values = ...; // N arrays of 5 integers.
    values[1]; // Second array of 5 integers.

One problem with C pointers is that there is no indication of whether the pointer targets one value or multiple values. Maybe this is an opportunity to fold pointers that reference only one value into the array notation.

    int32 value = 9;

    int32[1] myPointer = &value;
    int32 value2 = myPointer[0];

    int32& myPointer = &value; // This could be shorthand for int32[1].
    int32 value2 = *myPointer; // Shorthand for myPointer[0] ?

The only time we need raw pointers to unknown quantities is for C interop.

    // What else would I use the 0-length array for?
    // This could indicate that this pointer has no bounds checking.
    int32[0] myPointer = ThirdPartyPointer();

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
