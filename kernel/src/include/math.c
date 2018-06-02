
long fact(long x)
{
    long result;
    for (int i = 1; i < x; i++)
    {
        result = x * (x-i);
    }
    return result;
}

long sin(long x)
{
    return 0;
}

long pow(long b, long e)
{
    int r = 1;

    if (!(e % 2)) e /= 2;

    while (e > 0)
    {
        if (e & 1)
            r *= b;

        b *= b;
        e >>= 1;
    }

    return r;
}

long abs(long n)
{
    return n < 0 ? n * -1 : n * 1;
}

long floor(double n)
{
    // Handle floor(0)
    if (n == 0) return 0;

    if (n > 0)
        return (long)n;
    else
        return (long)n - ((n != (long)n) ? 1 : 0);
}

long ceil(double n)
{
    if (n == 0) return 0;

    if (n < 0)
        return (long)n;
    else
        return (long)n + ((n != (long)n) ? 1 : 0);
}