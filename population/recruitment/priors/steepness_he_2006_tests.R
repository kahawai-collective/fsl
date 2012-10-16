library(ggplot2)
theme_set(theme_bw())
theme_update(
  panel.border = theme_rect(fill = NA, colour = 'black'),
  panel.grid.major = theme_line(colour='white',size=0.1),
  panel.grid.minor = theme_line(colour='white',size=0.1),
  strip.background = theme_rect(fill = 'grey80', colour = 'black'),
  legend.key = theme_rect(colour = 'white'),
  legend.title = theme_text(size=11,hjust = 0),
  axis.title.x = theme_text(size=11),
  axis.title.y = theme_text(size=11,angle=90)
)
  
densities = read.table('steepness_he_2006_tests_relative.tsv',header=T)
densities = within(densities,{
  combo = factor(paste(mortality,sigma,sep='|'))
})

dev.new(width=7,height=5)
ggplot(subset(densities,combo %in% c('0.1|0.3','0.3|0.6','0.5|1.2'))) + 
  geom_line(aes(x=steepness,y=density,colour=combo),size=1) +
  labs(x='Steepness',y='Probability density',colour='Natural mortality|Recruitment variation') + 
  opts(legend.position=c(0.3,0.8))
dev.copy2pdf(file="steepness_he_2006_tests_relative.pdf")
