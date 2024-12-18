Some data compression tools I've implemented.

Right now, I have working run length encoding for arbitrary data, though it is not very effective at compressing images.

To use, run `make kompressor` then `./kompress <mode> filename` where mode is `-e` to compress a file and `-d` to decode it. 
