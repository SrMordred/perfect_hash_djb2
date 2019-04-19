# Perfect Hash For DJB2

This project loads a file with string entries and generate **C** code for a perfect hash function using DJB2 algorithm.

DJB2 original algorithm:

```d
unsigned long hash(char *str) {
        unsigned long hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c;
        return hash % mod;
}
```

This program will change the *hash* and *<<5* (*shift*) values,
exploring the space between 0 and 10000 for the *hash* and 1 to 31 for the *shift* output **C** code with the best algorithm for the lower hash array size.

## Show And Tell

Entries file:
```d
there
will
be
some
nice
strings
```

`./main entries.txt` :

```d
/********************/
/* GENERATED C CODE */
/********************/


unsigned long dbj2(unsigned char *str)
{
    unsigned long hash = 3;
    int c;

    while ((c = *str++))
        hash = ((hash << 10) + hash) + c;

    return hash % 7;
}


const char* hash_table[] = {
        [1] = "some",
        [2] = "be",
        [3] = "will",
        [4] = "strings",
        [5] = "there",
        [6] = "nice",
};
```

## License

MIT License

Copyright (c) 2019 Patric Dexheimer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
