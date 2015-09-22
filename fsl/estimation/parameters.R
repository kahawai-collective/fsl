require(stringr)

include('fsl/estimation/samples.R')

#' A set of parameters
#' 
#' @export ParameterSet
#' @exportClass ParameterSet
ParameterSet <- setRefClass(
  "ParameterSet",
  
  methods = list(
    
    sample_mvnorm = function(n,mean,sigma){
      'Generate random samples from a multivariate-normal distribution used the
      supplied mean and covariance estimates. This can be useful for generating samples
      based on results of an estimator external to FSL (e.g. CASAL)
      This is a simple wrapper around the `rmvnrom` function in the `mvtnorm`
      package which generates samples'
      require(mvtnorm)
      samples <- as.data.frame(rmvnorm(n,as.numeric(mean),as.matrix(sigma)))
      names(samples) <- names(mean)
      samples
    }
    
  )
)
