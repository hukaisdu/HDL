# HDL -- Higher-Order Differential-Linear Attacks
Source Code for "Revisiting Higher-Order Differential-Linear Attacks from an Algebraic Perspective"

## HD Distinguishers for Ascon : Section 4

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

## Higher-Order Algebraic Transitional Form (HATF) : Section 5

[ascon_hatf.py](https://) is for 

## Cube Testers






