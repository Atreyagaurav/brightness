# Brightness editing program

I had to made this becase there was problem with xbacklight in my Lenovo G40 laptop, on Arch i3-gaps setup.

Although there should be some options in xbacklight to make it work. It was faster to write this code than figure it out. 
I have it written in two seperate languages and there are variations among them.

## Build
For shell script you don't have to build it, you can just run it as follows to increase and decrese by 10 percentage.
```bash
brightness -inc 10
brightness -dec 10
```

You can use the `gcc` comamnd for c version and `go build` command for the go version. Once you make the executable named brightness, you can use it. If you want to use it from anywhere place it in your `/bin` folder. In case for i3 I have specified the keybindings for Brightness up and down key.
The corresponding lines of the i3 config is as follows.
### for c version 
```
bindsym XF86MonBrightnessUp exec --no-startup-id brightness -u 10
bindsym XF86MonBrightnessDown exec --no-startup-id brightness -d 10
```

### for golang version 
```
bindsym XF86MonBrightnessUp exec --no-startup-id brightness -up
bindsym XF86MonBrightnessDown exec --no-startup-id brightness -down
```

## Uses
It is used for increasing, decreasing and setting brightness percentage. 

The syntax is slightly different for two of them because of difference in argument parsing. (I didn't work too hard to make them the same :P)

### Syntax for Golang version
To increse the brightness (increases by 10%)
```
brightness -up 
```
To decrese the brightness (decreases by 10%)
```
brightness -down 
```
To set a brightness percentage (sets it to <val>, replace by your value (0-100))
```
brightness -percentage=<val>
```

### Syntax for c version
To increse the brightness (increases by <val>%)
```
brightness -u <val> 
```
To decrese the brightness (decreases by <val>%)
```
brightness -d <val>
```
To set a brightness percentage (sets it to <val>, replace by your value (0-100))
```
brightness -p <val>
```
The space between the option text and <val> is optional, the parser will understand it anyway. i.e. `brightness -u20` or `brightness -u 20` both will increase by 20%.

## Comparisions
### Golang version
I wrote it first because it was easier to pass commandline argument and parse it on golang. 

the feature of this version is that:

* It has directory specification, so you can specify that from commandline.
* It has less bugs in case of wrong aruments passing.

### C version
I originally wanted to write it in c, and I am using the c version right now, because of following benefits.

* It is lightweight. golang version is about 2.3 MB when compiled while c version is only 16.6 KB.
* the arguments are parsed in the order they are given. 

For example `brightness -percentage=30 -up` of go version Vs `brightness -p 30 -u 10` of c version. 
In go version since -up is present, -percentage will be ignored. In c version, first 30% will be set then it'll be increased by 10 again.
