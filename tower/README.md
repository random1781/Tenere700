# Tenere 700 Rally Tower

This is a free, easy-to-build (or source), modular, and customizable rally-style tower *design* that provides an adjustable platform for larger navigation devices and aftermarket lights by replacing or eliminating OEM components in the front end, including the headlight. 

![Tower V06 render](images/renders/v07/tower-v07-angles.jpg)
![Tower V06 installed](images/production/v06/v06_installed-cropped.jpg)

## Why?

The functional goals are to:
- Make space in the dash for additional components and aftermarket parts
- Provide a stable platform for large/heavy devices (i.e. tablets)
- Provide adjustability and customization
- Increase illumination with brighter light(s)
- Reduce overall weight
- Easy to repair

The unintentional benefits:
- Increased airflow (if you don't cover the front)
- Additional storage space - a spare tube fits perfectly in between the brackets
- Additional lateral strength

## How does it work?

It replaces the following OEM components:
- Headlight ([BW3-H4300-00-00](https://yamaha-motor.com/parts/diagram/10635215/242553596?partNumber=BW3H43000000))
- Headlight Stay ([BW3-F8321-00-00](https://yamaha-motor.com/parts/diagram/10635215/242410301?partNumber=BW3F83210000))
- Headlight Support Bracket ([BW3-2831V-10-00](https://yamaha-motor.com/parts/diagram/10635215/242410301?partNumber=BW32831V1000))
- Display Meter Mount ([BW3-F834L-00-00](https://yamaha-motor.com/parts/diagram/10635215/242368756?partNumber=BW3F834L0000))

...with:

##### Brackets

  - [Support Bracket](https://github.com/random1781/Tenere700/tree/main/tower/bracket-support) - two of these form the support bracket, replacing the [Headlight Stay](https://yamaha-motor.com/parts/diagram/10635215/242410301?partNumber=BW3F83210000)
  - [Tower Bracket](https://github.com/random1781/Tenere700/tree/main/tower/bracket-tower) - two of these form the tower itself
  - [Meter Bracket](https://github.com/random1781/Tenere700/tree/main/tower/bracket-meter) - this is the bracket that the [OEM Display Meter](https://yamaha-motor.com/parts/diagram/10635215/242368756?partNumber=BW3835002000) bolts to and replaces the [Display Meter Mount](https://yamaha-motor.com/parts/diagram/10635215/242368756?partNumber=BW3F834L0000)
  - [Aux Bracket](https://github.com/random1781/Tenere700/tree/main/tower/bracket-aux) - two of these attach to the sides, each offering a mount for 1x cigarette lighter-sized attachment and 2x switches
  
##### Lights

Choose your own! Mix and match! As long as the lights are >= 5" / 125mm and have side mounts, you can bolt them on with spacers. Not all *vertical spacing* has been accounted for, but these example combos should work...
  - [Baja Designs](https://www.bajadesigns.com/)
    - 2x Squadron Pro
    - 1x XL80 and 1x Squadron Pro
	- 1x Squadron Pro and 2x S1
	- 1x LP4
  - [Denali Electronics](https://denalielectronics.com/)
    - 2x D4
    - 1x D7 and 1x D4
  - [Clearwater](https://clearwaterlights.com/)
    - Anything up to and including the Super Sevina
  - [Amazon Special](https://www.amazon.com/Auxbeam-Strobe-Driving-Polaris-Wrangler/dp/B0BKRTSP36)

##### Hardware

The rest is just hardware, the majority of which can be found at a local hardware store - one with one of those aisles of meticulously organized drawers of fasteners (think Ace or True Value and not Home Depot or Lowe's). All hardware in the reference design is based off inventory and designs from [McMaster-Carr](https://www.mcmaster.com/).

...and satisfying these manufacturing goals:
- Affordable
- Easy to manufacture without the need for complex tools
- Easy to replicate and iterate on

## *Does* it work?

The original design had 18,001 miles before being revised. The only failure point was the threaded rods that bolt to the side panels: after ~5,000 miles they would fatigue and shear. This has been addressed in the latest version but has not been proven yet.

This is the original design immediately prior to upgrading it:

![v06 front](images/production/v06/v06_18k_front.jpg)
![v06 dash](images/production/v06/v06_18k_dash.jpg)

## Is this for me?

- Do you want any of the things listed above?
- Did you crack your headlight to discover that it's an expensive part sold only as an assembly and are having a difficult time finding a replacement part in stock somewhere? Did you then look at other rally towers available and think they were a bit expensive?
- Are you looking for a project or wanting to explore design and fabrication or manufacturing?

If yes, then you might be interested in this.

## How do I make the brackets?

1. Make them yourself.
2. If you can't, ask a friend for help.
3. If you don't have any of those, find a local fabricator.
4. If you can't find one or you don't want to leave your house, use a fabrication service. [SendCutSend](https://www.sendcutsend.com) (or other similar services) is an excellent, easy-to-use, and well-priced service that can create all these parts. This what I used to fabricate everything and I'll be posting documentation on that process.

## Reference design

This design and bill of materials here is specifically for 1x Baja Designs XL80 on the top mounting point and is a *baseline*. Since everything except oversized rubber washers can be ordered from [McMaster-Carr](https://www.mcmaster.com/), this is the *baseline* for fasteners/hardware. There is a substantial amount of flexibility regarding materials and fasteners used, so while you could order virtually everything from McMaster, you might be able to source parts from a bucket of bolts.

## Version history

My versioning probably needs work. Each part has its own versions and the assembly as a whole has its own.

Version 8 (not produced) changes:
- Reduced the size of the bottom of `bracket-tower` since Version 2 was impacting the forks.
- Added a rotational slot near the primary light mount to allow for the second mounting point for Baja Design's LP4.
- NOTE: This version has not been produced/manufactured, but the changes are minor tweaks.

Version 7 (produced) changes:
- Added an additional attachment point between the frame and the tower. This was done to minimize shaking and prevent rotation around the (previously) single-point of attachement. The inner bracket is much larger to facilitate this.
- Adjusted the placement of the hole that provided the attachment point for the struts to which the side panels bolted onto. This was also made into a slot to facilitate variance and error tolerance since the inner bracket's increased size adds significant cost.
- Increased the diameter of this hole to facilitate an M8 bolt (from M6).
- Changed the hole for the bottom light to a slot for additional adjustment.
- Brought the leading edge of the outer bracket inward. The side panels were slightly impacting this leading edge.
- Extended the outer bracket downwards and added an additional hole to accommodate another strut to support the side panels at another attachment point. This position has not be verified.
- Altered the auxiliary bracket to accommodate two switches in addition to a standard cigarette lighter.
- Comparison of v06 and v07 [Tower Brackets](../images/production/misc/bracket-tower_comparison.jpg) and [Support Brackets]z(../images/production/misc/bracket-support_comparison.jpg).

Version 6 (produced) changes:
- Initial version

## Discussion

Join this discussion:
- https://www.tenere700.net/topic/6574-rally-tower-concept/
- https://www.advrider.com/f/threads/tenere-700-rally-style-tower.1645135/

![The Two Towers](images/production/v07/two_towers.jpg)