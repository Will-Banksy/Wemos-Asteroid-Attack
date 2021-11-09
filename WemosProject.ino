// ===================================
// | WEMOS Project - Asteroid Attack |
// ===================================

// Author: William Banks

// The Plan:
//   Game: Ship dodging asteroids
//   Asteroids come from the top of the screen down, and the player dodges them by moving the ship horizontally across the bottom of the screen using the potentiometer
//   The LED is an indicator for when your player is damaged
//   The score will be incremented every game tick
//   Every 15 seconds (as counted by the RTC) a laser will fire randomly on the screen (with a warning, so it's possible to dodge)
//   The game will have 2 states - Playing and titlescreen
//     On Titlescreen, press the button to switch to playing, and to reset the RTC
//     Titlescreen will also show highscore, which will initially be 0
//   When you die (you have 3 lives, dying is losing all 3) you are returned to the titlescreen, and the highscore is updated if necessary
//   You can get an instruction manual for the game, and how to set up the Wemos, by running the this program on the Wemos, and then connecting to the AP the Wemos creates. SSID is "WemosAP", password is "passkey17". I've had troubles connecting to it and successfully requesting the page on my computer, but it definitely does work. I have tested it
//     An IP will be output via serial, just go to the root of that in a browser and you'll get a HTML instruction manual

// Reproduction Instructions
//
//   You have to connect both the OLED and RTC to D1 and D2 - These are different sets of those pins. The Wemos has 2 sets. I connected the RTC to the ones closest to the micro-usb port
//
//   Connect OLED:
//     OLED Pin -> Wemos Pin
//     GND      -> GND
//     VCC      -> 3.3v
//     SCL      -> D1
//     SDA      -> D2
//
//   Connect Potentiometer
//     Potentiometer Pin -> Wemos Pin
//     Either side pin   -> GND
//     Other side pin    -> 3.3v
//     Middle pin        -> A0
//
//   Connect Button
//     Button Pin -> Wemos Pin
//     One leg    -> GND
//     Other leg  -> D4
//
//   Connect LED
//     LED leg   -> Wemos Pin
//     Long leg  -> Resistor -> D3
//     Short leg -> GND
//
//   Connect RTC
//     RTC Pin -> Wemos Pin
//     GND     -> GND
//     VCC     -> 3.3v
//     SCL     -> D1
//     SDA     -> D2

// Notes about this project:
//   The #REGION and #ENDREGION are simply to organise my code into sections
//   Yes I did make my own simple library for interfacing with the OLED. I copied (and made effort to try and understand) some code from the Adafruit library for the actual communication with the OLED, but I've done all pixel/bitmap/font handling myself, including drawing the font and images from scratch
//   I hope the pascal case I like to use for functions doesn't anger you. Which is likely if you use Java
//   If the OLED isn't responsive, just unplug the OLED and plug it back in again. I don't know why that happens, it's not at all consistent
//   I put a lot of time and effort into this project as you hopefully will be able to tell and if I don't get good marks I will cry        (probably not really don't worry)

// #REGION Includes

// Include Serial Library
#include <Streaming.h>

// Include *My* OLED Interfacing Library (Very small library with a very long name)
#include "SSD1306_128x64_DisplayController.h" // Also includes <cstdint> for integer types. Also defines coord_t (alias for int16_t) and dims_t (alias for uint8_t)

// Include my sprites
#include "Sprites.h"

// Include my manual
#include "Manual.h"

// Include stuff for working with wifi and hosting a webserver
#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

// The stuff for working with the DS3231 Real Time Clock
#include <DS3231.h>
#include <SPI.h>

// #ENDREGION

// #REGION Defines

#define POTENTIOMETER A0
#define POTENTIOMETER_UPPERBOUND 1024 // The output of analogRead(A0) seems to always be between 0 and 1024
#define POTENTIOMETER_LOWERBOUND 0

// The standard HTTP port
#define HTTP_PORT 80

// The pin one end of the button is in
#define BUTTON_PIN D4

// The pin the LED used for indicating is used for
#define INDICATOR_LED_PIN D3

// The time returned by getTimestamp() when the time has been set to all 0s with setClock(0, 0, 0, 0, 0, 0); Is this Unix time? Idk. It doesn't really matter.
#define RTC_TIME_0 943920000

// #ENDREGION

// #REGION Global Classes/Structs/Enums

// I only made this a struct instead of a namespace cause then I can put it here
struct Utils {
	static float Map(float val, float start1, float stop1, float start2, float stop2) {
		return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
	}

