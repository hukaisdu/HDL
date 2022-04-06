# HDL -- Higher-Order Differential-Linear Attacks
Source Code for "Revisiting Higher-Order Differential-Linear Attacks from an Algebraic Perspective"

## Update: the verification codes for the bias on Page 22 from HATF : [Verify](https://github.com/hukaisdu/HDL/tree/main/Verify)

In this directory, we provide four cpp files for verifying the bias for 
(0, 7)  --> 25: verify_0_7_25.cpp
(0, 14) --> 51: verify_0_14_51.cpp
(0, 51) --> 18: verify_0_51_18.cpp
(0, 57) --> 18: verify_0_57_18.cpp

You can use the provided Makefile to compile them. 

## HD Distinguishers for Ascon : [Section 4](https://github.com/hukaisdu/HDL/tree/main/Section4)

In this directory, we provide codes for detecting the HD distinguishers for Ascon permutation, initialisation and encryption. 

[SageMath](https://www.sagemath.org) is used for implementing some detection algorithms about symbolic computations. So you need to install [SageMath](https://www.sagemath.org) first. 

Use the following command to run the detection algorithms:

* Detect the HD distinguishers for the Ascon permutation

	`sage -python permutation.py`

	The current setting is for 8 rounds, change the code in Line 92 for other rounds.

* Detect the HD distinguishers for the Ascon initialization

	`sage -python initialization.py`

* Detect the 2nd order DL distinguishers for the Ascon initialization

	`sage -python initialization_2nd.py`

* Detect the 2nd order DL distinguishers for the Ascon initialization

	`sage -python encrypt.py`

## Higher-Order Algebraic Transitional Form (HATF) : [Section 5](https://github.com/hukaisdu/HDL/tree/main/Section5)

[ascon_hatf.py](https://github.com/hukaisdu/HDL/blob/main/Section5/ascon.py) is the implementation of computing the HATF for Ascon. 

Use 

`sage -python_hatf ascon.py` 

to run it. You will get the bias for (0, 7) --> (25) and the corresponding conditions. 

[recoverykey.cpp](https://github.com/hukaisdu/HDL/blob/main/Section5/recoverkey.cpp) implementes the algorithm for the key recovery. With $2^{9.94}$ datas, we can judge the right keys with about 9,500 times of success among 10,000 experiments.



## Cube Testers : [Section 6](https://github.com/hukaisdu/HDL/tree/main/Section6)

[HDL.cpp](https://github.com/hukaisdu/HDL/blob/main/Section6/HDL.cpp) is used to verify the HDL approxiamtions provided in Table 4. Type `make` to compile it and run it to see the results. 






