# TrivilBiMap
[BiMap implementation from userver-triks](https://youtu.be/FcQC19CX-AY?t=1655)

## usage:
```c++
TrivilBiMap enumka = [](auto selector){
  return selector
    .Case("e1", 1)
    .Case("e2", 2)
    .Case("e3", 3)
    .Case("e4", 4)
    .Case("e5", 5)
    ...
    .Case("en", n);
};

...

/*constexpr*/ auto e1 = enumka.TryFindByFirst("e1") // int e1 = 1
/*constexpr*/ auto s2 = enumka.TryFindBySecond(2) // string_view s2 = "e2"
/*constexpr*/ auto e3 = enumla["e3"] // e3 == 3
/*constexpr*/ auto e4 = enumla[4] // e4 == "e4"
```
