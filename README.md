# Linear-regression
Development of Obliv-C and C linear regression program for developing secure multiparty computations.

To use, compile with `make linReg` and run with `./linReg <filename>`. 
Run the [test data](test.data) with `./linReg test.data`.

To contact me with any questions about this repository, open an issue on GitHub or email me at havron@virginia.edu.

# Usage Notes
Fixed point arithmetic is used in order to support Obliv-C. Note the small loss of accuracy due to fixed point arithmetic for a large input of data.
