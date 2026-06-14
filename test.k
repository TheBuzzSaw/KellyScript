//export 792;
export Company.Validator;
export Kabe.Collections.Arrays;
export Kabe
  .  Stuff;

export Kabe.Yup.Even.Moar;
export Kabe.Even.Yup;

import Kabe.Collections;
import Kabe.Nope;
import Kabe.Even.Blargh;
import Garbage.Nope;

Process();

int32 GlobalGarbage;

int32 Average(int32 a, int32 b)
{
  return (a + b) / 2;
}

(int32 low, int32 high) LowToHigh (int32 a, int32 b)
{
  import System.Memory;
  HeapLoader();
}

LowToHigh (int32 a, int32 b) to (int32 low, int32 high)
{
  return b < a ? b, a : a, b;
  
  low = min(a, b);
  high = max(a, b);
  
  low = 7;
  
  // 500 lines later
  
  high = 12;
  fun = -3.1415926535898;
}

func Average : (int32 a, int32 b) -> (int32 result)
{
  result = (a + b) / 2;
}

Save : (int32 a, int32 b)
{
  /** This method is
  super crazy. ****/
  WriteToFile(a);
  /* Even /* crazier */ */
  WriteToFile(b);
}

int32 Average(int32[] values)
{
  return sum(values) / values.count;
}

int32 Average(int32* values, int32 count)
{
  return ...;
}

class Vector[type T]
{
  Allocator allocator;
  T[] array;
  size count;

  

  public void Add(T item)
  {
    if (this.count == this.capacity)
    {
      Reallocate(ref head, capacity * 2);

    }
  }
}

open struct Tuple[type T0, type T1]
{
  T0 first;
  T1 second;
}

closed struct CodePoint
{
  public property int32 Value;
}

struct CString
{
  Pointer[uint8] pointer;

  public constructor(Pointer[uint8] pointer) => this.pointer = pointer;

  public Span[uint8] AsSpan()
  {
    if (!this.pointer)
    {
      return default;
    }

    size count = 0;
    while (this.pointer[count])
    {
      count += 1;
    }

    return new(this.pointer, count);
  }
}

public const int32 DaysPerYear = 365;
public const int32 DaysPerFourCenturies = DaysPerYear * 400 + 97;
public const int32 DaysPerCentury = DaysPerYear * 100 + 24;
public const int32 DaysPerFourYears = DaysPerYear * 4 + 1;

opaque struct LocalDateTime
{
  int64 ticks;
}

opaque struct Date
{
  int32 days; // Days since 1 Jan 0001.

  property int32 Year
  {
    int32 result = 1;

    return result;
  }
}

opaque struct SpanWriter[type T]
{
  property Span[T] Span;
  property size Position;

  property Span[T] Written => Span[0 to this.Position];
  property Span[T] Available => Span[this.Position to end];

  public bool TrySetPosition(size position)
  {
    if (position in 0 to this.Span.Count)
    {
      this.Position = position;
      return true;
    }
    else
    {
      return false;
    }
  }

  public bool TryWrite(ReadOnlySpan[T] source)
  {
    if (source.TryCopyTo(this.Available))
    {
      this.Position += source.Count;
      return true;
    }
    else
    {
      return false;
    }
  }
}

bundle SpanWriter
{
  public SpanWriter[T] Create[type T](Span[T] span)
  {
    return new
    {
      Span = span
    };
  }
}

bool TryBlit[type T](ref SpanWriter[uint8] this, ReadOnlySpan[T] source)
{
  var bytes = source.AsBytes();
  if (bytes.TryCopyTo(this.Available))
  {
    this.Position += bytes.Count;
    return true;
  }
  else
  {
    return false;
  }
}

// int main(int32 argc, int8** argv)
// int main(Span[CString] args)
// int32 Main(ReadOnlySpan[ReadOnlyUtf8Span] args)

int32 Main(ReadOnlySpan[ReadOnlySpan[uint8]] args)
{
  Pointer[float64] ptr;
  Map[string, string] data;
  Array[int32, 6] array;
  Span[int32] span = array;
  Vector[int32] v;
  v.Add(55);
  Write("The \"average\" is ", Average(789, 945215784225), "...\n");
  Write('a', '\n');
  return 0;
  
  Failure("asdf\x5eYes", 'a', 'g', '\v', 'go', "qwer\xfffff\x");
  []

  for (int32 i in 0 to 10)
  {
    Process.StandardOut.Write("Avast ye");
  }
} {}

