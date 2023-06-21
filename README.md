# Source code for submission "Revisiting Higher-Order Differential-Linear Attacks from an Algebraic Perspective"

The source code includes two parts: 1. HATF for probabilistic HDL 2. DSF for deterministic HD

# HATF 0.1

Tools to compute the biases of HDL approximations for Ascon, Xoodyak and Xoodoo.

## Dependencies

-- boost: We use the split function of boost, if you do not want to install boost, you can implement split function in split.cpp. Note that your own split function MUST be thread-safe. 

## Usage

We provide two kinds of tools. The first is a general one in main.cpp that allows you specify the cipher, round, order and differences. To use this tool:

```
$ make 
$ ./main --help
$ Usage: ./main --cipher [ascon|xoodyak|xoodoo] --round R --order l --difference d0,d1,...,d_{l-1} --thread t

optional arguments:
  --help           show this help message and exit
  --cipher         specify the target cipher
  --round          specify he target round
  --order          specify the order 
  --difference     specify the input difference as a tuple of integers such as "0,1,2", no space is allowed 
  --thread         specify the thread number according to your computer, this number must be an integer factor of 64

an example to use:

$ ./main --cipher ascon --round 4 --order 2 --difference 0,60 --thread 8

this is also the default setting
```

The second tool is to reproduce the results in our submission. The file is main_application.cpp.

```
$ make
$ ./main_application --help
$ Usage: ./main --cipher [ascon|xoodyak] --application [1|2|3|4|5|6] --thread t

optional arguments:
  --help           show this help message and exit
  --cipher         specify the target cipher
  --application    specify the application according to the paper
  --thread         specify the thread number according to your computer, this number must be an integer factor of 64

an example to use:

$ ./main_application --cipher ascon --application 1 --thread 8
```

## For Users' Own HDL Cryptanalysis 

Users need to implement a child class of HATF class defined in HATF.h

# DSF 0.1

DSF is a tool for detecting the HD distinguishers for Ascon permutation, initialisation and encryption. 

## Dependencies

--SageMath

[SageMath](https://www.sagemath.org) is used for implementing some detection algorithms about symbolic computations. So you need to install [SageMath](https://www.sagemath.org) first. 
## Usage

Use the following command to run the detection algorithms:

* Detect the HD distinguishers for the Ascon permutation

```
$ sage -python permutation.py
```

The current setting is for 8 rounds, change the code in Line 92 for other rounds.

* Detect the HD distinguishers for the Ascon initialization

```
$ sage -python initialization.py
```


* Detect the 2nd order DL distinguishers for the Ascon initialization

```
$ sage -python initialization_2nd.py
```

* Detect the 2nd order DL distinguishers for the Ascon encryption

```
$ sage -python encrypt.py
```






