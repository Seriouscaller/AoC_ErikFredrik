# Day 4

## part a

This is a real challenge for me, im not used to advanced string manipulation

### ideas

For the most part i will just have to get started and see what happens but some general ideas. I suspect main() will grow to be horribly big. And then good challenge to clean up
- use linelengthfinder to see how long the longest line is
- go over input file, line by line, fgets or what it is?
    - split it into sub sub-parts
        - text part
        - number part
        - checksum_input
    - make the checksum_calc ourself and verify
