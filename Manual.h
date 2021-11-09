#ifndef MANUAL_H
#define MANUAL_H

namespace Manual {
	/// Yeah, just a big string containing lots of HTML (and some CSS too to make the page pretty. Kind of)
	String manualHtml = "<!DOCTYPE html> \
<html lang=\"en\" dir=\"ltr\"> \
	<head> \
		<meta charset=\"utf-8\"> \
		<title></title> \
 \
		<style> \
body { \
	background-color: #222831; \
	color: #eeeeee; \
} \
main { \
	margin: 5em 10em; \
	padding: 2em 2em; \
	border-radius: 2em; \
	background-color: #393e46; \
} \
h1 { \
	font-family: sans-serif; \
	font-size: 300%; \
	text-align: center; \
	margin-top: 0px; \
} \
h2 { \
	font-family: sans-serif; \
	font-size: 175%; \
	border-bottom: 2px solid #515a66; \
} \
h3 { \
	font-family: sans-serif; \
} \
p { \
	line-height: 1.5em; \
} \
table, th, td { \
	border: 1px solid #eeeeee; \
	border-collapse: collapse; \
} \
th, td { \
	padding: 0.1em 0.3em; \
} \
th { \
	font-family: sans-serif; \
} \
		</style> \
	</head> \
	<body> \
		<main> \
			<h1>Asteroid Attack</h1> \
			<section> \
				<h2>The Game</h2> \
				<p> \
					You play as a ship, trying to avoid colliding with asteroids (and the occasional laser) in an asteroid swarm.<br> \
					The objective of the game is to survive for as long as you can to get a high score (Note: When the score reaches 65535, it will reset back to 0, as the score variable can't hold any more) \
				</p> \
			</section> \
			<section> \
				<h2>How To Play</h2> \
				<p> \
					The controls are very simple - There is simply a knob (the potentiometer) to turn that will control the horizontal position of the ship. That's all.<br> \
					Apart from a simple button to press to start the game from the titlescreen, that is.<br> \
					You can rotate the potentiometer between two positions - One of those positions represents the leftmost side of the screen for the ship, the other represents the rightmost side. Turn the potentiometer to move the ship to the position on the screen represented by the position of the potentiometer.<br> \
					<br> \
					Your ship has 3 lives. If you lose all 3 of those lives, you're dead, and are returned to the titlescreen, and your highscore is updated if necessary.<br> \
					Lives are represented in-game by the number of little copies of your ship in the top-right corner.<br> \
					Your score is shown in the top-left corner. \
				</p> \
			</section> \
			<section> \
				<h2>Setting Up The Wemos</h2> \
				<p> \
					You will need: \
					<ul> \
						<li>1 Wemos (Model ESP-8266)</li> \
						<li>1 128x64 Adafruit OLED Display (Model SSD1306)</li> \
						<li>1 3-pin Potentiometer</li> \
						<li>1 LED</li> \
						<li>1 Resistor suitable for the LED</li> \
						<li>1 Button</li> \
						<li>1 DS3231 Real Time Clock</li> \
						<li>You'll also need wires and probably a breadboard depending on how you want to set it up</li> \
					</ul> \
					<h3>Connections</h3> \
					<p> \
						You have to connect both the OLED and RTC to D1 and D2 - These are different sets of those pins. The Wemos has 2 sets. I connected the RTC to the ones closest to the micro-usb port \
					</p> \
					<p>OLED Display:</p> \
					<table> \
						<tr> \
							<th>OLED Pin</th> \
							<th>Wemos Pin</th> \
						</tr> \
						<tr> \
							<td>GND</td> \
							<td>GND</td> \
						</tr> \
						<tr> \
							<td>VCC</td> \
							<td>3.3v</td> \
						</tr> \
						<tr> \
							<td>SCL</td> \
							<td>D1</td> \
						</tr> \
						<tr> \
							<td>SDA</td> \
							<td>D2</td> \
						</tr> \
					</table> \
					<p>Potentiometer:</p> \
					<table> \
						<tr> \
							<th>Potentiometer Pin</th> \
							<th>Wemos Pin</th> \
						</tr> \
						<tr> \
							<td>Either side pin</td> \
							<td>GND</td> \
						</tr> \
						<tr> \
							<td>Other side pin</td> \
							<td>3.3v</td> \
						</tr> \
						<tr> \
							<td>Middle pin</td> \
							<td>A0</td> \
						</tr> \
					</table> \
					<p>Button:</p> \
					<table> \
						<tr> \
							<th>Button Pin</th> \
							<th>Wemos Pin</th> \
						</tr> \
						<tr> \
							<td>One leg</td> \
							<td>GND</td> \
						</tr> \
						<tr> \
							<td>Other leg</td> \
							<td>D4</td> \
						</tr> \
					</table> \
					<p>LED:</p> \
					<table> \
						<tr> \
							<th>LED Pin</th> \
							<th>Wemos Pin</th> \
						</tr> \
						<tr> \
							<td>Short leg</td> \
							<td>GND</td> \
						</tr> \
						<tr> \
							<td>Long leg</td> \
							<td>D3 (via resistor)</td> \
						</tr> \
					</table> \
					<p>RTC:</p> \
					<table> \
						<tr> \
							<th>RTC Pin</th> \
							<th>Wemos Pin</th> \
						</tr> \
						<tr> \
							<td>GND</td> \
							<td>GND</td> \
						</tr> \
						<tr> \
							<td>VCC</td> \
							<td>3.3v</td> \
						</tr> \
							<td>SCL</td> \
							<td>D1</td> \
						</tr> \
							<td>SDL</td> \
							<td>D2</td> \
						</tr> \
					</table> \
				</p> \
			</section> \
		</main> \
	</body> \
</html>";
}

/* Decided not to include this in the manual, as I'm submitting a video. This would go after the last </section> just before the </main> though
			<br><br> \
			<p> \
				You have my sincere apologies for this \
			</p> \
			<div style=\"display:flex;align-items:center;justify-content:center;\"> \
				<iframe width=\"1202\" height=\"676\" src=\"https://www.youtube.com/embed/dQw4w9WgXcQ\" frameborder=\"0\" allow=\"accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe> \
			</div> \
*/

#endif