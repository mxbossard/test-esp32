#include <WebAnimator.h>

/**
 * constructor
 */
WebAnimator::WebAnimator() {
}

/**
 * destructor
 */
WebAnimator::~WebAnimator() {
    if(_client) {
        _client->end();
    }
}

/**
 * @param client HTTPClient&
 * @return success bool
 */
bool WebAnimator::begin(HTTPClient &client, U8G2 &display) {
    _client = &client;
    _display = &display;
    _doc = & DynamicJsonDocument(4096);
    _screenHeight = display.getHeight();
    _screenWidth = display.getWidth();
    _fontHeight = 17;
    _fontWidth = 7;

    // ArduinoJson only support HTTP1.0 streaming.
    _client->useHTTP10(true);

    _display->begin();
    _display->setFont(u8g2_font_7x14_tf);
    _display->setFontRefHeightExtendedText();
    _display->setDrawColor(1);
    _display->setFontPosTop();
    _display->setFontDirection(0);
}

/**
 * @param url String
 */
void WebAnimator::requestAnimation(String url) {
    _client->begin(url);
    int httpCode = _client->GET();
    if (httpCode == 200) {
        // Send a GET request to a web page hosted by the server.
        ReadLoggingStream wifiClientWithLog(_client->getStream(), Serial);

        //sendHttpGet(&client, httpHost, httpEndpoint);
        
        DeserializationError err = deserializeJson(*_doc, wifiClientWithLog);

        if(err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
        }
    } else {
        // FIXME what to do ?
    }
}

/**
 */
void WebAnimator::displayAnimationOnce() {
    displayPagesInternal(*_doc);
}

/**
 */
void WebAnimator::displayText(String text) {
    String jsonPayload = "{\"pages\": [{\"text\": \"" + text + "\"}]}";

    DeserializationError err = deserializeJson(*_doc, jsonPayload);
    if(err) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
    }

    displayPagesInternal(*_doc);
}

void WebAnimator::displayPageInternal(JsonObject &page) {
    if (!page) return;
    const char* text = page["text"];

    Serial.print(F("Will display: "));
    Serial.println(text);

    _display->clear();

    // Code snippet : wrap a string to display

    // Allocate 2 bytes of memory to currentChar.
    char* currentChar = (char*) malloc(sizeof(char) * 2);
    char* nextChar = (char*) malloc(sizeof(char) * 2);
    // Init properly currentChar with an ending char array.
    strncpy(currentChar, "a", sizeof(char) * 2);
    strncpy(nextChar, "a", sizeof(char) * 2);

    uint16_t i, x, y;
    // u8x8 does not wrap lines.
    x = 0; y = 0;
    for (i = 0 ; i < strlen(text) && (y + _fontHeight) <= _screenHeight ; i ++) {
        strncpy(currentChar, &text[i], sizeof(char) * 1);

        if (strcmp(currentChar, "\n") == 0) {
            // Line feed
            x = 0;
            y += _fontHeight + 2;
            continue;
        }

        int charWidth = _display->getStrWidth(currentChar);
        if (strcmp(currentChar, " ") == 0) {
            // Special fix reduced width for spaces
            charWidth = charWidth / 2;
        }
        
        if (x + charWidth > _screenWidth) {
            // Reaching end of line
            if ((i + 1) < strlen(text)) {
                strncpy(nextChar, &text[i + 1], sizeof(char) * 1);
                if (strcmp(nextChar, " ") != 0) {
                    // if following char not a space print a dash
                    _display->print("-");
                }
            }
            x = 0;
            y += _fontHeight + 2;
        }

        if (x == 0 && strcmp(currentChar, " ") == 0) {
            // Remove spaces if first line chars
            continue;
        }

        _display->setCursor(x, y);
        _display->print(text[i]);
        /*
        Serial.print("x: ");
        Serial.print(x);
        Serial.print(" ; y: ");
        Serial.print(y);
        Serial.print(" ; width: ");
        Serial.print(charWidth);
        Serial.print(" ; char: ");
        Serial.println(currentChar);
        */
        
        x += charWidth + 1;
    }

    _display->sendBuffer();
}

void WebAnimator::displayPagesInternal(DynamicJsonDocument &doc) {
    JsonArray pages = doc["pages"];
    if (!pages) return;

    //Serial.println("Will display some pages ...");

    // Walk the JsonArray efficiently
    for(JsonObject page: pages) {
        displayPageInternal(page);
        delay(3000);
    }
}