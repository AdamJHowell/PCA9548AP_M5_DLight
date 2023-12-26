This project reads light levels from four I2C sensors.

Specifically, it is an implementation of the [M5Stack Pa.HUB](https://docs.m5stack.com/en/unit/pahub2) (PCA9548AP 6-port I2C multiplexer at address 0x70), 
with four [M5Stack DLIGHT I2C modules](https://docs.m5stack.com/en/unit/dlight) (BH1750FVI at address 0x23 with a range from 0 (dark) to 65535 (bright)).
This project is the basis for a Lego sun tracker.
The tracker will use the [M5Stack ATOM Motion base](https://docs.m5stack.com/en/atom/atom_motion) along with two [Kittenbot Geekservo continuous rotation servos](https://www.aliexpress.us/item/3256802685917966.html).

I've connected the four DLIGHT sensors to ports 0, 1, 4, and 5.
This was done to make routing the wires simpler in meatspace.
This port mapping is found in the Arduino setup() function.
If you use all ports (or use sequential port numbers) you can put those four lines in a for() loop to condense the code.

This has been tested on the [M5Stack Atom Lite](https://docs.m5stack.com/en/core/atom_lite) and works perfectly.