	/// Returns a random number between lowerBound and higherBound, or lowerBound if higherBound is less than or equal to lowerBound
	/// Automatically seeds the random number generator by analogReading pin 0
	static bool seeded;
	static int16_t Rand(int16_t lowerBound, int16_t higherBound) {
		if(higherBound <= lowerBound) {
			return lowerBound;
		}
		if(!seeded) {
			randomSeed(analogRead(0)); // I don't know which pin this is, hopefully it's not connected to get a nice random output?
			seeded = true;
		}
		return random(lowerBound, higherBound); // Use Arduino random function
	}

	// Unlike the Arduino String.trim() function, this one actually works properly, and removes whitespace including newlines
	static String Trim(String str) {
		str.trim(); // Use the Arduino trim() function, just because that might work for other whitespace
		char* chars = new char[str.length() + 1]; // A c-string
		uint8_t arrayIndex = 0;
		for(uint8_t i = 0; i < str.length() + 1; i++) {
			if(i == str.length()) {
				chars[arrayIndex] = '\0'; // End the c-string with a null terminator
			}
			if(str[i] != '\n') { // If the character is not a newline, then add it to the c-string
				chars[arrayIndex] = str[i];
				arrayIndex++;
			}
		}
		return String(chars);
	}
};
bool Utils::seeded = false; // Out of class initialisation

/// A struct (which is basically a class, except everything is public by default) to keep track of an animation
struct AnimationState {
	uint8_t timer = 0;
	uint8_t delay;
	uint8_t frame = 0;
	uint8_t numFrames;

	constexpr AnimationState(uint8_t delay_, uint8_t numFrames_) : delay(delay_), numFrames(numFrames_) {
	}

	void Increment() {
		timer++;
		if(timer >= delay) {
			timer = 0;
			frame++;
			if(frame >= numFrames) {
				frame = 0;
			}
		}
	}

	void Reset() {
		frame = 0;
		timer = 0;
	}
};

/// Only works with horizontal spritesheets
/// Contains various bits of info such as coordinates, size, animation state and also an image. This makes things a lot easier
struct Entity {
	coord_t x = 0;
	coord_t y = 0;
	coord_t velX = 0;
	coord_t velY = 0;
	dims_t width;
	dims_t height;

	const uint8_t* bitmap;
	dims_t spriteWidth;
	dims_t spriteHeight;
	AnimationState* animation = nullptr;

	bool active = true;

	/// This class will not take ownership of either the bitmap or animation pointers
	Entity(dims_t width, dims_t height, const uint8_t* bitmap, dims_t spriteWidth, dims_t spriteHeight, AnimationState* animation, coord_t velX = 0, coord_t velY = 0, coord_t x = 0, coord_t y = 0) :
		x(x), // Yeah this is a pretty long initialiser list
		y(y),
		velX(velX),
		velY(velY),
		width(width),
		height(height),
		bitmap(bitmap),
		spriteWidth(spriteWidth),
		spriteHeight(spriteHeight),
		animation(animation) {
	}

	/// Very simply, if this entity is active, increases the coordinates by the velocity
	virtual void Update() {
		if(active) {
			x += velX;
			y += velY;
		}
	}

	/// This also advances the animation, if not null and advanceAnimation is true (default)
	virtual void Display(SSD1306_128x64_DisplayController& display, bool advanceAnimation = true) {
		if(!active) { return; } // If the Entity is not active, don't do anything
		if(animation) {
			display.DrawBitmap(bitmap, spriteWidth, spriteHeight, x, y, width * animation->frame, 0, width, height, true);
			if(advanceAnimation) {
				animation->Increment();
			}
		} else {
			display.DrawBitmap(bitmap, spriteWidth, spriteHeight, x, y, true);
		}
	}

	virtual bool Colliding(Entity& other) { // Slightly pinched (and modified) from java.awt.Rectangle cause it's too early in the morning to be thinking about rectangle collisions. Don't worry I understand it
		if(width == 0 || height == 0 || other.width == 0 || other.height == 0 || !active || !other.active) { // If either Entity is inactive or has a dimension of 0, then no collisions can happen
			return false;
		}

		coord_t x2 = x + width;
		coord_t y2 = y + height;
		coord_t otherX2 = other.x + other.width;
		coord_t otherY2 = other.y + other.height;
		//	integer overflow || intersect
		return ((x2 < x || x2 > other.x) &&
				(y2 < y || y2 > other.y) &&
				(otherX2 < other.x || otherX2 > x) &&
				(otherY2 < other.y || otherY2 > y));
	}
};

