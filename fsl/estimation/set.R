require(stringr)
require(ggplot2)

Dimension <- setRefClass(
  "Dimension",
  fields = list(
    label = "character",
    type = "character",
    levels = "character"
  ),
  methods = list(
    bind = function(name){
      list(name=name,dimension=.self)
    },
    convert = function(values){
      'Convert values'
      if(type=='integer'){
        return(as.integer(values))
      }
      else if(type=='factor'){
        return(factor(values,levels=levels,ordered=T))
      }
      values
    }
  )
)

Stream <- setRefClass(
  "Stream",
  fields = list(
    data = "data.frame"
  ),
  methods = list(
    
    read = function(filename){
      # Read in the stream
      data <<- read.table(filename,header=T,stringsAsFactors=F)
      # Extract out array indices
      parts <- str_match(data$name,'(\\w+)\\(?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?,?(\\d+)?\\)?')
      # Drop the first column (the enire match) and convert to data frame
      parts <- as.data.frame(parts[,2:ncol(parts)],stringsAsFactors=F)
      # Give names and bind
      names(parts) <- c('array',paste0('dim',1:10))
      data <<- cbind(data,parts)
      
      invisible(.self)
    },
    
    plot_array = function(which,label,x,wrap){
      temp <- subset(data,array==which)
      temp$x_ <- x$dimension$convert(temp[,x$name])
      if(!missing(wrap)) temp$wrap_ <- wrap$dimension$convert(temp[,wrap$name])
      plot <- ggplot(temp) + 
        geom_line(aes(x=x_,y=value,group=sample),alpha=0.1) + 
        labs(x=x$dimension$label,y=label)
      if(!missing(wrap)) plot <- plot + facet_wrap(~wrap_)
      plot
    }
  )
)

DataStream <- setRefClass(
  "DataStream",
  contains = "Stream"
)
