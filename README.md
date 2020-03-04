# Notebook
A language for easily creating beautiful documents, mainly study notes.

![Preview 1](preview1.png)

![Preview 2](preview2.png)

![Preview 3](preview3.png)



### Install / Uninstall
- To install the `notebook` command, run `install.sh`
- To uninstall, run `uninstall.sh`


### Prepare directory for a new project
1. Navigate to the project directory.
2. Run `notebook -i` to initialize the directory and copy the dependencies. This way the directory is ready to host a new project.


### Parse a source file
1. Navigate to the project directory.
2. Write the source file (in this example, `source.nb`).
3. Run `notebook -p source.nb`. The document `source.html` is produced.


### Source file syntax

#### Headers
`# This is an <h0> tag` (larger than `<h1>`, used for main titles)

`## This is an <h1> tag`

...

`####### This is an <h6> tag`

#### Text styling
`*This will be italic*`

`**This will be bold**`

`***This will be bold & italic***`

`_This will be underlined_`

#### Line breaks
- Two lines separated by `\n` are rendered as one.

- Two lines separated by two `\n` are rendered one after another.

- Two lines separated by three `\n` are rendered one after another with a blank space in-between.


#### Lists

##### Unordered lists
```
- Item 1
- Item 2
[4 spaces]- Item 2a
[4 spaces]- Item 2b
[4 spaces][4 spaces]- Item 2b.1
- Item 3
```

is rendered as:

- Item 1
- Item 2
	- Item 2a
	- Item 2b
		- Item 2a.1
- Item 3

##### Ordered lists
```
1. Item 1
2. Item 2
[4 spaces]2.1. Item 2.1
[4 spaces]2.2. Item 2.2
[4 spaces][4 spaces]2.2.1. Item 2.2.1
3. Item 3
```

#### Margin notes
`\note{{This is a margin note. This tag should textually come before the element we want it to visually be rendered on the side of}}`

#### Definitions
`\def{{This is a definition}}`

#### Example
`\example{{This is an example}}`

#### Theorems
`\theorem{{This is a theorem}}`

#### Proofs (with QED symbol)
`\proof{{This is the proof for a theorem`
`\qed`
`}}`

#### Quotes
`\quote{{This is a quote}}`

#### In-line code
`\code{{int f(int x);}}`

#### Multi-line code
```c++
\codeblock{{int f(int x) {
    return x+1;
}
}}
```

#### In-line LaTeX
`$f(x) = x+1$`

#### Multi-line (centered) LaTeX
`$$f(x) = x+1$$`

#### Images
`\img{{src="images/image.png"}}`

`\img{{src="images/image.png" width=200}}`

`\img{{src="images/image.png" height=100}}`

`\img{{src="images/image.png" width=200 height=100}}`

#### Manually center elements
`\center{{This text will appear as centered}}`

#### HTML tags
We can also use any HTML tags, for instance, to make a table:

```html
<table>
    <tr>
        <td>Header 1</td>
        <td>Header 2</td>
        <td>Header 3</td>
    </tr>
    <tr>
        <td>Item 1</td>
        <td>Item 2</td>
        <td>Item 3</td>
    </tr>
</table>
```

#### Checkmark
Sometimes it is useful to quickly write a green checkmark, like ✓.
We can do this using the tag `\check`.

#### Cross
In a similar way, we can have the red cross ❌ character using `\cross`.

#### Blue text
It is often nice to highlight some text in blue quickly.
For this we wrap the text in `\blue{{text}}`.