// A subclass of Entity
struct Laser : public Entity {
	uint16_t laserDelay = 15; // Remember this is in SECONDS
	bool firing = false;
	const uint8_t laserDuration = 5;
	uint8_t laserTimer = 0;
	bool canHitPlayer = false; // This is set to true once per activation - Don't want it hitting players multiple times in a single shot!
	bool doingWarning = false; // If this is set to true when active is true, then the laser is simply warning the player rather than being shot
	uint8_t warningTime = 8; // This is in GAME TICKS
	uint8_t warningTimer = 0;

	Laser() : Entity(Sprites::Laser::frameWidth, OLED_HEIGHT, Sprites::Laser::bitmap, Sprites::Laser::width, Sprites::Laser::height, new AnimationState(1, 5)) {
		active = false;
	}

	~Laser() {
		delete animation; // Since this was manually allocated, we actually need to clean up
	}

	/// This should be called instead of Update()
	void UpdateLaser(DS3231& rtc) {
		if(active) {
			if(!doingWarning) {
				laserTimer++;
				if(laserTimer == laserDuration) {
					active = false;
					laserTimer = 0;
					return;
				}
			} else {
				warningTimer++;
				if(warningTimer == warningTime) {
					warningTimer = 0;
					doingWarning = false;
				}
			}
		} else {
			if((rtc.getTimestamp() - RTC_TIME_0) > laserDelay) {
				active = true;
				canHitPlayer = true;
				animation->Reset();
				x = Utils::Rand(0, OLED_WIDTH - 1 - width);
				rtc.setClock(0, 0, 0, 0, 0, 0); // Reset the RTC
				doingWarning = true;
			}
		}
	}

	/// Don't call this - It'll do nothing
	void Update() override {
		// Do nothing
	}

	/// Override the display function
	void Display(SSD1306_128x64_DisplayController& display, bool advanceAnimation = true) override {
		if(active) {
			if(advanceAnimation) {
				animation->Increment();
			}

			if(doingWarning) {
				display.DrawBitmap(Sprites::Alert::bitmap, Sprites::Alert::width, Sprites::Alert::height, x + ((width / 2) - 1), OLED_HEIGHT / 2);
			} else {
				// Draw the laser vertically
				for(uint8_t i = y; i < (x + height); i++) {
					display.DrawBitmap(bitmap, spriteWidth, spriteHeight, x, i, width * animation->frame, 0, width, height, true);
				}
			}
		}
	}

	bool Colliding(Entity& other) override {
		if(active && !doingWarning) {
			return Entity::Colliding(other); // Call superclass method
		} else {
			return false;
		}
	}
};

enum GameState {
	PLAYING,
	TITLESCREEN
};

// #ENDREGION

// #REGION Global Variables

static SSD1306_128x64_DisplayController display;

static AnimationState shipAnimation(5, 2);

static Entity player(Sprites::Ship::frameWidth, Sprites::Ship::height, Sprites::Ship::bitmap, Sprites::Ship::width, Sprites::Ship::height, &shipAnimation);
static uint8_t playerLife;

static constexpr uint8_t numAsteroids = 6; // Anything that is constexpr is implicitly const
static Entity asteroids[numAsteroids] = {
	Entity(Sprites::Asteroid1::frameWidth, Sprites::Asteroid1::height, Sprites::Asteroid1::bitmap, Sprites::Asteroid1::width, Sprites::Asteroid1::height, nullptr, 0, 3),
	Entity(Sprites::Asteroid2::frameWidth, Sprites::Asteroid2::height, Sprites::Asteroid2::bitmap, Sprites::Asteroid2::width, Sprites::Asteroid2::height, nullptr, 0, 2, 10),
	Entity(Sprites::Asteroid3::frameWidth, Sprites::Asteroid3::height, Sprites::Asteroid3::bitmap, Sprites::Asteroid3::width, Sprites::Asteroid3::height, nullptr, 0, 1, 25),
	Entity(Sprites::Asteroid1::frameWidth, Sprites::Asteroid1::height, Sprites::Asteroid1::bitmap, Sprites::Asteroid1::width, Sprites::Asteroid1::height, nullptr, 0, 4),
	Entity(Sprites::Asteroid2::frameWidth, Sprites::Asteroid2::height, Sprites::Asteroid2::bitmap, Sprites::Asteroid2::width, Sprites::Asteroid2::height, nullptr, 0, 3, 10),
	Entity(Sprites::Asteroid3::frameWidth, Sprites::Asteroid3::height, Sprites::Asteroid3::bitmap, Sprites::Asteroid3::width, Sprites::Asteroid3::height, nullptr, 0, 2, 25)
};

