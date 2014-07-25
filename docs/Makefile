all: doxygen

.PHONY: doxygen

# Generate Doxygen documentation
doxygen:
	make -C doxygen
	
# Publish documentation
# Aggregates alternative documentation into a single place for publishing using Github pages 
# Requires the a branch called "gh-pages":
#	git branch gh-pages
# and the "ghp-import" script
publish: doxygen
	mkdir -p published
	cp -f index.html published
	cp -fr doxygen published
	ghp-import -m "Updated documentation" -p published

	