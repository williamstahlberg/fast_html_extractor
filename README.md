Extracts HTML elements by tag name using pure C, by <code>start</code> tag and [code]end[/code] tag. It keeps inner nested tags by [code]middle[/code].

[b]Usage:[/b]
  ./main input_file start middle end output_file

[b]Example:[/b]
  ./main index.html '<div class="container">' '<div' '</div>' extracted.html
