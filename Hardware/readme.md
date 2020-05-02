# Hardware
* This section of the repo contains all the hardware design files necessary for replicating this project.
* This readme will serve as an instruction manual for building Key Crypt

![switch plate](case_switch.svg)
## Parts
* 5 Acrylic plates for laser cutting
* 62 key switches (plate mount)
* 62 [Amoeba switch pcbs](https://keeb.io/products/amoeba-single-switch-pcbs?variant=12960782024798)
* 62 1N4148 through hole diodes
* [Adafruit 2.42" Oled display](https://www.adafruit.com/product/2719)
* [Proton C micro controller](https://keeb.io/collections/frontpage/products/qmk-proton-c)
* Breadboard
* 2 feet of colored wire (colored sleeving)

## Guide
* As I have not finished the prototype build this section will stay empty while I work out the details
* I will be adding clear photos and tips here
* [Key Layout](http://www.keyboard-layout-editor.com/##@_name=Thesis%20Board%20(V.2)&author=Eric%20Boettner%3B&@_x:3&c=%2363696a&t=%23eee2d0&p=SA%20R1&a:5&f:4&fa@:6&:6%3B%3B&=%23%0A3&_x:7.75%3B&=*%0A8%3B&@_y:-0.75&x:2%3B&=%2F@%0A2&_x:1%3B&=$%0A4&_x:5.75%3B&=%2F&%0A7&_x:1%3B&=(%0A9%3B&@_y:-0.75&f:3&fa@:6&:6%3B%3B&=%2F_%0A-&_f:4&fa@:6&:6%3B%3B&=!%0A1&_x:3%3B&=%25%0A5&_x:3.75%3B&=%5E%0A6&_x:3%3B&=)%0A0&_f:3&fa@:6&:6%3B%3B&=+%0A%2F=%3B&@_y:-0.5&x:3&t=%23CCCCB7&p=SA&f:6%3B&=%2F&%238745%2F%3B%0AE&_x:7.75%3B&=%2F&%238734%2F%3B%0AI%3B&@_y:-0.75&x:2%3B&=%2F&%238744%2F%3B%0AW&_x:1%3B&=%2F&%238746%2F%3B%0AR&_x:5.75%3B&=%2F&%238704%2F%3B%0AU&_x:1%3B&=%2F&%238707%2F%3B%0AO%3B&@_y:-0.75&c=%230073a2&t=%23eee2d0&p=SA%20R3&a:7&f:3%3B&=Tab&_c=%2363696a&t=%23CCCCB7&p=SA&a:5&f:6%3B&=%2F&%238743%2F%3B%0AQ&_x:3%3B&=%2F&%238834%2F%3B%0AT&_x:3.75%3B&=%2F&%238835%2F%3B%0AY&_x:3%3B&=%2F&%238706%2F%3B%0AP&_c=%230073a2&t=%23eee2d0&p=SA%20R3&a:7&f:3%3B&=Back%20Space%3B&@_y:-0.5&x:3&c=%2363696a&t=%23CCCCB7&p=SA&a:5&f:6%3B&=%2F&%238866%2F%3B%0AD&_x:7.75%3B&=%2F&%238594%2F%3B%0AK%3B&@_y:-0.75&x:2%3B&=%2F&%238868%2F%3B%0AS&_x:1&n:true%3B&=%2F&%238867%2F%3B%0AF&_x:5.75&n:true%3B&=%2F&%238592%2F%3B%0AJ&_x:1%3B&=%2F&%238596%2F%3B%0AL%3B&@_y:-0.75&c=%230073a2&t=%23eee2d0&p=SA%20R3&a:7&f:3%3B&=FN&_c=%2363696a&t=%23CCCCB7&p=SA&a:5&f:6%3B&=%2F&%238869%2F%3B%0AA&_x:3%3B&=%2F&%238593%2F%3B%0AG&_x:3.75%3B&=%2F&%238595%2F%3B%0AH&_x:3%3B&=%2F:%0A%2F%3B&_c=%230073a2&t=%23eee2d0&p=SA%20R3&f:4%3B&=%0A%22%0A%0A%0A%0A%0A'%3B&@_y:-0.5&x:3&c=%2363696a&t=%23CCCCB7&p=SA&f:6%3B&=%2F&%238800%2F%3B%0AC&_x:7.75%3B&=%3C%0A,%3B&@_y:-0.75&x:2%3B&=%2F&%238968%2F%3B%0AX&_x:1%3B&=%2F&%238771%2F%3B%0AV&_x:5.75%3B&=%2F&%238805%2F%3B%0AM&_x:1%3B&=%3E%0A.%3B&@_y:-0.75&c=%230073a2&t=%23212224&p=SA%20R4&a:7&f:3%3B&=SHIFT&_c=%2363696a&t=%23CCCCB7&p=SA&a:5&f:6%3B&=%2F&%238970%2F%3B%0AZ&_x:3%3B&=%2F&%238801%2F%3B%0AB&_x:3.75%3B&=%2F&%238804%2F%3B%0AN&_x:3%3B&=%3F%0A%2F%2F&_c=%230073a2&t=%23212224&p=SA%20R4&a:7&f:3%3B&=Enter%3B&@_y:-0.5&x:3&t=%23000000&p=SA%20R2%3B&=Alt&_x:7.75&t=%23212224&p=SA%20R3%3B&=UP%3B&@_y:-0.75&x:2%3B&=Ctrl&_x:1&t=%23000000&p=SA%20R2%3B&=Meta&_x:5.75&t=%23212224&p=SA%20R3%3B&=DOWN&_x:1%3B&=RIGHT%3B&@_y:-0.75&t=%23000000%3B&=&_t=%23212224%3B&=FN&_x:3%3B&=Lower&_x:3.75%3B&=LEFT&_x:3%3B&=HYPER&_t=%23000000%3B&=)

![wiring](Wiring.png)
