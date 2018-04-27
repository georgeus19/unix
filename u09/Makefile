files=$(wildcard *.jpg)
results=$(files:.jpg=.thumb.jpg)
all:$(results)
%.thumb.jpg:%.jpg

	@convert $< -resize 100x100 $@
	@./ghtml
clean:
	@rm -f $(results) index.html
