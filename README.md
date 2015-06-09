# Linear-regression
Development of Obliv-C and C linear regression program for developing secure multiparty computations. Fixed point arithmetic is used in order to support Obliv-C.

To use, compile with `make linReg` and run with `./linReg <filename>`. 
Run the [test data](test.data) with `./linReg test.data`.

To contact me with any questions about this repository, open an issue on GitHub or email me at havron@virginia.edu.

# Warnings
Revision of [code](linReg.c) with fixed point arithmetic. Code does not account for overflow, and values are incorrect. Use previous commit to compute linear regression.