# MIDI Pedal Board

I have an idea to build a MIDI pedal board for sending MIDI messages to a laptop while I'm playing guitar. It could send note messages (to trigger samples), or program changes (to change effects) or many combinations.

Videos like [this by Hacker Shack](https://www.youtube.com/watch?v=M25I58I7BtI) and [this by Notes and Volts](https://www.youtube.com/watch?v=H3WevrsmO9o) make it look easy - well, at least not crazy difficult. And parts are both easy to obtain and cheap.

Specialised parts, like [this sloped aluminuim enclosure](https://uk.farnell.com/bopla/atph-1865-300/case-aluminium-68x300x181mm/dp/1843600), could give the unit a real professional finish. And by adding [a TFT touchscreen](https://www.pjrc.com/store/display_ili9341_touch.html) I could build something really powerful - and completely bespoke to me.

Easy, right?

The physical build will be challenging - I'm not a great craftsman - but I'm pretty sure I can handle the programming part. And that's what this project is about: a way to test the logic I want to build in HTML, CSS, and JavaScript.

## Files

`13-buttons.html` was the initial idea for this unit, with 13 buttons. However, I printed out the design on paper at scale and the buttons were too close together to be usable by my big feet!

`drawing.svg` and `drawing.pdf` are the diagram I created to print out on paper (A4 paper is 297mm long, and the unit is 300mm long, which is close enough for jazz)

`7-buttons.html` was my next design, with 7 buttons. This is definitely more comfortable to play - and meant I could put the screen in landscape mode.

`index.html` is the current version, still with 7 buttons but using a JSON configuration file to define the modes and actions of each button (or set of buttons) for each mode.

`config-example.json` is the example config file, in JSON format. While a nerd like me could edit it by hand, if anyone else were to use this format a GUI to edit the file would be necessary. I imagine something like the Blackstar Live Logic app:

![Screenshot of the Blackstar Live Logic editor app](research/blackstar-live-logic-app.jpg)