static Laser laser;

static GameState gameState = TITLESCREEN;
static uint16_t highscore = 0;
static uint16_t score = 0;

static ESP8266WebServer server(HTTP_PORT);

static DS3231 rtc;

// #ENDREGION

// #REGION Global Functions

void UpdateAsteroids() {
	for(uint8_t i = 0; i < numAsteroids; i++) {
		Entity& asteroid = asteroids[i];
		if(!asteroid.active) {
			if(Utils::Rand(0, 10) == 0) { // 1 in 10 chance
				asteroid.x = Utils::Rand(0, OLED_WIDTH - 1 - asteroid.width);
				asteroid.y = -asteroid.height;
				asteroid.active = true;
			}
		} else if(asteroid.y > OLED_HEIGHT) {
			asteroid.active = false;
		}
	}
}

/// Sets up the AP and HTTP server
void SetupServer() {
	static String ssid = "WemosAP";
	static String password = "passkey17";
	WiFi.softAP(ssid, password);

	Serial << "WiFi Access Point started. Connect with SSID 'WemosAP' and Password 'passkey17'" << endl;

	server.on("/", []() {
		server.send(200, "text/html", Manual::manualHtml); // Send over the manual
	});
	server.begin(); // Start the server

	Serial << endl << "\nHTTP Server Started. Go to IP address (" << WiFi.softAPIP() << ") to read the manual" << endl;
}

/*void ConnectToWiFi(String& ssid, String& password) {
	Serial << endl << "\nConnecting to " << ssid;
	WiFi.begin(ssid, password);

	display.SendCommand(COM_SET_CONTRAST); // Lower the contrast
	display.SendCommand(0x09);

	while (WiFi.status() != WL_CONNECTED) { // Wait for the WiFi to connect
		delay(500);
		Serial.print(".");
	}

	Serial << "\nConnected. Local IP Address: " << WiFi.localIP() << endl; // Print out the contrast

	SetupServer();

	display.SendCommand(COM_SET_CONTRAST); // Reset the contrast
	display.SendCommand(0xCF);
}

void CheckSerial() {
	static bool promptedSsid = false;
	static bool promptedPass = false;
	static bool skipCheckSerial = false;
	static String ssid = "";
	static String password = "";
	if(skipCheckSerial) { return; }
	if(!promptedSsid) {
		Serial << endl << "\nIn order to read an instruction manual for the game, please connect the Wemos to WiFi" << endl;
		Serial << "Please enter network credentials" << endl;
		Serial << "Enter SSID:" << endl;
		promptedSsid = true;
	} else if(!promptedPass && ssid != "") {
		Serial << "Enter Password:" << endl;
		promptedPass = true;
	}
	if(Serial.available()) {
		if(ssid == "") {
			ssid = Serial.readString();
			ssid = Utils::Trim(ssid);
			Serial << "Recieved SSID: " << ssid << endl;
		} else if(password == "") {
			password = Serial.readString();
			password = Utils::Trim(password);
			Serial << "Recieved Password: " << password << endl;
			if(password != "") {
				skipCheckSerial = true;
				ConnectToWiFi(ssid, password);
			}
		}
	}
}*/

// #ENDREGION

void setup() { // Initialisation
	// Initialise Serial
	Serial.begin(115200);
	while(!Serial) {} // Loop until got Serial connection
	Serial << endl;
	delay(200); // Need a bit of delay or the Serial doesn't seem to correctly send what it should be sending. 200ms seems to be enough

	// Setup the Wifi Access Point and the HTTP server
	SetupServer();

	// Initialise the OLED
	display.Begin();
	display.ClearDisplay();
	display.Display();

	// Set the button pin as input
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	// Set the LED pin as output
	pinMode(INDICATOR_LED_PIN, OUTPUT);

	// Turn off the LED. HIGH = on, LOW = off
	digitalWrite(INDICATOR_LED_PIN, LOW);

	// Reset the RTC
	// void setClock(int year, int month, int day, int hour, int minute, int sec);
	rtc.setClock(0, 0, 0, 0, 0, 0);

	// Deactivate all asteroids
	for(uint8_t i = 0; i < numAsteroids; i++) {
		asteroids[i].active = false;
	}

	// Set the player's y position. This won't change. Except when sneakily using the player entity to draw life indicators
	player.y = OLED_HEIGHT - 1 - player.height;
}

