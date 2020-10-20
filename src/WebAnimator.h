#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <U8g2lib.h>

class WebAnimator {

//protected:
    HTTPClient * _client = nullptr;
    U8G2 * _display = nullptr;
    DynamicJsonDocument * _doc = nullptr;
    const int _displayHeight;
    const int _displayWidth;
    const int _fontHeight;
    const int _fontWidth;

    void initU8g2();

    void displayPageInternal(JsonObject & page);

    void displayPagesInternal(DynamicJsonDocument & doc);

public:
    WebAnimator(HTTPClient & client, U8G2 & display);
    ~WebAnimator();

    void requestAnimation(String url);

    void displayAnimationOnce();

    void displayText(String text);

};