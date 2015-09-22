require(ggplot2)
require(reshape)

#' A set of samples
#' 
#' @export Samples
#' @exportClass Samples
Samples <- setRefClass(
  "Samples",
  
  fields = list(
    samples = 'data.frame'
  ),
  
  methods = list(
    
    constrain = function(constraints){
      for(name in names(constraints)){
        constraint = constraints[[name]]
        min = constraint[1]
        max = constraint[2]
        samples[samples[,name]<min,name] <<- min
        samples[samples[,name]>max,name] <<- max
      }
    },
    
    plot_hists = function(){
      ggplot(melt(samples),aes(x=value)) + 
        geom_bar() + 
        facet_wrap(~variable,scales='free_x')
    },
    
    read = function(filename){
      samples <<- read.table(filename,header=T)
      if(!('likelihood' %in% names(samples))) samples$likelihood <<- NA
    },
    
    write = function(filename){
      if(!('likelihood' %in% names(samples))) samples$likelihood <<- 'nan'
      write.table(samples,file=filename,row.names=F,col.names=T,quote=F)
    }
    
  )
)

