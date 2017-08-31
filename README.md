# Psion Pi

Personal electronics project; Attempting to build a modern PDA clamshell computer in a likeness of the venerable Psion 5mx.

![created by readme-kanban-board](http://i.imgur.com/hrjrRvy.png)
<!---KANBAN
# To Do
- Battery protected/stepped-up/hooked up
- Case build
- Keyboard interface
- Port extensions
- PCB design/etching as needed

# In Progress
- LCD screen hooked up

# Done
- Bootable Pi
- Battery build
KANBAN--->

## Aims

- Tasty colour screen - For enjoyable video playback.
- Quality keyboard - So it's legitimately useful.
- Portability - I'm under no illusions that it'll be larger than a Psion 5mx/Jornada (as would be ideal) because of screen size and battery demands but if it's smaller than a Netbook Pro I'll be happy.
- Sturdiness - Every single bit will be inherently fragile as heck, so I need to 3D print the structure real good.

## Ingredients

- Battery pack (18650 2S parallel 3.7v) - Home-made li-ion for the win!
- Charging circuity and steps ups with protection - So I don't blow myself up.
- Raspberry Pi Zero W (1.1)
- Psion 5mx keyboard - What? No way! Yes way. **Yes frickin way**.
- Keyboard USB interface - Arduino, hand-built.
- 7" Capacitive Touchscreen LCD 1024*600 (Waveshare C Rev2.1)
- Mad efficient 1337 OS and warez, legit IDE, etc - We'll start with vanilla Raspbian first though.

## "Wait, so what's this repo even for then?"

Notes about how I set everything up for reference, config files, microcontroller code for keyboard interface, etc etc.

### Setting up the Pi

I had more problems with the Zero W than any other Pi, even sent one back. Probably just batch teething problems but anyway..

Raspbian Stretch (2017-08-16) did **not** work, no matter what.

Raspbian Jessie (2017-07-05) did though, [available here](https://downloads.raspberrypi.org/raspbian/images/raspbian-2017-07-05/)

Installed on a 16gb Kingston microSD with win32diskImager (or similar) then, *before removing*, (in the windows partition, or /boot on unix):
1. Create a blank file called `ssh` on it.
2.
