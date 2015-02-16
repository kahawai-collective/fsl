'''
A python script for creating/checking a module
has the correct files present. See Makefile for useage. 
Intended to be run from FSL root directory
'''
import sys
import os

module = sys.argv[1].replace('.module','')
parts = module.split('/')
name = parts[len(parts)-1]

variables = dict(
	name = name,
	title = name.title(),
	include_line = '#include <fsl/' + os.path.join('/'.join(parts),name + '.hpp') + '>',
	namespace_start = '\n'.join(['namespace %s {'%part.title() for part in parts]),
	namespace_end = '\n'.join(['} // namespace %s'%part.title() for part in parts]),
	namespace_using  = 'using namespace ' + '::'.join([part.title() for part in parts]) + ';'
)

for template in (
	'module/template.hpp',
	'module/template.cxx',
	'module/template.cila'
):
	extension = os.path.splitext(template)[1]
	directory = os.path.join('fsl',module)
	filename = os.path.join('fsl',module,name+extension)
	if not os.path.exists(filename):
		if not os.path.exists(directory): os.makedirs(directory)
		content = file(template).read().format(**variables)
		file(filename,'w').write(content)
