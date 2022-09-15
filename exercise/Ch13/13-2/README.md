#### Question

Time the operation of the filebuf/write_bytes.c program (provided in the source
code distribuctions for this book) for various buffer sizes and file systems.

#### Answer

On Btrfs, write `10MB` bytes:

|buffer size| time|
|-----------|-----|
| 1 byte    |0.30s user 13.41s system 13.737 total|
| 10 bytes  |0.03s user 1.30s system 1.332 total|
| 100 bytes |0.00s user 0.15s system 0.151 total|
| 1000 bytes|0.00s user 0.02s system 0.023 total|
| 10000 bytes|0.00s user 0.00s system 0.005 total|
| 100000 bytes|0.00s user 0.00s system 0.002 total|
| 1000000 bytes|0.00s user 0.00s system 0.003 total|
