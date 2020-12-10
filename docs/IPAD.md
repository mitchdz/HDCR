# IPAD

this is the process used to generate the circuit on an IPad:

Using Ubuntu 20.04 as the machine do to the operations on the image.

## 1) Draw circuit in Notability
## 2) Export to pdf
## 3) convert pdf to png
```bash
$ pdftoppm Circuit2.pdf Circuit2 -png
```
## 4) convert RGB png to grayscale png
```bash
$ convert Circuit2-1.png -colorspace Gray Circuit2-1.png
```