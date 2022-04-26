# big_grep - grep's hungry cousin

big_grep was written to complement GNU grep for a very large number of matching patterns.
Essentially it does this:

```bash
grep "pattern1|pattern2|pattern3" reference_file
```

However, if you have a lot of patterns you want to match with GNU grep you will at some point hit the limits of argument length.
Big grep uses a pattern file where each line is a pattern, and matches those 

## Installation
Note that compilation requires libboost-regex headers, because I wanted to be able to user Perl-style regexes and usually have boost available in all my environments (`libboost-regex-dev` on Ubuntu). 

```bash
# Clone
git clone https://github.com/bleutner/big_grep.git 
cd big_grep

# Compile
make

## Run
bin/big_grep -h
```

## Usage

```
USAGE: big_grep [args] -p <patternfile> -i <inputfile> 

DESCRIPTION:
  Matches every pattern of <patternfile> against every line in <inputfile>.
  This is useful for large numbers of patterns, which exceed the normal 'GNU grep' 
  input limits. Inclusive (default) or exclusive matching modes are possible.

ARGUMENTS:
  -i [filename]: target input file
  -p [filename]: file with patterns to match
  -o [filename]: output file (optional)
  -v: <flag> inverse mode, i.e. exclude matches (optional)
  -r: <string> perl-style regex which is used to extract a substring from <inputfile>, 
      which is then searched for in <patternfile> (optional)
  -t: <flag> test regex on first line of <inputfile> and exit

  -h: <flag> print help and exit
  -V: <flag> print program version
```