void loop() { // Main loop
	// Handle WiFi stuff
	server.handleClient();
	delay(10); // Feed the watchdog

	// Clear the display buffer, and turn off the damage indicator LED
	display.ClearDisplay();
	digitalWrite(INDICATOR_LED_PIN, LOW);

	// Update and display the laser
	laser.UpdateLaser(rtc);
	laser.Display(display);

	switch(gameState) {
		case PLAYING: {
			int16_t analogue = analogRead(POTENTIOMETER);
			// Now take that value from the potentiometer and transform it to a suitable range
			uint8_t positionX = Utils::Map(analogue, POTENTIOMETER_LOWERBOUND, POTENTIOMETER_UPPERBOUND, 0, OLED_WIDTH - 6); // Map the value to the range 0-(128 - 1 - player.width) backwards - 0 -> (128 - 1 - player.width), 1024 -> 0
			player.x = positionX;

			// Check player-asteroid collisions, and take away player life as necessary
			for(uint8_t i = 0; i < numAsteroids; i++) {
				if(player.Colliding(asteroids[i])) {
					asteroids[i].active = false;
					playerLife--;
					digitalWrite(INDICATOR_LED_PIN, HIGH);
					if(playerLife == 0) {
						if(score > highscore) {
							highscore = score;
						}
						gameState = TITLESCREEN;
						for(uint8_t i = 0; i < numAsteroids; i++) { // Disable all asteroids - Then, when the game starts again, all asteroids will reset
							asteroids[i].active = false;
						}
						goto Case_Titlescreen; // Go directly to the titlescreen code
					}
				}
			}

			// Hmm. player.Colliding(laser) doesn't work correctly. Of course it doesn't. I could use templates but I'll just do it the other way around. Laser.Colliding(player) is what I meant to do
			if(laser.Colliding(player) && laser.canHitPlayer) {
				playerLife--;
				digitalWrite(INDICATOR_LED_PIN, HIGH); // Turn on hit indicator LED
				if(playerLife == 0) {
					if(score > highscore) {
						highscore = score;
					}
					gameState = TITLESCREEN;
					for(uint8_t i = 0; i < numAsteroids; i++) { // Disable all asteroids - Then, when the game starts again, all asteroids will reset
						asteroids[i].active = false;
					}
					goto Case_Titlescreen; // Go directly to the titlescreen code
				}
				laser.canHitPlayer = false;
			}

			// Display player
			player.Display(display);

			// Update and display the asteroids
			UpdateAsteroids();
			for(uint8_t i = 0; i < numAsteroids; i++) {
				asteroids[i].Update();
				asteroids[i].Display(display);
			}

			// Display the player life indicator
			// Draw a black box as a background to the life indicator
			uint8_t boxX = (OLED_WIDTH - 1) - (player.width * playerLife);
			display.FillRect(boxX, 0, (OLED_WIDTH - 1) - boxX, player.height, false);
			// Now be naughty and use the player Entity for drawing the player life indicator
			coord_t plrX = player.x;
			coord_t plrY = player.y;
			player.y = 0;
			for(uint8_t i = 0; i < playerLife; i++) {
				positionX = (OLED_WIDTH - 1) - (player.width * (i + 1)); // Reuse positionX variable
				player.x = positionX;
				player.Display(display, false); // Display the player in the new position without advancing the animation
			}
			player.x = plrX; // Restore the previous player position
			player.y = plrY;

			// Display score
			String scoreStr = String("Score: ") + String(score);
			uint8_t scorePixelWidth = scoreStr.length() * display.font.charWidth;
			uint8_t scorePixelHeight = display.font.charHeight;
			display.FillRect(0, 0, scorePixelWidth, scorePixelHeight, false);
			display.DrawString(scoreStr, 0, 0);

			// Increment the score every tick
			score++;

			// Break so as to not flow into the next case
			break;
		}

		case TITLESCREEN: {
			Case_Titlescreen: // Label to go to
			// Draw highscore
			String highscoreStr = String("Highscore: ") + String(highscore);
			display.DrawString(highscoreStr, 0, 0);

			// Draw title
			display.DrawBitmap(Sprites::AsteroidAttack::bitmap, Sprites::AsteroidAttack::width, Sprites::AsteroidAttack::height, 0, 0, true);

			// Reset the RTC
			// void setClock(int year, int month, int day, int hour, int minute, int sec);
			rtc.setClock(0, 0, 0, 0, 0, 0);

			// Check for button press to switch to PLAYING state
			if(!digitalRead(BUTTON_PIN)) {
				score = 0;
				playerLife = 3;
				gameState = PLAYING;
			}

			// The break is unneccessary here, but it's good practice
			break;
		}
	}

	// Update the display
	display.Display();
}