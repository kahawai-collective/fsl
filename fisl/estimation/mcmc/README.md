# Markov chain Monte Carlo methods

This modules implements MCMC algorithms.
Currently only the very simple Metropolis algorithm is implemented.
This algorithm requires tuning by setting the variance of the proposal distibution.

However, we a considering implementing adaptive MCMC algorithms which do not require tuning.

A good review of adaptive MCMC algorithms is available here:
	Roberts, G. O., & Rosenthal, J. S. (2009). Examples of adaptive MCMC. Journal of Computational and Graphical Statistics, 18(2), 349-367.

The SCAM algorithm of Haario et al (2005) seems like a good option.

* Haario, H., Saksman, E., & Tamminen, J. (2005). Componentwise adaptation for high dimensional MCMC. Computational Statistics, 20(2), 265-273.
* Haario, H., Laine, M., Mira, A., & Saksman, E. (2006). DRAM: efficient adaptive MCMC. Statistics and Computing, 16(4), 339-354.
* Christen, J. A., & Fox, C. (2010). A general purpose sampling algorithm for continuous distributions (the t-walk). Bayesian Analysis, 5(2), 263-281.
  Available in C++ and C at http://www.cimat.mx/~jac/twalk
