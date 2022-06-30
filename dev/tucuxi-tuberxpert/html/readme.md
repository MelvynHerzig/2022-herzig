<h1> TuberXpert JavaScript converter </h1>

The PDF converter (wkhtmltopdf) requires JavaScript code in EcmaScript 5 version to work. This node project uses Babel to perform this conversion.

<h3> src </h3>

Contains the index used as base for the template, the css files and the original unconverted js files (EcmaScript6).

<h3> build </h3>

Contains the js files converted to EcmaScript 5. These files are the ones used by the HTML exporter and the index page.

<h2> Usage </h2>

Install the dependency

```
$ npm install -D babel-cli
``` 

Install the needed preset

```
$ npm install -D babel-preset-env
``` 

Build and transpile

```
$ npm run build
``` 
