# KellyScript

Because C++ is the hero we need but not the one we deserve.

Also, despite the working title, this is not a scripting language. It is a programming language that will compile down to machine code. I want a version of C++ without all the stupid stuff. I want many of the niceties introduced by C#/Java without crossing into garbage collection territory. I want fast build times and easy code/package sharing. I want the syntax to remain familiar, so yes, that means braces and semicolons. Get over it.

## Feature Ideas

* Replace many macros with nicer system libraries. I wanna call something like `Compiler.CurrentLine` rather than `__LINE__`.
* Use dot for everything. I'd much rather see `Package.Class.Member.Data.Stuff` than `Package::Class::Member.Data->Stuff`.
* Proper array and slicing support. Tools (and people) can better reason about `int[]` than the traditional `int*, size_t` separation.

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
